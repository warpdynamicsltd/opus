#include <gtest/gtest.h>
#include "grammar.h"  // Include your header file

GrammarCoreStr::ParsedTree* t_plus(const typename GrammarCoreStr::ParsedTree* a) {
    return new GrammarCoreStr::ParsedTree(a->args[1]->value, {a->args[0], a->args[2]});
}

int main() {

    GrammarNodeStr number, start, sum, exp;
    GrammarNodeStr d(digit), e(end), lb(st, "("), rb(st, ")"), plus(st, "+");

    number = d | d + number << transform_blank<std::string>;
    exp = number | lb + sum + rb;
    sum = (exp | exp + plus + sum << t_plus);
    start = sum + e;

    //number << transform_blank;

    std::string s = "11+22+33";
    auto gr = start(s, 0)[0];
    std::cout << gr->to_string() << std::endl;

    auto gr1 = transform<std::string>(gr);
    std::cout << gr1->to_string() << std::endl;
    return 0;
}

