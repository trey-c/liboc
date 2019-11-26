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

int main(void)
{
    oc_hash_table_t *hash_table = NULL;

    hash_table = oc_hash_table_new(oc_hash_str, oc_hash_compare_str);

    oc_assert(hash_table != NULL);

    oc_hash_table_insert(hash_table, "akey", "adata");
    oc_hash_table_insert(hash_table, "bkey", "bdata");
    oc_hash_table_insert(hash_table, "ckey", "cdata");

    oc_assert(oc_hash_table_length(hash_table) == 3);

    oc_hash_table_remove(hash_table, "bkey");

    oc_assert(oc_hash_table_length(hash_table) == 2);

    return 0;
}
