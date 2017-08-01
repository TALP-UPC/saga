/* coding: utf-8 */
/* No copyright is applicable */
/* The functions here have been released under public domain */

#include "PosixCompat.h"

#include <string.h>
#include <stdlib.h>

char *saga_strdup(const char *str)
{
    char *str_copy = malloc(strlen(str)+1);
    if (str_copy == NULL) return NULL;
    strcpy(str_copy, str);
    return str_copy;
}


/* saga_getline.c
 *
 * saga_getdelim(), saga_getline() - read a delimited record from stream, ersatz implementation
 *
 * For more details, see: http://pubs.opengroup.org/onlinepubs/9699919799/functions/saga_getline.html
 *
 * This implementation of saga_getline was released into the public domain at 
 * https://github.com/ivanra/saga_getline
 *
 *
 */
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

ssize_t saga_getdelim(char **lineptr, size_t *n, int delim, FILE *stream) {
    char *cur_pos, *new_lineptr;
    size_t new_lineptr_len;
    int c;

    if (lineptr == NULL || n == NULL || stream == NULL) {
#ifdef EINVAL
        errno = EINVAL;
#endif
        return -1;
    }

    if (*lineptr == NULL) {
        *n = 128; /* init len */
        if ((*lineptr = (char *)malloc(*n)) == NULL) {
#ifdef ENOMEM
            errno = ENOMEM;
#endif
            return -1;
        }
    }

    cur_pos = *lineptr;
    for (;;) {
        c = getc(stream);

        if (ferror(stream) || (c == EOF && cur_pos == *lineptr))
            return -1;

        if (c == EOF)
            break;

        if ((*lineptr + *n - cur_pos) < 2) {
            if (SSIZE_MAX / 2 < *n) {
#ifdef EOVERFLOW
                errno = EOVERFLOW;
#else
                errno = ERANGE; /* no EOVERFLOW defined */
#endif
                return -1;
            }
            new_lineptr_len = *n * 2;

            if ((new_lineptr = (char *)realloc(*lineptr, new_lineptr_len)) == NULL) {
#ifdef ENOMEM
                errno = ENOMEM;
#endif
                return -1;
            }
            *lineptr = new_lineptr;
            *n = new_lineptr_len;
        }

        *cur_pos++ = (char) c;

        if (((char) c) == delim)
            break;
    }

    *cur_pos = '\0';
    return (ssize_t)(cur_pos - *lineptr);
}

ssize_t saga_getline(char **lineptr, size_t *n, FILE *stream) {
    return saga_getdelim(lineptr, n, '\n', stream);
}
