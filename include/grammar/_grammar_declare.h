#ifndef GRAMMAR_DEF_H
#define GRAMMAR_DEF_H


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

template <typename T>
class GrammarCore : public MassDealocator<GrammarCore<T>>
{
    public:
        typedef AbstractTerm<ParsedNode<T>*> ParsedTree; 
        typedef std::vector<ParsedTree*> GrammarResult;
        typedef GrammarResult(*GrammarFunction)(const GrammarCore<T>*, const T&, size_t);

        typedef ParsedTree* (*Transformation)(const ParsedTree*, const ParsedTree*);

    public:
        GrammarFunction func = nullptr;
        
        const GrammarCore<T> *left = nullptr;
        const GrammarCore<T> *right = nullptr;

        std::string root_string;

    public:
        Transformation transform = nullptr;

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

template<typename T>
typename GrammarCore<T>::ParsedTree* transform(const typename GrammarCore<T>::ParsedTree* a);

typedef GrammarNode<std::string> GrammarNodeStr;
typedef GrammarCore<std::string> GrammarCoreStr;
typedef ParsedNode<std::string> ParsedNodeStr;


GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult digit(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult end(const GrammarCoreStr* node, const std::string& input, size_t index);

#endif