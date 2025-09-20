#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <tuple>

// Represents a term produced by grammar parsing
struct GrammarTerm {
    std::string name;
    std::vector<GrammarTerm> args;

    GrammarTerm(const std::string& name, const std::vector<GrammarTerm>& args = {})
        : name(name), args(args) {}
};

// Represents a grammar node for parsing operations
template <typename T = std::string>
class GrammarNode {
public:
    using GrammarFunction = std::function<std::vector<std::tuple<size_t, GrammarTerm>>(const T&, size_t)>;

    typedef std::vector<size_t>(*GrammarFunction2)(const GrammarNode<T>*, const T&, size_t);
    enum Mode {PLUS, OR, ASSIGN, ROOT};

public:
    std::string name;
    bool consumable;
    GrammarFunction func;
    
    GrammarFunction2 func2;
    Mode mode;
    
    const GrammarNode<T> *left;
    const GrammarNode<T> *right;

    std::string root_string;

public:
    // Constructors
    GrammarNode();
    GrammarNode(const GrammarFunction2&);
    GrammarNode(const GrammarFunction2&, const std::string& root_string);


    std::vector<size_t> _assign(const T&, size_t) const;
    std::vector<size_t> _plus(const T&, size_t) const;
    std::vector<size_t> _or(const T&, size_t) const;

    std::vector<size_t> _call(const T&, size_t) const;

    void create_assignment(const GrammarNode<T>* other);
    GrammarNode<T>* create_plus(const GrammarNode<T>* other) const;
    GrammarNode<T>* create_or(const GrammarNode<T>* other) const;

    // Core parsing function
    std::vector<size_t> operator()(const T& input, size_t index) const;

    // Factory for constructing grammar nodes
    static GrammarNode<T>& Factory(const GrammarFunction& func);
};

template <typename T>
class SmartGrammarNode
{
    public:
        SmartGrammarNode(GrammarNode<T>* node) : node(node) {}
        GrammarNode<T> *node;

        SmartGrammarNode<T> operator+(const SmartGrammarNode<T>& other) const;
        SmartGrammarNode<T> operator|(const SmartGrammarNode<T>& other) const;

        std::vector<size_t> operator()(const T& input, size_t index) const;

        SmartGrammarNode<T>& operator=(const SmartGrammarNode<T>& other);
};

#endif // GRAMMAR_H
