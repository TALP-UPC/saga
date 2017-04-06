/* coding: utf-8 */
/* Saga - Un transcriptor fonético para el idioma español
 *
 * Copyright (C) 1993-2009  Albino Nogueiras Rodríguez y José B. Mariño
 *       TALP - Universitat Politècnica de Catalunya, ESPAÑA
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "config.h"

#ifndef POSIX_COMPAT_H
#define POSIX_COMPAT_H

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#if HAVE_UNISTD_H
#include <unistd.h>
#endif


#if !HAVE_DECL_STRDUP
char *strdup(const char *str);
#endif

#if !HAVE_DECL_GETOPT
int getopt(int nargc, char *const *nargv, const char *ostr);
extern char *optarg;
extern int optind, opterr, optopt;
#endif

#if !HAVE_DECL_GETLINE
#include <stdio.h>
#include <stdint.h>

#ifndef ssize_t
#define ssize_t int64_t
#endif
#ifndef SSIZE_MAX
#define SSIZE_MAX INT64_MAX
#endif
ssize_t getline(char **lineptr, size_t *n, FILE *stream);
#endif


#endif
