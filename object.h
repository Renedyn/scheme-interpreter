#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Object;

class Scope {
public:
    Scope();
    Scope(const std::unordered_map<std::string, std::shared_ptr<Object>>& val);
    Scope(const std::shared_ptr<Scope>& parent);
    std::shared_ptr<Object> Get(const std::string& name) const;
    void Set(const std::string& name, std::shared_ptr<Object> value);
    bool InTable(const std::string& name) const;
    bool InScope(const std::string& name) const;
    std::shared_ptr<Scope> Parent() const;

private:
    std::shared_ptr<Scope> parent_;
    std::unordered_map<std::string, std::shared_ptr<Object>> table_;
};

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object();
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>&,
                                          std::shared_ptr<Scope> scope);
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope);
    virtual std::string ToString() const;
};

class Number : public Object {
public:
    Number(int64_t val);
    int64_t GetValue() const;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) override;
    virtual std::string ToString() const override;

private:
    int64_t value_;
};

class Symbol : public Object {
public:
    Symbol(const std::string str);
    const std::string& GetName() const;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) override;
    virtual std::string ToString() const override;

private:
    std::string str_;
};

class Cell : public Object {
public:
    Cell();
    Cell(const std::shared_ptr<Object>& fr, const std::shared_ptr<Object>& sc);
    void SetFirst(std::shared_ptr<Object> vert);
    void SetSecond(std::shared_ptr<Object> vert);
    std::shared_ptr<Object> GetFirst() const;
    std::shared_ptr<Object> GetSecond() const;

    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) override;

    virtual std::string ToString() const override;

private:
    std::shared_ptr<Object> fr_;
    std::shared_ptr<Object> sc_;
};

std::vector<std::shared_ptr<Object>> EvalForAll(const std::vector<std::shared_ptr<Object>>& vec,
                                                std::shared_ptr<Scope> scope);
std::vector<std::shared_ptr<Object>> UnwrapProperList(const std::shared_ptr<Cell>& root);
bool IsProperList(const std::shared_ptr<Object>& root);
std::shared_ptr<Object> GetBool(bool var);
std::shared_ptr<Object> GetProperList(const std::vector<std::shared_ptr<Object>>& vec);

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}

bool IsBool(const std::shared_ptr<Object>& obj);

bool IsTrue(const std::shared_ptr<Object>& obj);

template <class T>
bool IsForAll(const std::vector<std::shared_ptr<Object>>& vec) {
    for (auto i : vec) {
        if (i == nullptr || !Is<T>(i)) {
            return false;
        }
    }
    return true;
}

void PrintAst(const std::shared_ptr<Object>& ptr, int d = 0);