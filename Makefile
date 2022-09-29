CC ?= cc
CFLAGS += -Wall -pipe
OFLAGS ?= -Ofast
LFLAGS += -lcrypto

all:	ed25519-verify test

ed25519-verify:	main.c
	$(CC) $(CFLAGS) $(OFLAGS) -o ed25519-verify main.c $(LFLAGS)

test:	ed25519-verify
	echo -n 'Message for Ed25519 signing' | ./ed25519-verify b7a3c12dc0c8c748ab07525b701122b88bd78f600c76342d27f25e5f92444cde 6dd355667fae4eb43c6e0ab92e870edb2de0a88cae12dbd8591507f584fe4912babff497f1b8edf9567d2483d54ddc6459bea7855281b7a246a609e3001a4e08

clean:
	rm -f *.o ed25519-verify
