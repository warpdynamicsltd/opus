#include "grammar.h"
#include <stdexcept>
#include <utility>

// Constructor
template <typename T>
GrammarNode<T>::GrammarNode()
{
    this->mode = Mode::ROOT;
    this->func2 = nullptr;
}

template <typename T>
GrammarNode<T>::GrammarNode(const GrammarFunction2& customFunc)
{
    this->mode = Mode::ROOT;
    this->func2 = customFunc;
}

template <typename T>
GrammarNode<T>::GrammarNode(const GrammarFunction2& customFunc, const std::string& root_string)
{
    this->mode = Mode::ROOT;
    this->func2 = customFunc;
    this->root_string = root_string;
}

template <typename T>
std::vector<size_t> GrammarNode<T>::_assign(const T& input, size_t index) const
{
    return this->left->_call(input, index);
}

template <typename T>
std::vector<size_t> GrammarNode<T>::_plus(const T& input, size_t index) const
{
    std::vector<size_t> result;
    for (size_t i : this->left->_call(input, index)) {
        for (size_t j : this->right->_call(input, i)) {
            
            result.push_back(j);
        }
    }
    return result;
}

template <typename T>
std::vector<size_t> GrammarNode<T>::_or(const T& input, size_t index) const
{
    auto result = this->left->_call(input, index);
    auto otherResult = this->right->_call(input, index);
    result.insert(result.end(), otherResult.begin(), otherResult.end());
    return result;
}

template <typename T>
std::vector<size_t> GrammarNode<T>::_call(const T& input, size_t index) const
{
    switch (mode)
    {
        case Mode::PLUS:
            return _plus(input, index);
        case Mode::OR:
            return _or(input, index);
        case Mode::ASSIGN:
            return _assign(input, index);
        
        default:
            return func2(this, input, index);
        
    }
}

template <typename T>
GrammarNode<T>* GrammarNode<T>::create_plus(const GrammarNode<T>* other) const
{
    GrammarNode<T> *res = new GrammarNode<T>();
    res->left = this;
    res->right = other;
    res->mode = Mode::PLUS;
    return res;
}

template <typename T>
GrammarNode<T>* GrammarNode<T>::create_or(const GrammarNode<T>* other) const
{
    GrammarNode<T> *res = new GrammarNode<T>();
    res->left = this;
    res->right = other;
    res->mode = Mode::OR;
    return res;
}

template <typename T>
void GrammarNode<T>::create_assignment(const GrammarNode<T>* other)
{
    this->left = other;
    this->mode = Mode::ASSIGN;
}


// Operator() (calls the grammar function)
template <typename T>
std::vector<size_t> GrammarNode<T>::operator()(const T& input, size_t index) const {
    
    return _call(input, index);
}

template class GrammarNode<std::string>;

template<typename T>
SmartGrammarNode<T> SmartGrammarNode<T>::operator+(const SmartGrammarNode<T>& other) const
{
    return SmartGrammarNode<T>(this->node->create_plus(other.node));
}

template<typename T>
SmartGrammarNode<T> SmartGrammarNode<T>::operator|(const SmartGrammarNode<T>& other) const
{
    return SmartGrammarNode<T>(this->node->create_or(other.node));
}

template<typename T>
SmartGrammarNode<T>& SmartGrammarNode<T>::operator=(const SmartGrammarNode<T>& other)
{
    this->node->create_assignment(other.node);
    return *this;
}

template<typename T>
std::vector<size_t> SmartGrammarNode<T>::operator()(const T& input, size_t index) const
{
    return this->node->_call(input, index);
}

template class SmartGrammarNode<std::string>;
