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

#include <liboc/object.h>

static void oc_object_class_init(oc_object_class_t *klass);
static void oc_object_init(oc_object_t *self);
static void oc_object_destroy(oc_object_t *self);

oc_type_t *oc_object_get_type(void)
{
    return oc_type_register(
        NULL, "oc_object", sizeof(oc_object_class_t), (oc_class_init_t)oc_object_class_init,
        sizeof(oc_object_t), (oc_instance_init_t)oc_object_init,
        (oc_instance_destroy_t)oc_object_destroy, OC_TYPE_FLAG_FUNDAMENTAL | OC_TYPE_FLAG_OBJECT);
}

static void oc_object_class_init(oc_object_class_t *klass)
{
}

static void oc_object_init(oc_object_t *self)
{
}

static void oc_object_destroy(oc_object_t *self)
{
}

void *oc_object_new(oc_type_t *type)
{
    oc_object_t *obj = (oc_object_t *)oc_type_instance_new(type);

    oc_return_val_if_fail(obj != NULL, NULL);

    oc_object_ref(obj);

    return obj;
}

void *oc_object_ref(void *object)
{
    oc_object_t *obj = object;

    oc_return_val_if_fail(obj != NULL, NULL);
    oc_return_val_if_fail(IS_OC_OBJECT(obj), NULL);

    obj->ref_count += 1;

    return obj;
}

void oc_object_unref(void *object)
{
    oc_object_t *obj = object;

    oc_return_if_fail(obj != NULL);
    oc_return_if_fail(IS_OC_OBJECT(obj));

    obj->ref_count -= 1;

    if (obj->ref_count == 0)
        oc_type_instance_free((oc_type_instance_t *)obj);
}

void oc_object_free(void *object)
{
    oc_object_t *obj = object;

    oc_return_if_fail(obj != NULL);
    oc_return_if_fail(IS_OC_OBJECT(obj));

    oc_type_instance_free((oc_type_instance_t *)obj);
}
