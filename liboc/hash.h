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

#include <liboc/utils.h>

typedef int (*oc_hash_func_t)(void *key);
typedef oc_bool (*oc_hash_compare_func_t)(void *key1, void *key2);

typedef struct oc_hash_bucket oc_hash_bucket_t;
typedef struct oc_hash_table oc_hash_table_t;

struct oc_hash_bucket {
    void *key;
    void *data;

    oc_hash_bucket_t *next;
};

struct oc_hash_table {
    int size;
    int nbuckets;

    oc_hash_bucket_t **buckets;
    oc_hash_func_t hash_func;
    oc_hash_compare_func_t compare_func;
};

oc_hash_table_t *oc_hash_table_new(oc_hash_func_t hash_func, oc_hash_compare_func_t compare_func);

void oc_hash_table_insert(oc_hash_table_t *hash_table, void *key, void *value);

void oc_hash_table_remove(oc_hash_table_t *hash_table, void *key);

void *oc_hash_table_lookup(oc_hash_table_t *hash_table, void *key);

void g_hash_table_destroy(void *hash_table);

int oc_hash_table_length(oc_hash_table_t *hash_table);

int oc_hash_str(void *key);

oc_bool oc_hash_compare_str(void *key1, void *key2);
