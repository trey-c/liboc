/*
 * OC - Overpowered C
 *
 * Copyright © 2019 Trey Cutter <treycutter@protonmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { OC_FALSE, OC_TRUE } oc_bool;

void oc_fprint(FILE *file, const char *format, ...);

#define oc_print(f_, ...) oc_fprint(stdout, (f_), ##__VA_ARGS__)

#define oc_warning(format, ...) oc_fprint(stdout, ("\e[33mWarning:\e[0m " format), ##__VA_ARGS__);

#define oc_error(format, ...)                                            \
    do {                                                                 \
        oc_fprint(stderr, ("\e[31mError:\e[0m " format), ##__VA_ARGS__); \
        abort();                                                         \
    } while (0);

#define oc_assert(expr) \
    if (!(expr))        \
        oc_error("%s:%d: Assertion '%s' failed.\n", __FILE__, __LINE__, #expr);

#define oc_return_if_fail(exp)               \
    if (!(exp)) {                            \
        oc_warning("If '%s' failed.", #exp); \
        return;                              \
    }

#define oc_return_val_if_fail(exp, val)      \
    if (!(exp)) {                            \
        oc_warning("If '%s' failed.", #exp); \
        return (val);                        \
    }

void oc_free(void *ptr);
