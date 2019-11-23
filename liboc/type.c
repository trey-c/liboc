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

#include <liboc/type.h>

#include <liboc/list.h>

static oc_list_t *g_type_list = NULL;

static oc_type_t *oc_type_find(char *name)
{
    oc_list_foreach(node, g_type_list)
    {
        if (strcmp(((oc_type_t *)node->data)->name, name) == 0)
            return (oc_type_t *)node->data;
    }

    return NULL;
}

oc_type_t *oc_type_register(oc_type_t *parent, char *name, int klass_size,
                            oc_class_init_t klass_init, int instance_size,
                            oc_instance_init_t instance_init,
                            oc_instance_destroy_t instance_destroy, oc_type_flags_t type_flags)
{
    oc_type_t *type = oc_type_find(name);

    if (type != NULL)
        return type;

    type = calloc(1, sizeof(oc_type_t));

    oc_return_val_if_fail(type != NULL, NULL);

    type->parent = parent;
    type->name = name;
    type->klass_size = klass_size;
    type->klass_init = klass_init;
    type->klass = NULL;
    type->instance_size = instance_size;
    type->instance_init = instance_init;
    type->instance_destroy = instance_destroy;
    type->flags = type_flags;
    type->ref_count = 0;

    if (parent != NULL)
        oc_type_ref(parent);

    g_type_list = oc_list_append(g_type_list, type);

    return type;
}

void oc_type_ref(oc_type_t *type)
{
    type->ref_count += 1;
}

void oc_type_unref(oc_type_t *type)
{
    type->ref_count -= 1;

    if (type->ref_count != 0)
        return;

    if (!(type->flags & OC_TYPE_FLAG_FUNDAMENTAL))
        oc_type_unref(type->parent);

    g_type_list = oc_list_remove(g_type_list, type);
    oc_free(type);
}

void oc_type_class_ref(oc_type_class_t *klass)
{
    klass->ref_count += 1;
}

void oc_type_class_unref(oc_type_class_t *klass)
{
    klass->ref_count -= 1;

    if (klass->ref_count != 0)
        return;

    klass->type->klass = NULL;
    oc_free(klass);
}

oc_type_instance_t *oc_type_instance_new(oc_type_t *type)
{
    oc_type_instance_t *instance;

    if (type->flags & OC_TYPE_FLAG_ABSTRACT) {
        oc_warning("Cannot create a new instance of an abstract type: %s", type->name);
        return NULL;
    }

    instance = calloc(1, type->instance_size);

    oc_return_val_if_fail(instance != NULL, NULL);

    if (type->klass == NULL)
        type->klass = oc_type_class_new(type);

    oc_type_class_ref(type->klass);
    instance->klass = type->klass;
    oc_type_ref(type);

    oc_type_instance_init_chainup(type, instance);

    return instance;
}

oc_type_class_t *oc_type_class_new(oc_type_t *type)
{
    oc_type_class_t *klass = calloc(1, type->klass_size);

    oc_return_val_if_fail(klass != NULL, NULL);

    klass->type = type;

    oc_type_class_init_chainup(type, klass);

    return klass;
}

oc_bool oc_type_check_derivation(oc_type_t *type, oc_type_t *parent)
{
    while (type != NULL) {
        if (type->name == parent->name)
            return OC_TRUE;

        type = type->parent;
    }

    return OC_FALSE;
}

oc_type_instance_t *oc_type_instance_cast(oc_type_instance_t *self, oc_type_t *type)
{
    oc_return_val_if_fail(self != NULL, NULL);
    oc_return_val_if_fail(self->klass != NULL, NULL);

    if (oc_type_check_derivation(self->klass->type, type) == 0)
        oc_error("Cannot cast an %s to an %s!", self->klass->type->name, type->name);

    return self;
}

oc_type_class_t *oc_type_class_cast(oc_type_class_t *self, oc_type_t *type)
{
    oc_return_val_if_fail(self != NULL, NULL);
    oc_return_val_if_fail(self->type != NULL, NULL);
    oc_return_val_if_fail(self->type->name != NULL, NULL);

    if (oc_type_check_derivation(self->type, type) == 0)
        oc_error("Cannot cast an %s (class) to an %s (class)!", self->type->name, type->name);

    return self;
}

void oc_type_class_init_chainup(oc_type_t *type, oc_type_class_t *klass)
{
    if (type->parent != NULL)
        oc_type_class_init_chainup(type->parent, klass);

    type->klass_init(klass);
}

void oc_type_instance_init_chainup(oc_type_t *type, oc_type_instance_t *instance)
{
    if (type->parent != NULL)
        oc_type_instance_init_chainup(type->parent, instance);

    type->instance_init(instance);
}

void oc_type_instance_destroy_chaindown(oc_type_t *type, oc_type_instance_t *instance)
{
    type->instance_destroy(instance);

    if (type->parent != NULL)
        oc_type_instance_destroy_chaindown(type->parent, instance);
}

void oc_type_instance_free(oc_type_instance_t *instance)
{
    oc_type_t *type = instance->klass->type;

    oc_type_instance_destroy_chaindown(type, instance);

    oc_type_class_unref(instance->klass);
    oc_type_unref(type);

    oc_free(instance);
}
