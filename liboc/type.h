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

typedef enum oc_type_flags oc_type_flags_t;

typedef struct oc_type oc_type_t;
typedef struct oc_type_instance oc_type_instance_t;
typedef struct oc_type_class oc_type_class_t;

typedef void (*oc_class_init_t)(oc_type_class_t *);
typedef void (*oc_instance_init_t)(oc_type_instance_t *);
typedef void (*oc_instance_destroy_t)(oc_type_instance_t *);

#define OC_DECLARE_TYPE_EXTENDED(type_name, TYPE_NAME, parent_name)                     \
    oc_type_t *type_name##_get_type(void);                                              \
                                                                                        \
    typedef struct type_name type_name##_t;                                             \
    typedef struct type_name##_class type_name##_class_t;                               \
                                                                                        \
    static inline oc_type_t *TYPE_NAME##_TYPE()                                         \
    {                                                                                   \
        return type_name##_get_type();                                                  \
    }                                                                                   \
    static inline type_name##_t *TYPE_NAME(void *self)                                  \
    {                                                                                   \
        return (type_name##_t *)oc_type_instance_cast((oc_type_instance_t *)self,       \
                                                      type_name##_get_type());          \
    }                                                                                   \
    static inline int IS_##TYPE_NAME(void *self)                                        \
    {                                                                                   \
        return oc_type_check_derivation(((oc_type_instance_t *)self)->klass->type,      \
                                        type_name##_get_type());                        \
    }                                                                                   \
    static inline type_name##_class_t *TYPE_NAME##_GET_CLASS(type_name##_t *self)       \
    {                                                                                   \
        return ((type_name##_class_t *)((oc_type_instance_t *)self)->klass);            \
    }                                                                                   \
    static inline type_name##_class_t *TYPE_NAME##_CLASS(void *self)                    \
    {                                                                                   \
        return (type_name##_class_t *)oc_type_class_cast(self, type_name##_get_type()); \
    }

#define OC_DEFINE_TYPE_EXTENDED(type_name, PARENT_TYPE, type_flags)                             \
    static void type_name##_class_init(type_name##_class_t *klass);                             \
    static void type_name##_init(type_name##_t *self);                                          \
    static void type_name##_destroy(type_name##_t *self);                                       \
                                                                                                \
    oc_type_t *type_name##_get_type(void)                                                       \
    {                                                                                           \
        return oc_type_register(PARENT_TYPE(), #type_name, sizeof(type_name##_class_t),         \
                                (oc_class_init_t)type_name##_class_init, sizeof(type_name##_t), \
                                (oc_instance_init_t)type_name##_init,                           \
                                (oc_instance_destroy_t)type_name##_destroy, type_flags);        \
    }

#define OC_DECLARE_TYPE(type_name, TYPE_NAME, parent_name) \
    OC_DECLARE_TYPE_EXTENDED(type_name, TYPE_NAME, parent_name)

#define OC_DEFINE_TYPE(type_name, PARENT_TYPE) \
    OC_DEFINE_TYPE_EXTENDED(type_name, PARENT_TYPE, OC_TYPE_FLAG_OBJECT)

#define OC_DECLARE_ABSTRACT_TYPE(type_name, TYPE_NAME, parent_name) \
    OC_DECLARE_TYPE_EXTENDED(type_name, TYPE_NAME, parent_name)

#define OC_DEFINE_ABSTRACT_TYPE(type_name, PARENT_TYPE) \
    OC_DEFINE_TYPE_EXTENDED(type_name, PARENT_TYPE, OC_TYPE_FLAG_ABSTRACT | OC_TYPE_FLAG_OBJECT)

enum oc_type_flags {
    OC_TYPE_FLAG_FUNDAMENTAL = 1 << 0,
    OC_TYPE_FLAG_OBJECT = 1 << 1,
    OC_TYPE_FLAG_ABSTRACT = 1 << 2,
    OC_TYPE_FLAG_INTERFACE = 1 << 3,
    OC_TYPE_FLAG_OTHER = 1 << 4
};

struct oc_type {
    oc_type_t *parent;
    char *name;

    int klass_size;
    oc_class_init_t klass_init;
    oc_type_class_t *klass;

    int instance_size;
    oc_instance_init_t instance_init;
    oc_instance_destroy_t instance_destroy;

    oc_type_flags_t flags;
    volatile int ref_count;
};

struct oc_type_instance {
    oc_type_class_t *klass;
};

struct oc_type_class {
    oc_type_t *type;
    volatile int ref_count;
};

oc_type_t *oc_type_register(oc_type_t *parent, char *name, int klass_size,
                            oc_class_init_t klass_init, int instance_size,
                            oc_instance_init_t instance_init,
                            oc_instance_destroy_t instance_destroy, oc_type_flags_t type_flags);

oc_bool oc_type_check_derivation(oc_type_t *type, oc_type_t *parent);
void oc_type_ref(oc_type_t *type);

void oc_type_unref(oc_type_t *type);

void oc_type_class_ref(oc_type_class_t *klass);

void oc_type_class_unref(oc_type_class_t *klass);

oc_type_instance_t *oc_type_instance_new(oc_type_t *type);

oc_type_class_t *oc_type_class_new(oc_type_t *type);

oc_type_instance_t *oc_type_instance_cast(oc_type_instance_t *self, oc_type_t *type);

oc_type_class_t *oc_type_class_cast(oc_type_class_t *self, oc_type_t *type);

void oc_type_class_init_chainup(oc_type_t *type, oc_type_class_t *klass);

void oc_type_instance_init_chainup(oc_type_t *type, oc_type_instance_t *instance);

void oc_type_instance_destroy_chaindown(oc_type_t *type, oc_type_instance_t *instance);

void oc_type_instance_free(oc_type_instance_t *instance);
