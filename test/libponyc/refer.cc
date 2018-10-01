#include <gtest/gtest.h>
#include <platform.h>

#include "util.h"


#define TEST_COMPILE(src) DO(test_compile(src, "refer"))

#define TEST_ERRORS_1(src, err1) \
  { const char* errs[] = {err1, NULL}; \
    DO(test_expected_errors(src, "refer", errs)); }

#define TEST_ERRORS_2(src, err1, err2) \
  { const char* errs[] = {err1, err2, NULL}; \
    DO(test_expected_errors(src, "refer", errs)); }

#define TEST_ERRORS_3(src, err1, err2, err3) \
  { const char* errs[] = {err1, err2, err3, NULL}; \
    DO(test_expected_errors(src, "refer", errs)); }


class ReferTest : public PassTest
{};


TEST_F(ReferTest, WhileLoopConsumeLet)
{
  const char* src =
    "class iso C\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "let c = C\n"
        "while true do\n"
          "P(consume c)\n"
        "end";

  TEST_ERRORS_1(src,
    "identifier 'c' is consumed when the loop exits");
}

TEST_F(ReferTest, WhileLoopConsumeLetInCondition)
{
  const char* src =
    "class iso C\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "let c = C\n"
        "while P(consume c); true do\n"
          "None\n"
        "end";

  TEST_ERRORS_1(src,
    "can't consume from an outer scope in a loop condition");
}

TEST_F(ReferTest, WhileLoopDeclareLetAndUseOutside)
{
  const char* src =
    "class val C\n"
      "new val create() => None\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "while let c1 = C; true do\n"
          "let c2 = C\n"
        "else\n"
          "let c3 = C\n"
        "end\n"

        "P(c1)\n"
        "P(c2)\n"
        "P(c3)";

  TEST_ERRORS_3(src,
    "can't find declaration of 'c1'",
    "can't find declaration of 'c2'",
    "can't find declaration of 'c3'");
}

TEST_F(ReferTest, RepeatLoopConsumeLet)
{
  const char* src =
    "class iso C\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "let c = C\n"
        "repeat\n"
          "P(consume c)\n"
        "until false end";

  TEST_ERRORS_1(src,
    "identifier 'c' is consumed when the loop exits");
}

TEST_F(ReferTest, RepeatLoopConsumeLetInCondition)
{
  const char* src =
    "class iso C\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "let c = C\n"
        "repeat\n"
          "None\n"
        "until\n"
          "P(consume c); false\n"
        "end";

  TEST_ERRORS_1(src,
    "can't consume from an outer scope in a loop condition");
}

TEST_F(ReferTest, RepeatLoopDeclareLetAndUseOutside)
{
  const char* src =
    "class val C\n"
      "new val create() => None\n"

    "primitive P\n"
      "fun apply(c: C) => None\n"

    "actor Main\n"
      "new create(env: Env) =>\n"
        "repeat\n"
          "let c1 = C\n"
        "until\n"
          "let c2 = C; true\n"
        "else\n"
          "let c3 = C\n"
        "end\n"

        "P(c1)\n"
        "P(c2)\n"
        "P(c3)";

  TEST_ERRORS_3(src,
    "can't find declaration of 'c1'",
    "can't find declaration of 'c2'",
    "can't find declaration of 'c3'");
}

TEST_F(ReferTest, RepeatLoopDefineVarAndUseOutside)
{
  const char* src =
    "actor Main\n"
      "new create(env: Env) =>\n"
        "var n: U8\n"
        "repeat\n"
          "n = 5\n"
        "until true end\n"
        "let n' = n";

  TEST_COMPILE(src);
}

TEST_F(ReferTest, RepeatLoopDefineVarAfterBreakAndUseOutside)
{
  const char* src =
    "actor Main\n"
      "new create(env: Env) =>\n"
        "var n: U8\n"
        "repeat\n"
          "if true then break end\n"
          "n = 5\n"
        "until true end\n"
        "let n' = n";

  TEST_ERRORS_1(src,
    "can't use an undefined variable in an expression");
}

TEST_F(ReferTest, RepeatLoopDefineVarAfterNestedBreakAndUseOutside)
{
  const char* src =
    "actor Main\n"
      "new create(env: Env) =>\n"
        "var n: U8\n"
        "repeat\n"
          "while true do break end\n"
          "n = 5\n"
        "until true end\n"
        "let n' = n";

  TEST_COMPILE(src);
}

TEST_F(ReferTest, RepeatLoopDefineVarAndUseInCondition)
{
  // From issue #2784
  const char* src =
    "actor Main\n"
      "new create(env: Env) =>\n"
        "var n: U8\n"
        "repeat\n"
          "n = 5\n"
        "until (n > 2) end";

  TEST_COMPILE(src);
}
