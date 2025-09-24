#include "grammar.h"

GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index < input.size() && input.substr(index, node->root_string.size()) == node->root_string) {
        return {
            new GrammarCoreStr::ParsedNodeTerm(
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
            new  GrammarCoreStr::ParsedNodeTerm(
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
            new GrammarCoreStr::ParsedNodeTerm( 
                new ParsedNodeStr(node, index, index, input)
            ) 
        };
    }
    return {};
}

