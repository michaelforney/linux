/* Extract X.509 certificate in DER form from PKCS#11 or PEM.
 *
 * Copyright © 2014-2015 Red Hat, Inc. All Rights Reserved.
 * Copyright © 2015      Intel Corporation.
 *
 * Authors: David Howells <dhowells@redhat.com>
 *          David Woodhouse <dwmw2@infradead.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; either version 2.1
 * of the licence, or (at your option) any later version.
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <err.h>
#include <bearssl.h>

/*
 * OpenSSL 3.0 deprecates the OpenSSL's ENGINE API.
 *
 * Remove this if/when that API is no longer used
 */
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#define PKEY_ID_PKCS7 2

static __attribute__((noreturn))
void format(void)
{
	fprintf(stderr,
		"Usage: extract-cert <source> <dest>\n");
	exit(2);
}

static const char *key_pass;
static char *cert_dst;
static bool verbose;

static void write_cert(void *dst_ctx, const void *src, size_t len)
{
	FILE *dst = dst_ctx;

	if (fwrite(src, 1, len, dst) != len)
		err(1, "write %s", cert_dst);
}

int main(int argc, char **argv)
{
	char *cert_src;
	char *verbose_env;

	verbose_env = getenv("KBUILD_VERBOSE");
	if (verbose_env && strchr(verbose_env, '1'))
		verbose = true;

        key_pass = getenv("KBUILD_SIGN_PIN");

	if (argc != 3)
		format();

	cert_src = argv[1];
	cert_dst = argv[2];

	if (!cert_src[0]) {
		/* Invoked with no input; create empty file */
		FILE *f = fopen(cert_dst, "wb");
		if (!f)
			err(1, "%s", cert_dst);
		fclose(f);
		exit(0);
	} else if (!strncmp(cert_src, "pkcs11:", 7)) {
		errx(1, "PKCS#11 certificates are not supported");
	} else {
		FILE *src, *dst;
		br_pem_decoder_context ctx;
		char buf[8192], *pos;
		size_t len = 0, n;

		src = fopen(cert_src, "rb");
		if (!src)
			err(1, "open %s", cert_src);
		dst = fopen(cert_dst, "wb");
		if (!dst)
			err(1, "open %s", cert_dst);

		br_pem_decoder_init(&ctx);
		br_pem_decoder_setdest(&ctx, write_cert, dst);
		for (;;) {
			if (len == 0) {
				if (feof(src))
					break;
				len = fread(buf, 1, sizeof(buf), src);
				if (ferror(src))
					err(1, "read %s", cert_src);
				pos = buf;
			}
			n = br_pem_decoder_push(&ctx, pos, len);
			pos += n;
			len -= n;
			if (br_pem_decoder_event(&ctx) == BR_PEM_ERROR)
				errx(1, "PEM decode failure");
		}
		if (fflush(dst) != 0)
			err(1, "flush %s", cert_dst);
	}

	return 0;
}
