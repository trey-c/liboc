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

#include <liboc/core.h>

int main(int argc, char **argv)
{
    oc_list_t *list = NULL;
    oc_list_t *found = NULL;

    list = oc_list_append(list, "aa");
    list = oc_list_append(list, "bb");
    list = oc_list_append(list, "cc");
    list = oc_list_append(list, "dd");
    list = oc_list_remove(list, "aa");
    list = oc_list_remove(list, "ee");

    found = oc_list_find(list, "bb");

    oc_assert(oc_list_length(list) == 3);
    oc_assert(strcmp((char *)found->data, "bb") == 0);

    list = oc_list_clear(list);

    return 0;
}
