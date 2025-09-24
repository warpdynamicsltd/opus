#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <memory>
#include <tuple>

#include "abstract_term.h"
#include "memory.h"

template <typename T>
class ParsedNode;  // Forward declaration

template <typename T = std::string>
class GrammarCore : public MassDealocator<GrammarCore<T>>
{
    public:
        typedef AbstractTerm<ParsedNode<T>*> ParsedNodeTerm; 
        typedef std::vector<ParsedNodeTerm*> GrammarResult;
        typedef GrammarResult(*GrammarFunction)(const GrammarCore<T>*, const T&, size_t);

    public:
        GrammarFunction func = nullptr;
        
        const GrammarCore<T> *left = nullptr;
        const GrammarCore<T> *right = nullptr;

        std::string root_string;

    public:
        GrammarCore();
        GrammarCore(const GrammarFunction&);
        GrammarCore(const GrammarFunction&, const std::string& root_string);

        GrammarResult call(const T&, size_t) const;

        void create_assignment(const GrammarCore<T>* other);
        GrammarCore<T>* create_plus(const GrammarCore<T>* other) const;
        GrammarCore<T>* create_or(const GrammarCore<T>* other) const;

};

template <typename T>
typename GrammarCore<T>::GrammarResult assign(const GrammarCore<T>*, const T&, size_t);

template <typename T>
typename GrammarCore<T>::GrammarResult add(const GrammarCore<T>*, const T&, size_t);

template <typename T>
typename GrammarCore<T>::GrammarResult alternative(const GrammarCore<T>*, const T&, size_t);

template <typename T>
class GrammarNode : public MassDealocator<GrammarNode<T>>
{
    public:
        GrammarNode();
        GrammarNode(GrammarCore<T>* node) : node(node) {}
        GrammarNode(const typename GrammarCore<T>::GrammarFunction& func)
        : node(new GrammarCore<T>(func)) {}
        GrammarNode(const typename GrammarCore<T>::GrammarFunction& func, const std::string& root_string) 
        : node(new GrammarCore<T>(func, root_string)) {}

        GrammarCore<T> *node;

        GrammarNode<T> operator+(const GrammarNode<T>& other) const;
        GrammarNode<T> operator|(const GrammarNode<T>& other) const;

        typename GrammarCore<T>::GrammarResult operator()(const T& input, size_t index) const;

        GrammarNode<T>& operator=(const GrammarNode<T>& other);
};

template <typename T>
class ParsedNode : public MassDealocator<ParsedNode<T>>
{
    public:
        ParsedNode(
            const GrammarCore<T>* node, 
            size_t start, 
            size_t end, 
            const T& data):
                node(node),  
                start(start), 
                end(end), 
                data(data) {}
    public:
        const GrammarCore<T> *node = nullptr;

        size_t start = 0;
        size_t end = 0;
        const T& data;
};

typedef GrammarNode<std::string> GrammarNodeStr;
typedef GrammarCore<std::string> GrammarCoreStr;
typedef ParsedNode<std::string> ParsedNodeStr;


GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult digit(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult end(const GrammarCoreStr* node, const std::string& input, size_t index);

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


#endif // GRAMMAR_H
