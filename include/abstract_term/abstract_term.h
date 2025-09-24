#ifndef ABSTRACT_TERM_H
#define ABSTRACT_TERM_H

#include "memory.h"

template <typename T>
struct AbstractTerm : public MassDealocator
{
    AbstractTerm(T value) : value(value) {};
    AbstractTerm(T value, std::vector<AbstractTerm<T>*> args) : value(value), args(args) {};
    const T value;
    std::vector<AbstractTerm<T>*> args;
};

#endif