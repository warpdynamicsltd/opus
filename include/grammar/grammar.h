#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <functional>
#include <vector>
#include <set>
#include <memory>
#include <tuple>
#include <format>


#include "abstract_term.h"
#include "memory.h"
#include "_grammar_declare.h"


/* BEGIN OF TEMPLATE IMPLEMENTATION */

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
    // typename GrammarCore<T>::GrammarResult result;
    // for (auto n : that->left->call(input, index)) {
    //     result.push_back(
    //         new GrammarCore<T>::ParsedTree(
    //             new ParsedNode<T>(
    //                         that,
    //                         index,
    //                         n->value->end,
    //                         input), {n}));
    // }
    // return result;
}

template <typename T>
typename GrammarCore<T>::GrammarResult add(const GrammarCore<T>* that, const T& input, size_t index)
{
    typename GrammarCore<T>::GrammarResult result;
    for (auto n : that->left->call(input, index)) {
        for (auto m : that->right->call(input, n->value->end)) {
            result.push_back(
                new GrammarCore<T>::ParsedTree(
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
    auto lst = that->left->call(input, index);
    auto otherResult = that->right->call(input, index);
    lst.insert(lst.end(), otherResult.begin(), otherResult.end());
    typename GrammarCore<T>::GrammarResult result;
    for (auto n : lst) {
        result.push_back(
            new GrammarCore<T>::ParsedTree(
                new ParsedNode<T>(
                            that,
                            index,
                            n->value->end,
                            input), {n}));
    }
    return result;
}

template <typename T>
typename GrammarCore<T>::GrammarResult attach(const GrammarCore<T>* that, const T& input, size_t index)
{
    //return that->left->call(input, index);
    typename GrammarCore<T>::GrammarResult result;
    for (auto n : that->left->call(input, index)) {
        result.push_back(
            new GrammarCore<T>::ParsedTree(
                new ParsedNode<T>(
                            that,
                            index,
                            n->value->end,
                            input), {n}));
    }
    return result;
}

template <typename T>
typename GrammarCore<T>::GrammarResult GrammarCore<T>::call(const T& input, size_t index) const
{
    return func(this, input, index);
}

template <typename T>
GrammarCore<T>* GrammarCore<T>::create_add(const GrammarCore<T>* other) const
{
    GrammarCore<T> *res = new GrammarCore<T>();
    res->left = this;
    res->right = other;
    res->func = add<T>;
    res->transform = transform_add<T>;
    return res;
}

template <typename T>
GrammarCore<T>* GrammarCore<T>::create_or(const GrammarCore<T>* other) const
{
    GrammarCore<T> *res = new GrammarCore<T>();
    res->left = this;
    res->right = other;
    res->func = alternative<T>;
    res->transform = transform_assign<T>;
    //res->transform = transform_contract<T>;
    return res;
}

template <typename T>
GrammarCore<T>* GrammarCore<T>::create_attach(const GrammarCore<T>::Transformation& trans) const
{
    GrammarCore<T> *res = new GrammarCore<T>();
    res->left = this;
    res->func = attach<T>;
    res->transform = trans;
    return res;
}

template <typename T>
void GrammarCore<T>::create_assignment(const GrammarCore<T>* other)
{
    this->left = other;
    this->func = assign<T>;
    this->transform = transform_assign<T>;
}


template<typename T>
GrammarNode<T>::GrammarNode()
{
    this->node = new GrammarCore<T>();
}


template<typename T>
GrammarNode<T> GrammarNode<T>::operator+(const GrammarNode<T>& other) const
{
    return GrammarNode<T>(this->node->create_add(other.node));
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
GrammarNode<T> GrammarNode<T>::operator<<(const typename GrammarCore<T>::Transformation& trans) const
{
    return GrammarNode<T>(this->node->create_attach(trans));
}

template<typename T>
GrammarCore<T>::GrammarResult GrammarNode<T>::operator()(const T& input, size_t index) const
{
    return this->node->call(input, index);
}

template<typename T>
GrammarCore<T>::ParsedTree* transform(const typename GrammarCore<T>::ParsedTree* a){
    if (a == nullptr) {
        return nullptr;
    }
    
    // std::vector<const typename GrammarCore<T>::ParsedTree*> targs;
    // for (auto arg : a->args) {
    //     if (arg->value->node->transform != nullptr) {
    //         targs.push_back(transform<T>(arg->value->node->transform(arg)));
    //     }
    //     else {
    //         targs.push_back(transform<T>(arg));
    //     }
    //
    // }
    //
    // return new GrammarCore<T>::ParsedTree(a->value, targs);
    if (a->value->node->func == attach<T>) {
        return a->value->node->transform(transform<T>(a->args[0]));
    }

    std::vector<const typename GrammarCore<T>::ParsedTree*> targs;
    for (auto arg : a->args) {
            targs.push_back(transform<T>(arg));
    }

    if (a->value->node->transform != nullptr) {
        return a->value->node->transform(new GrammarCore<T>::ParsedTree(a->value, targs));
    }

    return new GrammarCore<T>::ParsedTree(a->value, targs);
}

template <typename T>
GrammarCore<T>::ParsedTree* transform_add(const typename GrammarCore<T>::ParsedTree* a)
{
    std::vector<const typename GrammarCore<T>::ParsedTree*> targs;

    for (auto arg : a->args) {
        if (arg->value->node->func == add<T>) {
            targs.insert(targs.end(), arg->args.begin(), arg->args.end());
        }
        else {
            targs.push_back(arg);
        }
    }

    return new GrammarCore<T>::ParsedTree(a->value, targs);
}

template <typename T>
GrammarCore<T>::ParsedTree* transform_assign(const typename GrammarCore<T>::ParsedTree* a)
{
    return new GrammarCore<T>::ParsedTree(a->value, a->args);
}

template <typename T>
GrammarCore<T>::ParsedTree* transform_contract(const typename GrammarCore<T>::ParsedTree* a)
{
    return new GrammarCore<T>::ParsedTree(a->args[0]->value, a->args[0]->args);
}

template <typename T>
GrammarCore<T>::ParsedTree* transform_blank(const typename GrammarCore<T>::ParsedTree* a)
{
    return new GrammarCore<T>::ParsedTree(a->value);
}


/* END OF TEMPLATE IMPLEMENTATION */

#endif // GRAMMAR_H
