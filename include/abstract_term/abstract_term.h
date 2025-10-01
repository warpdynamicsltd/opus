#ifndef ABSTRACT_TERM_H
#define ABSTRACT_TERM_H

#include "memory.h"

template <typename T>
struct AbstractTerm : public MassDealocator<AbstractTerm<T>> {
    explicit AbstractTerm(const T value) : value(value) {};
    AbstractTerm(const T value, std::vector<const AbstractTerm<T>*> args) : value(value), args(args) {};
    const T value;
    std::vector<const AbstractTerm<T>*> args;

    std::string to_string(int=0) const;
};

template<typename T>
std::string AbstractTerm<T>::to_string(int indent) const {
    std::string result;

    if (this == nullptr) {
        return "nullptr";
    }

    result += value->to_string(indent);
    if (args.size() > 0) {
        result += "\n" + std::string(indent, ' ') + "{";
        int c = 0;
        for (auto &arg : args) {
            if (c++ < args.size() - 1) {
                result += arg->to_string(indent + 2) + ",";
            }
            else {
                result += arg->to_string(indent + 2);
            }
        }
        result += "\n" + std::string(indent, ' ') + "}";
    }

    return result;
}

#endif