/*
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

/*
 * getpass.c: read a passphrase from the keyboard without echoing to stdout
 *            nor stderr.
 *
 * The code is not mine, but it belongs to glibc authors. My only
 * contribution is based on adapt it to this program. For further
 * information, see https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_node/libc_649.html
 *
 * Daniel Dwek
 *
 */
#ifndef _GET_PASS_H_
#define _GET_PASS_H_
#include <stdio.h>
#include <stddef.h>
#include <termios.h>

int my_getpass(char **lineptr, size_t *n, FILE *stream)
{
        struct termios old, new;
        int nread;

        /* Turn echoing off and fail if we can't. */
        if (tcgetattr(fileno(stream), &old) != 0)
                return -1;

        new = old;
        new.c_lflag &= ~ECHO;
        if (tcsetattr(fileno(stream), TCSAFLUSH, &new) != 0)
                return -1;

        /* Read the passphrase */
        nread = getline(lineptr, n, stream);

        /* Restore terminal. */
        tcsetattr(fileno(stream), TCSAFLUSH, &old);

        return nread;
}
#endif
