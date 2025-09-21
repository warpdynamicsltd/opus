#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <tuple>

template <typename T>
class ParsedNode;  // Forward declaration

template <typename T = std::string>
class GrammarCore {
public:
    //typedef ParsedNode<T>* GrammarResult;
    typedef std::vector<size_t> GrammarResult;
    typedef GrammarResult(*GrammarFunction)(const GrammarCore<T>*, const T&, size_t);

public:
    GrammarFunction func = nullptr;
    //Mode mode;
    
    const GrammarCore<T> *left = nullptr;
    const GrammarCore<T> *right = nullptr;

    std::string root_string;

public:
    GrammarCore();
    GrammarCore(const GrammarFunction&);
    GrammarCore(const GrammarFunction&, const std::string& root_string);

    std::vector<size_t> call(const T&, size_t) const;

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
class GrammarNode
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

        std::vector<size_t> operator()(const T& input, size_t index) const;

        GrammarNode<T>& operator=(const GrammarNode<T>& other);
};

template <typename T>
class ParsedNode
{
    public:
        GrammarCore<T> *node;

        const ParsedNode<T>* left = nullptr;
        const ParsedNode<T>* right = nullptr;

        size_t index;
        size_t length;
        const T& data;
};

typedef GrammarNode<std::string> GrammarNodeStr;
typedef GrammarCore<std::string> GrammarCoreStr;

GrammarCoreStr::GrammarResult st(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult digit(const GrammarCoreStr* node, const std::string& input, size_t index);

GrammarCoreStr::GrammarResult end(const GrammarCoreStr* node, const std::string& input, size_t index);


#endif // GRAMMAR_H
