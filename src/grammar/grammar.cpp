#include "grammar.h"
#include <map>
#include <format>
#include <iomanip>

GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index < input.size() && input.substr(index, node->root_string.size()) == node->root_string) {
        return {
            new GrammarCoreStr::ParsedTree(
                new ParsedNodeStr(node, index, index + node->root_string.size(), input) 
            )
        };
    }
    return {};
}

GrammarCoreStr::GrammarResult digit(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index < input.size() && std::isdigit(input[index])){
        return {
            new  GrammarCoreStr::ParsedTree(
                new ParsedNodeStr(node, index, index + 1, input)
            ) 
        };
    }
    return {};
}

GrammarCoreStr::GrammarResult end(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index >= input.size()){
        return {
            new GrammarCoreStr::ParsedTree( 
                new ParsedNodeStr(node, index, index, input)
            ) 
        };
    }
    return {};
}

std::map <GrammarCoreStr::GrammarFunction, std::string> func_name_map = {
    {assign<std::string>, "assign"},
    {add<std::string>, "add"},
    {alternative<std::string>, "alternative"},
    {attach<std::string>, "attach"},
    {st, "st"},
    {digit, "digit"},
    {end, "end"}
};


template<>
std::string ParsedNode<std::string>::to_string(int indent) const {
    std::string func_name;
    if (func_name_map.contains(this->node->func)) {
        func_name = func_name_map[this->node->func];
    } else {
        func_name = "unknown";
    }

    //return this->data;
    return std::format(
        "\n{0}{1}:{2} ({3})",
        std::string(indent, ' '),
        node->name,
        func_name,
        this->data.substr(this->start, this->end - this->start));
}

