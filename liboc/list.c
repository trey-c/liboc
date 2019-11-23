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

#include <liboc/list.h>

#include <liboc/utils.h>

oc_list_t *oc_list_append(oc_list_t *list, void *data)
{
    oc_list_t **new_head = &list;
    oc_list_t *entry = calloc(1, sizeof(oc_list_t));

    oc_return_val_if_fail(entry != NULL, NULL);

    entry->data = data;

    while (*new_head != NULL) {
        if ((*new_head)->next == NULL)
            entry->prev = *new_head;

        new_head = &(*new_head)->next;
    }

    *new_head = entry;

    return list;
}

oc_list_t *oc_list_remove_node(oc_list_t *list, oc_list_t *node)
{
    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;

    if (node == list)
        list = list->next;

    oc_free(node);

    return list;
}

oc_list_t *oc_list_remove(oc_list_t *list, void *data)
{
    oc_list_t *node = list;

    while (node != NULL) {
        if (node->data == data)
            return oc_list_remove_node(list, node);

        node = node->next;
    }

    return list;
}

int oc_list_length(oc_list_t *list)
{
    int length = 0;

    while (list != NULL) {
        length++;
        list = list->next;
    }

    return length;
}

oc_list_t *oc_list_find(oc_list_t *list, void *data)
{
    while (list != NULL) {
        if (list->data == data)
            break;
        list = list->next;
    }

    return list;
}

oc_list_t *oc_list_clear(oc_list_t *list)
{
    oc_list_t *node;

    while (list != NULL) {
        node = list;
        list = list->next;
        oc_list_remove_node(list, node);
    }

    return NULL;
}
