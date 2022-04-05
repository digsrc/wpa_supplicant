/*
 * WPA Supplicant - ASCII passphrase to WPA PSK tool
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include <stdio.h>
#include "includes.h"

#include "common.h"
#include "crypto/sha1.h"
extern int my_getpass(char **lineptr, size_t *n, FILE *stream);

int main(int argc, char *argv[])
{
	unsigned char psk[32];
	int i;
	char *ssid, *passphrase, **buf = NULL;
	int n = 64;
	FILE *stream = NULL;

	if (argc < 2) {
		printf("usage: wpa_passphrase <ssid> [passphrase]\n"
			"\nIf passphrase is left out, it will be read from "
			"stdin\n");
		return 1;
	}

	ssid = argv[1];

	if (argc > 2) {
		passphrase = argv[2];
	} else {
		printf("# reading passphrase from stdin\n");
		buf = (char **)calloc(1, sizeof(char *));
		if (buf)
			buf[0] = (char *)calloc(1, 64);

		if (buf[0]) {
			stream = fdopen(0, "r");
			my_getpass(buf, (size_t *)&n, stream);
		}
		
		passphrase = buf[0];
	}

	if (os_strlen(passphrase) < 8 || os_strlen(passphrase) > 63) {
		printf("Passphrase must be 8..63 characters\n");
		return 1;
	}

	pbkdf2_sha1(passphrase, (u8 *) ssid, os_strlen(ssid), 4096, psk, 32);

	printf("network={\n");
	printf("\tssid=\"%s\"\n", ssid);

	// This should be not displayed if standard output is set to stdout, but
	// we do that for backward compatibility
	printf("\t#psk=\"%s\"\n", passphrase);

	printf("\tpsk=");
	for (i = 0; i < 32; i++)
		printf("%02x", psk[i]);
	printf("\n");
	printf("}\n");

	return 0;
}
