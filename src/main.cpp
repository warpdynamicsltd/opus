#include <gtest/gtest.h>
#include "grammar.h"  // Include your header file

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


int main() {

    GrammarNodeStr number, start, sum, exp, x;
    GrammarNodeStr d(digit), e(end), lb(st, "("), rb(st, ")"), plus(st, "+"), minus(st, "-");

    // number.set_name("number");
    // start.set_name("start");
    // sum.set_name("sum");
    // exp.set_name("exp");
    // d.set_name("digit");
    // e.set_name("end");
    // lb.set_name("lb");
    // rb.set_name("rb");
    // plus.set_name("plus");

    number = d | (d + number) << transform_blank<std::string>;
    exp = number | (lb + sum + rb << t_bracket);
    sum = exp | (exp + ( plus|minus ) + sum << t_op);
    start = sum + e;

    //number << transform_blank;

    std::string s = "11+(22-33)+44";
    auto gr = start(s, 0)[0];
    std::cout << gr->to_string() << std::endl;

    auto gr1 = transform<std::string>(gr);
    std::cout << gr1->to_string() << std::endl;

    auto str = gr1->to_string();

    std::cout << remove_whitespaces(str) << std::endl;
    return 0;
}

