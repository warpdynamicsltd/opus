#include "grammar.h"
#include <iostream>
#include <cctype>

// // Factory method for creating literal nodes (`st`)
// template <typename T>
// GrammarNode<T> st(char &value) {
//     return GrammarNode<std::string>::Factory([&value](const std::string& input, size_t index) {
//         if (index < input.size() && input[index] == value) {
//             return std::vector<std::tuple<size_t, GrammarTerm>>{
//                 {index + 1, GrammarTerm("st", {GrammarTerm(std::string(1, input[index]))})}};
//         }
//         return std::vector<std::tuple<size_t, GrammarTerm>>{};
//     });
// }

// // Simple utility functions for grammar node creation
// GrammarNode<std::string> digit() {
//     return GrammarNode<std::string>::Factory([](const std::string& input, size_t index) {
//         if (index < input.size() && std::isdigit(input[index])) {
//             return std::vector<std::tuple<size_t, GrammarTerm>>{
//                 {index + 1, GrammarTerm("digit", {GrammarTerm(std::string(1, input[index]))})}};
//         }
//         return std::vector<std::tuple<size_t, GrammarTerm>>{};
//     });
// }

// // Simple utility functions for grammar node creation
// GrammarNode<std::string> letter() {
//     return GrammarNode<std::string>::Factory([](const std::string& input, size_t index) {
//         if (index < input.size() && std::isalpha(input[index])) {
//             return std::vector<std::tuple<size_t, GrammarTerm>>{
//                 {index + 1, GrammarTerm("letter", {GrammarTerm(std::string(1, input[index]))})}};
//         }
//         return std::vector<std::tuple<size_t, GrammarTerm>>{};
//     });
// }

// GrammarNode<std::string> end() {
//     return GrammarNode<std::string>::Factory([](const std::string& input, size_t index) {
//         if (index >= input.size()) {
//             return std::vector<std::tuple<size_t, GrammarTerm>>{{index, GrammarTerm("end")}};
//         }
//         return std::vector<std::tuple<size_t, GrammarTerm>>{};
//     });
// }

// class A{
//     public:
//         std::function<int()> f;
//         std::function<int()> g;

//         void setup(const A &a) {
//             f = [this, a](){ return a.f(); };
//         }
// };

int main() {
    // auto a = A();
    // auto b = A();
    // b.f = [](){ return 1; };
    // a.setup(b);

    // b.f = [](){ return 2; };

    // std::cout << a.f() << "\n";




    // auto d = digit();
    // auto l = letter();
    // auto e = end();

    // GrammarNode<std::string> number("number");
    // GrammarNode<std::string> start("start");
    // GrammarNode<std::string> sum("sum");
    // GrammarNode<std::string> exp("exp");

    // char lb = '(';
    // char rb = ')';
    // char plus = '+';

    // number == (d | (d + number));// >> "composed_number";
    // //number == (d | (d + number) | (d + d + d + d));
    // exp = number | (st<std::string>(lb) + sum + st<std::string>(rb));
    // //exp = number | (st<std::string>("(") + number + st<std::string>(")"));
    
    // //number == (d | d + d | d + d + d | d + d + d + d | d + d + d + d + d | d + d + d + d + d + d);
    // //number == (d | d + d );
    // //number == (d | d + d | d + d + d);
    // //number == (d | d + d | d + d + d | d + d + d + d);
    // //number == (d | d + d | d + d + d | d + d + d + d | d + d + d + d + d);
    // //number == (d | (d + d) | (d + d + d) | (d + d + d + d) | (d + d + d + d + d));
    // //exp == (st<std::string>(lb) + number + st<std::string>(rb));
    
    
    // //exp = d + number + d;
    // //exp = number;
    // sum = exp | (exp + st<std::string>(plus) + sum);
    // start = (sum + e);
    // //start == ((d | d + d | d + d + d | d + d + d + d) + e);
    
    

    // //auto f = (d | d + d | d + d + d).func;
    // //auto result = ((d | d + d | d + d + d) + e).func("111", 0);
    // //auto result = f("11", 0);
    // //auto result = exp("(22)", 0);
    // //auto result = ((st<std::string>(lb) + (d | d + d | d + d + d | d + d + d + d) + st<std::string>(rb)))("(2222)", 0);
    // //auto node = (d | d + d | d + d + d | d + d + d + d | d + d + d + d + d | d + d + d + d + d + d);
    // //number == node;
    // auto result = start("123+((23+2)+3)", 0);
    // //auto result = ((st<std::string>(lb) + (number == (d | d + d | d + d + d | d + d + d + d | d + d + d + d + d | d + d + d + d + d + d)) + st<std::string>(rb)))("(222222)", 0);
    // //auto result = 
    // for (size_t i : result) {
    //     std::cout << "Matched up to index: " << i << "\n";
    // }
    return 0;
}

