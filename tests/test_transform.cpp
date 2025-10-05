#include <gtest/gtest.h>
#include "grammar.h"  // Include your header file//

GrammarCoreStr::ParsedTree* t_op(const typename GrammarCoreStr::ParsedTree* a) {
    return new GrammarCoreStr::ParsedTree(a->args[0]->args[1]->value, {a->args[0]->args[0], a->args[0]->args[2]});
}

GrammarCoreStr::ParsedTree* t_bracket(const typename GrammarCoreStr::ParsedTree* a) {
    return new GrammarCoreStr::ParsedTree(a->args[0]->value, {a->args[0]->args[1]});
}

std::string remove_whitespaces(const std::string& str)
{
    std::string result;
    std::copy_if(str.begin(), str.end(), std::back_inserter(result),
        [](unsigned char c) { return !std::isspace(c); });
    return result;
}

#define TEST_NODE_SUCCESS(node, input, check) ASSERT_EQ(remove_whitespaces(transform<std::string>(node(input, 0)[0])->to_string()), check)

TEST(GrammarTransformAddTest, SimpleTestCase) {
    GrammarNodeStr number, start, sum, exp, x;
    GrammarNodeStr d(digit), e(end), lb(st, "("), rb(st, ")"), plus(st, "+"), minus(st, "-");

    number = d | (d + number) << transform_blank<std::string>;
    exp = number | (lb + sum + rb << t_bracket);
    sum = exp | (exp + ( plus|minus ) + sum << t_op);
    start = sum + e;

    TEST_NODE_SUCCESS(start, "11+(22-33)+44", ":add(11+(22-33)+44){:st(+){:attach(11),:st(+){:st(-){:attach(22),:attach(33)},:attach(44)}},:end()}");
    TEST_NODE_SUCCESS(start, "1+2", ":add(1+2){:st(+){:digit(1),:digit(2)},:end()}");
    TEST_NODE_SUCCESS(start, "5+10", ":add(5+10){:st(+){:digit(5),:attach(10)},:end()}");
    TEST_NODE_SUCCESS(start, "100-50", ":add(100-50){:st(-){:attach(100),:attach(50)},:end()}");
    TEST_NODE_SUCCESS(start, "(3+4)+5", ":add((3+4)+5){:st(+){:st(+){:digit(3),:digit(4)},:digit(5)},:end()}");
    TEST_NODE_SUCCESS(start, "7-(8+9)", ":add(7-(8+9)){:st(-){:digit(7),:st(+){:digit(8),:digit(9)}},:end()}");
    TEST_NODE_SUCCESS(start, "15+20+25", ":add(15+20+25){:st(+){:attach(15),:st(+){:attach(20),:attach(25)}},:end()}");
    TEST_NODE_SUCCESS(start, "(10)", ":add((10)){:attach(10),:end()}");
    TEST_NODE_SUCCESS(start, "99-88-77", ":add(99-88-77){:st(-){:attach(99),:st(-){:attach(88),:attach(77)}},:end()}");
    TEST_NODE_SUCCESS(start, "1+(2+(3+4))", ":add(1+(2+(3+4))){:st(+){:digit(1),:st(+){:digit(2),:st(+){:digit(3),:digit(4)}}},:end()}");
    TEST_NODE_SUCCESS(start, "(1+2)-(3+4)", ":add((1+2)-(3+4)){:st(-){:st(+){:digit(1),:digit(2)},:st(+){:digit(3),:digit(4)}},:end()}");
    TEST_NODE_SUCCESS(start, "6", ":add(6){:digit(6),:end()}");
}