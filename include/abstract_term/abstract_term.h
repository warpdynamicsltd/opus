#ifndef ABSTRACT_TERM_H
#define ABSTRACT_TERM_H

template <typename T>
struct AbstractTerm
{
    AbstractTerm(T value) : value(value) {};
    AbstractTerm(T value, std::vector<AbstractTerm<T>*> args) : value(value), args(args) {};
    const T value;
    std::vector<AbstractTerm<T>*> args;
};

#endif