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
        typedef AbstractTerm<const ParsedNode<T>*> ParsedTree; 
        typedef std::vector<ParsedTree*> GrammarResult;
        typedef GrammarResult(*GrammarFunction)(const GrammarCore<T>*, const T&, size_t);

        typedef ParsedTree* (*Transformation)(const ParsedTree* a);

    public:
        GrammarFunction func = nullptr;
        
        const GrammarCore<T> *left = nullptr;
        const GrammarCore<T> *right = nullptr;

        std::string root_string;
        std::string name;

    public:
        Transformation transform = nullptr;

    public:
        GrammarCore();
        explicit GrammarCore(const GrammarFunction&);
        GrammarCore(const GrammarFunction&, const std::string& root_string);

        GrammarResult call(const T&, size_t) const;

        void create_assignment(const GrammarCore<T>* other);
        GrammarCore<T>* create_add(const GrammarCore<T>* other) const;
        GrammarCore<T>* create_or(const GrammarCore<T>* other) const;
        GrammarCore<T>* create_attach(const Transformation& trans) const;

};

template <typename T>
GrammarCore<T>::GrammarResult assign(const GrammarCore<T>*, const T&, size_t);

template <typename T>
GrammarCore<T>::GrammarResult add(const GrammarCore<T>*, const T&, size_t);

template <typename T>
GrammarCore<T>::GrammarResult alternative(const GrammarCore<T>*, const T&, size_t);

template <typename T>
GrammarCore<T>::GrammarResult attach(const GrammarCore<T>*, const T&, size_t);

template <typename T>
GrammarCore<T>::ParsedTree* transform_add(const typename GrammarCore<T>::ParsedTree*);

template <typename T>
GrammarCore<T>::ParsedTree* transform_assign(const typename GrammarCore<T>::ParsedTree*);

template <typename T>
GrammarCore<T>::ParsedTree* transform_contract(const typename GrammarCore<T>::ParsedTree*);

template <typename T>
class GrammarNode : public MassDealocator<GrammarNode<T>>
{
    public:
        GrammarNode();
        explicit GrammarNode(GrammarCore<T>* node) : node(node) {}
        explicit GrammarNode(const GrammarCore<T>::GrammarFunction& func)
        : node(new GrammarCore<T>(func)) {}
        GrammarNode(const GrammarCore<T>::GrammarFunction& func, const std::string& root_string)
        : node(new GrammarCore<T>(func, root_string)) {}

        GrammarCore<T> *node;

        GrammarNode<T> operator+(const GrammarNode<T>& other) const;
        GrammarNode<T> operator|(const GrammarNode<T>& other) const;
        GrammarNode<T> operator<<(const GrammarCore<T>::Transformation& trans) const;


        GrammarCore<T>::GrammarResult operator()(const T& input, size_t index) const;

        GrammarNode<T>& operator=(const GrammarNode<T>& other);
        GrammarNode<T>& set_name(const std::string& name);
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

    public:
        std::string to_string(int=0) const;
};

template<typename T>
GrammarCore<T>::ParsedTree* transform(const typename GrammarCore<T>::ParsedTree* a);

typedef GrammarNode<std::string> GrammarNodeStr;
typedef GrammarCore<std::string> GrammarCoreStr;
typedef ParsedNode<std::string> ParsedNodeStr;


GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult digit(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult end(const GrammarCoreStr* node, const std::string& input, size_t index);

template <typename T>
GrammarCore<T>::ParsedTree* transform_blank(const typename GrammarCore<T>::ParsedTree*);

#endif