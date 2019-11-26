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

static char *some_str_base = "BB";

static int some_func_base(void)
{
    return 1;
}

static char *some_str_derived = "DD";

static int some_func_derived(void)
{
    return 2;
}

OC_DECLARE_TYPE(test_base, TEST_BASE, oc_object)

struct test_base {
    oc_object_t parent;

    char *some_str;
};

struct test_base_class {
    oc_object_class_t parent;

    int (*some_func)(void);
};

test_base_t *test_base_new(void);

OC_DEFINE_TYPE(test_base, OC_OBJECT_TYPE)

static void test_base_class_init(test_base_class_t *klass)
{
    klass->some_func = some_func_base;
}

static void test_base_init(test_base_t *self)
{
    self->some_str = malloc(3);
    strcpy(self->some_str, some_str_base);
}

static void test_base_destroy(test_base_t *self)
{
    free(self->some_str);
}

test_base_t *test_base_new(void)
{
    return oc_object_new(TEST_BASE_TYPE());
}

OC_DECLARE_TYPE(test_derived, TEST_DERIVED, test_base)

struct test_derived {
    test_base_t parent;
};

struct test_derived_class {
    test_base_class_t parent;
};

test_derived_t *test_derived_new(void);

OC_DEFINE_TYPE(test_derived, TEST_BASE_TYPE)

static void test_derived_class_init(test_derived_class_t *klass)
{
    test_base_class_t *base_klass = TEST_BASE_CLASS(klass);

    base_klass->some_func = some_func_derived;
}

static void test_derived_init(test_derived_t *self)
{
    test_base_t *base = TEST_BASE(self);

    strcpy(base->some_str, some_str_derived);
}

static void test_derived_destroy(test_derived_t *self)
{
}

test_derived_t *test_derived_new(void)
{
    return oc_object_new(TEST_DERIVED_TYPE());
}

int main(int argc, char **argv)
{
    test_base_t *base = test_base_new();
    test_base_class_t *base_klass = TEST_BASE_GET_CLASS(base);
    test_derived_t *derived = test_derived_new();
    test_derived_class_t *derived_klass = TEST_DERIVED_GET_CLASS(derived);
    test_base_class_t *derived_base_klass = TEST_BASE_CLASS(derived_klass);
    test_base_t *derived_base = TEST_BASE(derived);

    oc_assert(IS_TEST_BASE(base) == 1);
    oc_assert(IS_TEST_BASE(derived) == 1);
    oc_assert(IS_TEST_DERIVED(base) == 0);

    oc_assert(strcmp(base->some_str, some_str_base) == 0);
    oc_assert(strcmp(derived_base->some_str, some_str_derived) == 0);

    oc_assert(base_klass->some_func() == some_func_base());
    oc_assert(derived_base_klass->some_func() == some_func_derived());

    oc_object_free(base);
    oc_object_free(derived);

    return 0;
}
