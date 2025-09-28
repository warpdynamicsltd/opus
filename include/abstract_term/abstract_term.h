#ifndef ABSTRACT_TERM_H
#define ABSTRACT_TERM_H

#include "memory.h"

template <typename T>
struct AbstractTerm : public MassDealocator<AbstractTerm<T>> {
    explicit AbstractTerm(const T value) : value(value) {};
    AbstractTerm(const T value, std::vector<const AbstractTerm<T>*> args) : value(value), args(args) {};
    const T value;
    std::vector<const AbstractTerm<T>*> args;

    std::string to_string() const;
};

template<typename T>
std::string AbstractTerm<T>::to_string() const {
    std::string result;

    if (this == nullptr) {
        return "nullptr";
    }

    result += value->to_string() + "(";
    for (auto &arg : args) {
        result += arg->to_string() + ",";
    }
    result += ")";

    return result;
}

#endif