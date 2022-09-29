#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/evp.h>

int main(int argc, char * argv[])
{
	// The expected usage for verify is
	//  cat message | ed25519 key signature
	// so we need 2 parameters
	if (argc != 3) {
		printf("usage: %s <key> <signature>\n", argv[0]);
		return EXIT_FAILURE;
	}

	// copy key to buffer
	long buflen;
	unsigned char * key = OPENSSL_hexstr2buf(argv[1], &buflen);

	if (! key) {
		ERR_print_errors_fp(stderr);
		return EXIT_FAILURE;
	}

	if (buflen != 32) {
		fprintf(stderr, "Invalid key length %ld\n", buflen);
		return EXIT_FAILURE;
	}

	EVP_PKEY * pkey = EVP_PKEY_new_raw_public_key(EVP_PKEY_ED25519, NULL, key, 32);
	// copy signature to buffer
	unsigned char * sig = OPENSSL_hexstr2buf(argv[2], &buflen);

	if (! sig) {
		ERR_print_errors_fp(stderr);
		return EXIT_FAILURE;
	}

	if (buflen != 64) {
		fprintf(stderr, "Invalid sig length %ld\n", buflen);
		return EXIT_FAILURE;
	}

	// read message from stdin
	int msg_len = 0, msg_capacity = 0;
	unsigned char * msg = NULL;
	int c;

	while ((c = getchar()) != EOF) {
		if (msg_len >= msg_capacity) {
			msg_capacity = (msg_capacity * 1.5) + 1;
			msg = realloc(msg, msg_capacity);

			if (msg == NULL) {
				perror("msg = realloc() error");
				return EXIT_FAILURE;
			}
		}

		msg[msg_len] = c;
		msg_len ++;
	}

	// use OpenSSL to do validation of signature
	EVP_MD_CTX * md_ctx = EVP_MD_CTX_new();

	if (! EVP_DigestVerifyInit(md_ctx, NULL, NULL, NULL, pkey)) {
		ERR_print_errors_fp(stderr);
		return EXIT_FAILURE;
	}

	int ret = EVP_DigestVerify(md_ctx, sig, 64, msg, msg_len);

	if (ret == 1) {
		printf("Signature valid.\n");
		return EXIT_SUCCESS;
	} else if (ret == 0) {
		printf("Signature INVALID.\n");
		return EXIT_FAILURE;
	} else {
		ERR_print_errors_fp(stderr);
		return EXIT_FAILURE;
	}
}
