#include "grammar.h"
#include <stdexcept>
#include <utility>

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
std::vector<size_t> assign(const GrammarCore<T>* that, const T& input, size_t index)
{
    return that->left->call(input, index);
}

template <typename T>
std::vector<size_t> add(const GrammarCore<T>* that, const T& input, size_t index)
{
    std::vector<size_t> result;
    for (size_t i : that->left->call(input, index)) {
        for (size_t j : that->right->call(input, i)) {
            
            result.push_back(j);
        }
    }
    return result;
}

template <typename T>
std::vector<size_t> alternative(const GrammarCore<T>* that, const T& input, size_t index)
{
    auto result = that->left->call(input, index);
    auto otherResult = that->right->call(input, index);
    result.insert(result.end(), otherResult.begin(), otherResult.end());
    return result;
}

template <typename T>
std::vector<size_t> GrammarCore<T>::call(const T& input, size_t index) const
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

template class GrammarCore<std::string>;


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
std::vector<size_t> GrammarNode<T>::operator()(const T& input, size_t index) const
{
    return this->node->call(input, index);
}

template class GrammarNode<std::string>;

std::vector<size_t> st(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index < input.size() && input.substr(index, node->root_string.size()) == node->root_string) {
        return { index + node->root_string.size() };
    }
    return {};
}

std::vector<size_t> digit(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index < input.size() && std::isdigit(input[index])){
        return { index + 1 };
    }
    return {};
}

std::vector<size_t> end(const GrammarCoreStr* node, const std::string& input, size_t index)
{
    if (index >= input.size()){
        return { index };
    }
    return {};
}
