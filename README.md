# ed25519-verify
Greg Kennedy 2022

Verify an ed25519 signature via CLI (C + OpenSSL).

This is a minimal example to verify an ed25519 signature as a command-line tool.  Usage is:

`ed25519-verify <key> <signature>`

The message should be passed on STDIN (via cat, echo, etc).  Remember that some Unix tools append a newline by default, and this will cause the signature to differ.

NOTE: proper cleanup of buffers / structures is omitted, be cautious embedding this in any larger project
