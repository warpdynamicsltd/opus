#include <gtest/gtest.h>
#include "grammar/grammar.h"  // Include your header file


#define TEST_NODE_SUCCESS(node, input) ASSERT_TRUE(node(input, 0)[0]->end == std::string(input).size())
#define TEST_NODE_FAILURE(node, input) ASSERT_TRUE(node(input, 0).empty())

TEST(GrammarAddTest, SimpleTestCase) {
    GrammarNode<std::string> number, start, sum, exp;
    GrammarNode<std::string> d(digit), e(end), lb(st, "("), rb(st, ")"), plus(st, "+");

    number = d | d + number;
    exp = number | lb + sum + rb;
    sum = exp | exp + plus + sum;
    start = sum + e;
    
    // Valid cases
    TEST_NODE_SUCCESS(start, "1");
    TEST_NODE_SUCCESS(start, "(1)");
    TEST_NODE_SUCCESS(start, "12");
    TEST_NODE_SUCCESS(start, "(12)");
    TEST_NODE_SUCCESS(start, "1+2");
    TEST_NODE_SUCCESS(start, "12+34");
    TEST_NODE_SUCCESS(start, "1+2+3");
    TEST_NODE_SUCCESS(start, "(1+2)");
    TEST_NODE_SUCCESS(start, "((1+2))");
    TEST_NODE_SUCCESS(start, "1+(2)");
    TEST_NODE_SUCCESS(start, "1+(2+3)");
    TEST_NODE_SUCCESS(start, "(1+(2+3))");

    // Multi-digit numbers
    TEST_NODE_SUCCESS(start, "123");
    TEST_NODE_SUCCESS(start, "123456789");
    TEST_NODE_SUCCESS(start, "123456789123456789123456789123456789");
    TEST_NODE_SUCCESS(start, "(123)");
    TEST_NODE_SUCCESS(start, "(123456789)");
    TEST_NODE_SUCCESS(start, "(123456789123456789123456789123456789)");
    TEST_NODE_SUCCESS(start, "123+456");
    TEST_NODE_SUCCESS(start, "123126751276141294+4562634871278524396587263726193712654");
    TEST_NODE_SUCCESS(start, "(123+456)");
    TEST_NODE_SUCCESS(start, "123+(456+789)");
    TEST_NODE_SUCCESS(start, "((123)+(456+789))");
    TEST_NODE_SUCCESS(start, "123+456+789");
    TEST_NODE_SUCCESS(start, "((123+456)+789)");
    TEST_NODE_SUCCESS(start, "(123+(456+789))+1011");
    TEST_NODE_SUCCESS(start, "1+(23+(456+789))");
    TEST_NODE_SUCCESS(start, "(((123+456)+789)+1011)");

    // Nested parentheses
    TEST_NODE_SUCCESS(start, "((1+2)+3)");
    TEST_NODE_SUCCESS(start, "((1+2)+(3+4))");
    TEST_NODE_SUCCESS(start, "(((1+2)+3)+(4+5))");
    TEST_NODE_SUCCESS(start, "(1+(2+(3+(4+5))))");
    TEST_NODE_SUCCESS(start, "1+(2+(3+(4+(5+6))))");
    TEST_NODE_SUCCESS(start, "(((1+2)+3+4)+5)+6");
    TEST_NODE_SUCCESS(start, "((1+(2+3+4)))+5");
    TEST_NODE_SUCCESS(start, "((1+2)+(3+(4+5))+(6+7))");
    TEST_NODE_SUCCESS(start, "(1+(2+(3+(4+5+6+7+8))))");
    TEST_NODE_SUCCESS(start, "(1+(2+3))+((4+5)+(6+7))");

    // Chained additions
    TEST_NODE_SUCCESS(start, "1+2+3+4");
    TEST_NODE_SUCCESS(start, "1+2+3+4+5");
    TEST_NODE_SUCCESS(start, "11+22+33+44+55");
    TEST_NODE_SUCCESS(start, "1+(2+(3+4+5+6))");
    TEST_NODE_SUCCESS(start, "(((1+2)+3)+4)+5");
    TEST_NODE_SUCCESS(start, "(1+(2+3+(4+5)))");
    TEST_NODE_SUCCESS(start, "(1+2)+(3+4)+(5+6)");
    TEST_NODE_SUCCESS(start, "1+(2+(3+(4)))");
    TEST_NODE_SUCCESS(start, "((1+2)+3)+(4+(5+6+7))");
    TEST_NODE_SUCCESS(start, "1+(23+45)+67");

    // Random valid structures
    TEST_NODE_SUCCESS(start, "((1+2))+3");
    TEST_NODE_SUCCESS(start, "(((1+23))+45)");
    TEST_NODE_SUCCESS(start, "((123)+456+789)");
    TEST_NODE_SUCCESS(start, "1+((23+45)+6)");
    TEST_NODE_SUCCESS(start, "(((1+2345)+(678)))+9");
    TEST_NODE_SUCCESS(start, "1+(2+(3+(4+(9+10))))");
    TEST_NODE_SUCCESS(start, "123+456");
    TEST_NODE_SUCCESS(start, "(1+2)+(3+4)");
    TEST_NODE_SUCCESS(start, "(1+(2+3)+(4+5))");
    TEST_NODE_SUCCESS(start, "((1+2)+((3+4+5)+6))");



        // Invalid cases
    // Missing operands
    TEST_NODE_FAILURE(start, "1+");
    TEST_NODE_FAILURE(start, "+1");
    TEST_NODE_FAILURE(start, "123+");
    TEST_NODE_FAILURE(start, "+12345");
    TEST_NODE_FAILURE(start, "(+)");

    // Incorrectly nested parentheses
    TEST_NODE_FAILURE(start, "(1");
    TEST_NODE_FAILURE(start, "1)");
    TEST_NODE_FAILURE(start, "(123");
    TEST_NODE_FAILURE(start, "123)");
    TEST_NODE_FAILURE(start, "(123456789");
    TEST_NODE_FAILURE(start, "123456789)");
    TEST_NODE_FAILURE(start, "(1+2");
    TEST_NODE_FAILURE(start, "1+2)");
    TEST_NODE_FAILURE(start, "(1+(2+3)");
    TEST_NODE_FAILURE(start, ")1+2(");
    TEST_NODE_FAILURE(start, "((1+2)");
    TEST_NODE_FAILURE(start, "(1+2))");
    TEST_NODE_FAILURE(start, "(())");
    TEST_NODE_FAILURE(start, "(()");
    TEST_NODE_FAILURE(start, ")");
    TEST_NODE_FAILURE(start, "(");

    // Multiple consecutive operators
    TEST_NODE_FAILURE(start, "1++2");
    TEST_NODE_FAILURE(start, "1+++2");
    TEST_NODE_FAILURE(start, "1+2+3++");
    TEST_NODE_FAILURE(start, "++1+2");
    TEST_NODE_FAILURE(start, "1+2+3--");
    TEST_NODE_FAILURE(start, "1+-2");
    TEST_NODE_FAILURE(start, "--1");
    TEST_NODE_FAILURE(start, "1**2");
    TEST_NODE_FAILURE(start, "**2+3");
    TEST_NODE_FAILURE(start, "1+**3");

    // Spaces between operands or operators
    TEST_NODE_FAILURE(start, "1 + 2");
    TEST_NODE_FAILURE(start, "( 1+2 )");
    TEST_NODE_FAILURE(start, "1 +2");
    TEST_NODE_FAILURE(start, "1+ 2");
    TEST_NODE_FAILURE(start, "12 +34");
    TEST_NODE_FAILURE(start, "12 + 34");
    TEST_NODE_FAILURE(start, "( 1 + 2)");
    TEST_NODE_FAILURE(start, "1+2 +3");
    TEST_NODE_FAILURE(start, "1 + 2+3");
    TEST_NODE_FAILURE(start, "1 +2+ 3");

    // Empty input or incomplete input
    TEST_NODE_FAILURE(start, "");
    TEST_NODE_FAILURE(start, " ");
    TEST_NODE_FAILURE(start, "( )");
    TEST_NODE_FAILURE(start, "()");
    TEST_NODE_FAILURE(start, "(1");
    TEST_NODE_FAILURE(start, "1)");
    TEST_NODE_FAILURE(start, ")1(");
    TEST_NODE_FAILURE(start, "1+2(");
    TEST_NODE_FAILURE(start, ")1+2");
    TEST_NODE_FAILURE(start, "1+(");
    TEST_NODE_FAILURE(start, "(+)");

    // Invalid characters
    TEST_NODE_FAILURE(start, "1+a");
    TEST_NODE_FAILURE(start, "(1%b)");
    TEST_NODE_FAILURE(start, "1#2");
    TEST_NODE_FAILURE(start, "12_34");
    TEST_NODE_FAILURE(start, "1+2&3");
    TEST_NODE_FAILURE(start, "1@2");
    TEST_NODE_FAILURE(start, "(12 34)");
    TEST_NODE_FAILURE(start, "(12!34)");
    TEST_NODE_FAILURE(start, "1$2");
    TEST_NODE_FAILURE(start, "(1+2)$");


}


TEST(GrammarMulTest, MultiplicationTestCase) {
    GrammarNode<std::string> number, start, sum, exp, mul;
    GrammarNode<std::string> d(digit), e(end), lb(st, "("), rb(st, ")"), plus(st, "+"), times(st, "*");

    number = d | d + number;
    exp = number | lb + sum + rb;
    mul = exp | exp + times + mul;     // multiplication is now available
    sum = mul | mul + plus + sum;      // sum is now mul (+ sum)
    start = sum + e;

    // -- Valid cases --
    TEST_NODE_SUCCESS(start, "1");
    TEST_NODE_SUCCESS(start, "1*2");
    TEST_NODE_SUCCESS(start, "12*34");
    TEST_NODE_SUCCESS(start, "(1*2)");
    TEST_NODE_SUCCESS(start, "1+2*3");
    TEST_NODE_SUCCESS(start, "(1+2)*3");
    TEST_NODE_SUCCESS(start, "1*(2+3)");
    TEST_NODE_SUCCESS(start, "1*2+3");
    TEST_NODE_SUCCESS(start, "(1*2)+(3*4)");
    TEST_NODE_SUCCESS(start, "1*2*3");
    TEST_NODE_SUCCESS(start, "(1+2)*(3+4)");
    TEST_NODE_SUCCESS(start, "(1*2+3)");
    TEST_NODE_SUCCESS(start, "1+(2*3)");
    TEST_NODE_SUCCESS(start, "12*34+56*78");
    TEST_NODE_SUCCESS(start, "1+2*3+4*5");
    TEST_NODE_SUCCESS(start, "(1+2*3)+(4*5+6)");
    TEST_NODE_SUCCESS(start, "12*34");
    TEST_NODE_SUCCESS(start, "1*2*3");
    TEST_NODE_SUCCESS(start, "(1*2)*3");
    TEST_NODE_SUCCESS(start, "2*(3*4)");
    TEST_NODE_SUCCESS(start, "5*5*5*5");
    TEST_NODE_SUCCESS(start, "(1*1)*(2*2)");
    TEST_NODE_SUCCESS(start, "100*1000");
    TEST_NODE_SUCCESS(start, "7*8+9*10");
    TEST_NODE_SUCCESS(start, "(3*4)+(5*6)");
    TEST_NODE_SUCCESS(start, "9+(10*11)");
    TEST_NODE_SUCCESS(start, "1+2*3+4");
    TEST_NODE_SUCCESS(start, "7*8*9*10");
    TEST_NODE_SUCCESS(start, "(7*8)+(9*10)");
    TEST_NODE_SUCCESS(start, "(2+3)*4");
    TEST_NODE_SUCCESS(start, "2*(4+5)");
    TEST_NODE_SUCCESS(start, "2*2*2*2*2");
    TEST_NODE_SUCCESS(start, "(1+2)*(3*4)");
    TEST_NODE_SUCCESS(start, "1*(2+3)");
    TEST_NODE_SUCCESS(start, "(1*2)+(3*4)");
    TEST_NODE_SUCCESS(start, "1*2*3*4*5*6");
    TEST_NODE_SUCCESS(start, "7*8*9");
    TEST_NODE_SUCCESS(start, "9*8+7*6");
    TEST_NODE_SUCCESS(start, "123*456");
    TEST_NODE_SUCCESS(start, "(10*20)*30");
    TEST_NODE_SUCCESS(start, "10*20*30*40");
    TEST_NODE_SUCCESS(start, "(2*2)+(3*3)");
    TEST_NODE_SUCCESS(start, "1+2+3*4+5+6");
    TEST_NODE_SUCCESS(start, "(12*34+56)");
    TEST_NODE_SUCCESS(start, "1*2+4*8+16");
    TEST_NODE_SUCCESS(start, "(1*2)+(3*4)+(5*6)");
    TEST_NODE_SUCCESS(start, "100*200*300");
    TEST_NODE_SUCCESS(start, "11*22+33*44");
    TEST_NODE_SUCCESS(start, "(1*2+3)+4");
    TEST_NODE_SUCCESS(start, "5*6*7*8");
    TEST_NODE_SUCCESS(start, "1*(2*3*4)");
    TEST_NODE_SUCCESS(start, "12+(34*56)");
    TEST_NODE_SUCCESS(start, "(10*20)+30");
    TEST_NODE_SUCCESS(start, "7+(8*9)");
    TEST_NODE_SUCCESS(start, "(1*2)*(3*4)");
    TEST_NODE_SUCCESS(start, "1234*5678");
    TEST_NODE_SUCCESS(start, "2*3*4*5*6*7*8");
    TEST_NODE_SUCCESS(start, "(1+2*3)");
    TEST_NODE_SUCCESS(start, "4*5*6*7");
    TEST_NODE_SUCCESS(start, "10+20*30");
    TEST_NODE_SUCCESS(start, "(3*4)+(5*6)+7");
    TEST_NODE_SUCCESS(start, "2*2*2*2*2*2*2");
    TEST_NODE_SUCCESS(start, "1*2+3*4+5*6");
    TEST_NODE_SUCCESS(start, "99*98+97*96");

    // -- Invalid cases (mirror your style, expect failures) --
    TEST_NODE_FAILURE(start, "1**2");
    TEST_NODE_FAILURE(start, "1+*2");
    TEST_NODE_FAILURE(start, "*1+2");
    TEST_NODE_FAILURE(start, "1*");
    TEST_NODE_FAILURE(start, "*");
    TEST_NODE_FAILURE(start, "+*");
    TEST_NODE_FAILURE(start, "(1*)");
    TEST_NODE_FAILURE(start, "(1+2*)");
    TEST_NODE_FAILURE(start, "((1*2))*)");
    TEST_NODE_FAILURE(start, "(*)");
    TEST_NODE_FAILURE(start, "*()");
    TEST_NODE_FAILURE(start, "()*1");
    TEST_NODE_FAILURE(start, "1+2+*3");
    TEST_NODE_FAILURE(start, "1*2++3");
    TEST_NODE_FAILURE(start, "1+2**3");
    TEST_NODE_FAILURE(start, "1 * 2");     // spaces
    TEST_NODE_FAILURE(start, "1*2 +3");
    TEST_NODE_FAILURE(start, "1+ 2*3");
    TEST_NODE_FAILURE(start, "");          // empty
    TEST_NODE_FAILURE(start, "1**2");
    TEST_NODE_FAILURE(start, "1*");
    TEST_NODE_FAILURE(start, "*1");
    TEST_NODE_FAILURE(start, "*");
    TEST_NODE_FAILURE(start, "1***2");
    TEST_NODE_FAILURE(start, "1*2*");
    TEST_NODE_FAILURE(start, "1*2+*3");
    TEST_NODE_FAILURE(start, "1+*2*3");
    TEST_NODE_FAILURE(start, "1*+2");
    TEST_NODE_FAILURE(start, "+1*2");
    TEST_NODE_FAILURE(start, "");
    TEST_NODE_FAILURE(start, "*1+2");
    TEST_NODE_FAILURE(start, "(1*2))*3");
    TEST_NODE_FAILURE(start, "((1*2)");
    TEST_NODE_FAILURE(start, "(1*)2");
    TEST_NODE_FAILURE(start, "(*)");
    TEST_NODE_FAILURE(start, "1 * 2");
    TEST_NODE_FAILURE(start, "1* 2");
    TEST_NODE_FAILURE(start, "1 *2");
    TEST_NODE_FAILURE(start, "( 1*2 )");
    TEST_NODE_FAILURE(start, "1*a");
    TEST_NODE_FAILURE(start, "a*1");
    TEST_NODE_FAILURE(start, "1#2");
    TEST_NODE_FAILURE(start, "()*1");
    TEST_NODE_FAILURE(start, "(1*b)");
    TEST_NODE_FAILURE(start, "(1*2)$");
    TEST_NODE_FAILURE(start, "1*2*");
    TEST_NODE_FAILURE(start, "1+2**3");
    TEST_NODE_FAILURE(start, "**2+3");
    TEST_NODE_FAILURE(start, "***");
    TEST_NODE_FAILURE(start, "1*2+3*");
    TEST_NODE_FAILURE(start, "1&2*3");
    TEST_NODE_FAILURE(start, "1;2*3");
    TEST_NODE_FAILURE(start, "(1*2)+(3*)");
    TEST_NODE_FAILURE(start, "1**2+3");
    TEST_NODE_FAILURE(start, "1*2*3*");
    TEST_NODE_FAILURE(start, "*1*2*3");
    TEST_NODE_FAILURE(start, "()*");
    TEST_NODE_FAILURE(start, "*()");
    TEST_NODE_FAILURE(start, "()");
    TEST_NODE_FAILURE(start, "1+*2");
    TEST_NODE_FAILURE(start, "(1*2*3))");
    TEST_NODE_FAILURE(start, "1*2 3");
    TEST_NODE_FAILURE(start, "1 * 2*3");
    TEST_NODE_FAILURE(start, "1*2 *3");
    TEST_NODE_FAILURE(start, "..1*2");
    TEST_NODE_FAILURE(start, "1*.2");
    TEST_NODE_FAILURE(start, "1*2.");
    TEST_NODE_FAILURE(start, "(1*2*3");

    // -- Invalid characters --
    TEST_NODE_FAILURE(start, "1*a");
    TEST_NODE_FAILURE(start, "(1*b)");
    TEST_NODE_FAILURE(start, "1#2");
    TEST_NODE_FAILURE(start, "1*2&3");
    TEST_NODE_FAILURE(start, "(1*2)$");
}