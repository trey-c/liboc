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

typedef struct oc_list oc_list_t;

struct oc_list {
    void *data;

    oc_list_t *next;
    oc_list_t *prev;
};

#define oc_list_foreach(node, list) for (oc_list_t *node = list; node != NULL; node = node->next)

oc_list_t *oc_list_append(oc_list_t *list, void *data);

oc_list_t *oc_list_remove_node(oc_list_t *list, oc_list_t *node);

oc_list_t *oc_list_remove(oc_list_t *list, void *data);

int oc_list_length(oc_list_t *list);

oc_list_t *oc_list_find(oc_list_t *list, void *data);

oc_list_t *oc_list_clear(oc_list_t *list);
