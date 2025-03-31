#include "object.h"
#include <iostream>
#include <memory>
#include "error.h"
#include "object_func.h"

std::vector<std::shared_ptr<Object>> EvalForAll(const std::vector<std::shared_ptr<Object>>& vec,
                                                std::shared_ptr<Scope> scope) {
    std::vector<std::shared_ptr<Object>> res(vec.size());
    for (size_t id = 0; id < vec.size(); ++id) {
        if (vec[id] == nullptr) {
            throw RuntimeError("Trying to evaluate empty list");
        }
        res[id] = vec[id]->Eval(scope);
    }
    return res;
}

std::vector<std::shared_ptr<Object>> UnwrapProperList(const std::shared_ptr<Cell>& root) {
    std::vector<std::shared_ptr<Object>> res;
    for (auto ptr = root; ptr != nullptr; ptr = As<Cell>(ptr->GetSecond())) {
        res.push_back(ptr->GetFirst());
        if (!Is<Cell>(ptr->GetSecond()) && ptr->GetSecond() != nullptr) {
            throw RuntimeError("Bad proper list's tail");
        }
    }
    return res;
}

std::shared_ptr<Object> GetBool(bool var) {
    return std::make_shared<Symbol>(var ? "#t" : "#f");
}

bool IsBool(const std::shared_ptr<Object>& obj) {
    return Is<Symbol>(obj) &&
           (As<Symbol>(obj)->GetName() == "#t" || As<Symbol>(obj)->GetName() == "#f");
}

bool IsTrue(const std::shared_ptr<Object>& obj) {
    return !(IsBool(obj) && As<Symbol>(obj)->GetName() == "#f");
}

bool IsProperList(const std::shared_ptr<Object>& root) {
    if (root == nullptr) {
        return true;
    }
    if (!Is<Cell>(root)) {
        return false;
    }
    for (std::shared_ptr<Cell> ptr = As<Cell>(root); ptr != nullptr;
         ptr = As<Cell>(ptr->GetSecond())) {
        if (!Is<Cell>(ptr->GetSecond()) && ptr->GetSecond() != nullptr) {
            return false;
        }
    }
    return true;
}

std::shared_ptr<Object> GetProperList(const std::vector<std::shared_ptr<Object>>& vec) {
    if (vec.empty()) {
        return nullptr;
    }
    std::shared_ptr<Cell> root = std::make_shared<Cell>(vec[0], nullptr);
    auto cur = root;
    for (size_t id = 1; id < vec.size(); ++id) {
        cur->SetSecond(std::make_shared<Cell>(vec[id], nullptr));
        cur = As<Cell>(cur->GetSecond());
    }
    return root;
}

// Object

Object::~Object() = default;

std::shared_ptr<Object> Object::Apply(const std::vector<std::shared_ptr<Object>>&,
                                      std::shared_ptr<Scope>) {
    throw RuntimeError("No implemintation for Apply");
}
std::shared_ptr<Object> Object::Eval(std::shared_ptr<Scope>) {
    throw RuntimeError("No implemintation for Eval");
}
std::string Object::ToString() const {
    throw RuntimeError("No implemintation for ToString");
}

// Number

Number::Number(int64_t val) : value_(val) {
}
int64_t Number::GetValue() const {
    return value_;
}
std::shared_ptr<Object> Number::Eval(std::shared_ptr<Scope>) {
    return std::make_shared<Number>(value_);
}
std::string Number::ToString() const {
    return std::to_string(value_);
}

// Symbol

Symbol::Symbol(const std::string str) : str_(str) {
}
const std::string& Symbol::GetName() const {
    return str_;
}
std::shared_ptr<Object> Symbol::Eval(std::shared_ptr<Scope> scope) {
    if (str_ == "#t" || str_ == "#f") {
        return std::make_shared<Symbol>(str_);
    }
    if (scope->InScope(str_)) {
        return scope->Get(str_);
    } else {
        throw NameError("no name");
    }
}
std::string Symbol::ToString() const {
    return str_;
}

// Cell
Cell::Cell() = default;

Cell::Cell(const std::shared_ptr<Object>& fr, const std::shared_ptr<Object>& sc)
    : fr_(fr), sc_(sc) {
}

void Cell::SetFirst(std::shared_ptr<Object> vert) {
    fr_ = vert;
}
void Cell::SetSecond(std::shared_ptr<Object> vert) {
    sc_ = vert;
}

std::shared_ptr<Object> Cell::GetFirst() const {
    return fr_;
}
std::shared_ptr<Object> Cell::GetSecond() const {
    return sc_;
}

std::shared_ptr<Object> Cell::Eval(std::shared_ptr<Scope> scope) {
    if (fr_ == nullptr) {
        throw RuntimeError("Cannot evaluate expression, nullptr");
    }
    if (sc_ != nullptr && !Is<Cell>(sc_)) {
        throw RuntimeError("Cannot evaluate expression, not cell");
    }

    if (Is<Symbol>(fr_) && scope->InScope(As<Symbol>(fr_)->GetName())) {
        auto fun = scope->Get(As<Symbol>(fr_)->GetName());
        auto vec = UnwrapProperList(As<Cell>(sc_));
        return fun->Apply(vec, scope);
    } else {
        auto fun = fr_->Eval(scope);
        if (!Is<Lambda>(fun)) {
            throw RuntimeError("Cannot evaluate expression, not lambda");
        }
        auto vec = UnwrapProperList(As<Cell>(sc_));
        return fun->Apply(vec, scope);
    }
}

std::string Cell::ToString() const {  // Slow and strange and wrong
    if (fr_ == nullptr && sc_ == nullptr) {
        return "(())";
    }
    std::string res = "(";
    if (IsProperList(std::make_shared<Cell>(*this))) {
        auto vec = UnwrapProperList(std::make_shared<Cell>(*this));
        for (auto v : vec) {
            if (v == nullptr) {
                res += "() ";
            } else {
                res += v->ToString() + " ";
            }
        }
        res.pop_back();
    } else {
        for (auto ptr = std::make_shared<Cell>(*this);; ptr = As<Cell>(ptr->GetSecond())) {
            if (ptr->GetFirst() == nullptr) {
                res += "()";
            } else {
                res += ptr->GetFirst()->ToString() + " ";
            }
            if (!Is<Cell>(ptr->GetSecond())) {
                res += ". ";
                res += ptr->GetSecond()->ToString();
                break;
            }
        }
    }

    res += ")";
    return res;
}

void PrintAst(const std::shared_ptr<Object>& ptr, int d) {
    if (ptr == nullptr) {
        std::cout << std::string(3 * d, ' ') << "nullptr" << std::endl;
        return;
    }
    if (Is<Cell>(ptr)) {
        std::cout << std::string(3 * d, ' ') << "Cell {" << std::endl;
        PrintAst(As<Cell>(ptr)->GetFirst(), d + 1);
        PrintAst(As<Cell>(ptr)->GetSecond(), d + 1);
        std::cout << std::string(3 * d, ' ') << "}" << std::endl;
        return;
    }
    if (Is<Symbol>(ptr)) {
        std::cout << std::string(3 * d, ' ') << As<Symbol>(ptr)->GetName() << std::endl;
        return;
    }
    if (Is<Number>(ptr)) {
        std::cout << std::string(3 * d, ' ') << As<Number>(ptr)->GetValue() << std::endl;
        return;
    }
}
