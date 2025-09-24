#include "grammar.h"
#include <stdexcept>
#include <utility>

template class GrammarCore<std::string>;
template class GrammarNode<std::string>;
template class ParsedNode<std::string>;
template class AbstractTerm<ParsedNode<std::string>*>;

// Constructor
template <typename T>
GrammarCore<T>::GrammarCore()
{
    this->func = nullptr;
}

template <typename T>
GrammarCore<T>::GrammarCore(const GrammarFunction& customFunc)
{
    this->func = customFunc;
}

template <typename T>
GrammarCore<T>::GrammarCore(const GrammarFunction& customFunc, const std::string& root_string)
{
    this->func = customFunc;
    this->root_string = root_string;
}

template <typename T>
typename GrammarCore<T>::GrammarResult assign(const GrammarCore<T>* that, const T& input, size_t index)
{
    return that->left->call(input, index);
}

template <typename T>
typename GrammarCore<T>::GrammarResult add(const GrammarCore<T>* that, const T& input, size_t index)
{
    typename GrammarCore<T>::GrammarResult result;
    for (auto n : that->left->call(input, index)) {
        for (auto m : that->right->call(input, n->value->end)) {
            result.push_back(
                new typename GrammarCore<T>::ParsedNodeTerm(
                    new ParsedNode<T>(
                        that,
                        index,
                        m->value->end,
                        input
                    ),
                    {n, m}
                )
            );
        }
    }
    return result;
}

template <typename T>
typename GrammarCore<T>::GrammarResult alternative(const GrammarCore<T>* that, const T& input, size_t index)
{
    auto result = that->left->call(input, index);
    auto otherResult = that->right->call(input, index);
    result.insert(result.end(), otherResult.begin(), otherResult.end());
    return result;
}

template <typename T>
typename GrammarCore<T>::GrammarResult GrammarCore<T>::call(const T& input, size_t index) const
{
    return func(this, input, index);
}

template <typename T>
GrammarCore<T>* GrammarCore<T>::create_plus(const GrammarCore<T>* other) const
{
    GrammarCore<T> *res = new GrammarCore<T>();
    res->left = this;
    res->right = other;
    res->func = add<T>;
    return res;
}

template <typename T>
GrammarCore<T>* GrammarCore<T>::create_or(const GrammarCore<T>* other) const
{
    GrammarCore<T> *res = new GrammarCore<T>();
    res->left = this;
    res->right = other;
    res->func = alternative<T>;
    return res;
}

template <typename T>
void GrammarCore<T>::create_assignment(const GrammarCore<T>* other)
{
    this->left = other;
    this->func = assign<T>;
}


template<typename T>
GrammarNode<T>::GrammarNode()
{
    this->node = new GrammarCore<T>();
}


template<typename T>
GrammarNode<T> GrammarNode<T>::operator+(const GrammarNode<T>& other) const
{
    return GrammarNode<T>(this->node->create_plus(other.node));
}

template<typename T>
GrammarNode<T> GrammarNode<T>::operator|(const GrammarNode<T>& other) const
{
    return GrammarNode<T>(this->node->create_or(other.node));
}

template<typename T>
GrammarNode<T>& GrammarNode<T>::operator=(const GrammarNode<T>& other)
{
    this->node->create_assignment(other.node);
    return *this;
}

template<typename T>
typename GrammarCore<T>::GrammarResult GrammarNode<T>::operator()(const T& input, size_t index) const
{
    return this->node->call(input, index);
}


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