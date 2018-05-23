#include <gtest/gtest.h>
#include <platform.h>

#include <ast/ast.h>
#include <pass/names.h>

#include "util.h"

#define TEST_ERROR(src) DO(test_error(src, "flatten"))


class FlattenTest: public PassTest
{};

/*
static void test(const char* before, const char* after, token_id start_id)
{
  test_good_pass(before, after, start_id, pass_flatten);
}


TEST(FlattenTest, Union)
{
  const char* before =
    "(uniontype"
    "  (nominal x (id A) x ref x)"
    "  (uniontype"
    "    (nominal x (id B) x iso x)"
    "    (nominal x (id C) x tag x))"
    "  (uniontype"
    "    (nominal x (id D) x trn x)"
    "    (nominal x (id E) x box x)))";

  const char* after =
    "(uniontype"
    "  (nominal x (id A) x ref x)"
    "  (nominal x (id B) x iso x)"
    "  (nominal x (id C) x tag x)"
    "  (nominal x (id D) x trn x)"
    "  (nominal x (id E) x box x))";

  ASSERT_NO_FATAL_FAILURE(test(before, after, TK_UNIONTYPE));
}
*/


TEST_F(FlattenTest, TypeparamCap)
{
  const char* src =
    "class C\n"
    "  fun foo[A]() =>\n"
    "    let a: A ref";

  TEST_ERROR(src);
}
