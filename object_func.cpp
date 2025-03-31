#include "object_func.h"
#include "object.h"
#include <memory>
#include "error.h"

////////////// Integer functions

std::shared_ptr<Object> CheckInteger::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                            std::shared_ptr<Scope> scope) {
    if (data.size() != 1) {
        throw RuntimeError("Incorrect arguments for CheckInteger");
    }
    return GetBool(Is<Number>(data[0]->Eval(scope)));
}

static std::shared_ptr<Object> Compare(const std::vector<std::shared_ptr<Object>>& data,
                                       std::shared_ptr<Scope> scope, auto comparator) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec)) {
        throw RuntimeError("Incorrect arguments for Compare");
    }
    if (!vec.empty()) {
        int64_t old = As<Number>(vec[0])->GetValue();
        for (size_t id = 1; id < vec.size(); ++id) {
            int64_t nw = As<Number>(vec[id])->GetValue();
            if (!comparator(old, nw)) {
                return GetBool(false);
            }
            old = nw;
        }
    }
    return GetBool(true);
}

std::shared_ptr<Object> Equal::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                     std::shared_ptr<Scope> scope) {
    return Compare(data, scope, [](int64_t a, int64_t b) { return a == b; });
}
std::shared_ptr<Object> Less::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                    std::shared_ptr<Scope> scope) {
    return Compare(data, scope, [](int64_t a, int64_t b) { return a < b; });
}
std::shared_ptr<Object> LessEqual::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                         std::shared_ptr<Scope> scope) {
    return Compare(data, scope, [](int64_t a, int64_t b) { return a <= b; });
}
std::shared_ptr<Object> More::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                    std::shared_ptr<Scope> scope) {
    return Compare(data, scope, [](int64_t a, int64_t b) { return a > b; });
}
std::shared_ptr<Object> MoreEqual::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                         std::shared_ptr<Scope> scope) {
    return Compare(data, scope, [](int64_t a, int64_t b) { return a >= b; });
}
std::shared_ptr<Object> Addition::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                        std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec)) {
        throw RuntimeError("Incorrect arguments for addition");
    }
    int64_t res = 0;
    for (auto v : vec) {
        res += As<Number>(v)->GetValue();
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Subtraction::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                           std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec) || vec.empty()) {
        throw RuntimeError("Incorrect arguments for subtraction");
    }
    int64_t res = As<Number>(vec[0])->GetValue();
    for (size_t id = 1; id < vec.size(); ++id) {
        res -= As<Number>(vec[id])->GetValue();
    }
    if (vec.size() == 1) {
        res *= -1;
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Multiplying::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                           std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec)) {
        throw RuntimeError("Incorrect arguments for multiplying");
    }
    int64_t res = 1;
    for (auto v : vec) {
        res *= As<Number>(v)->GetValue();
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Division::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                        std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec) || vec.size() <= 1) {
        throw RuntimeError("Incorrect arguments for subtraction");
    }
    int64_t res = As<Number>(vec[0])->GetValue();
    for (size_t id = 1; id < vec.size(); ++id) {
        res /= As<Number>(vec[id])->GetValue();
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Min::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                   std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec) || vec.empty()) {
        throw RuntimeError("Incorrect arguments for Min");
    }
    int64_t res = As<Number>(vec[0])->GetValue();
    for (size_t id = 1; id < vec.size(); ++id) {
        res = std::min(res, As<Number>(vec[id])->GetValue());
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Max::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                   std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec) || vec.empty()) {
        throw RuntimeError("Incorrect arguments for Max");
    }
    int64_t res = As<Number>(vec[0])->GetValue();
    for (size_t id = 1; id < vec.size(); ++id) {
        res = std::max(res, As<Number>(vec[id])->GetValue());
    }
    return std::make_shared<Number>(res);
}

std::shared_ptr<Object> Absolute::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                        std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (!IsForAll<Number>(vec) || vec.size() != 1) {
        throw RuntimeError("Incorrect arguments for Abs");
    }
    return std::make_shared<Number>(std::abs(As<Number>(vec[0])->GetValue()));
}

////////////// Boolean functions

std::shared_ptr<Object> CheckBoolean::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                            std::shared_ptr<Scope> scope) {
    if (data.size() != 1) {
        throw RuntimeError("Incorrect arguments for CheckBoolean");
    }
    return GetBool(IsBool(data[0]->Eval(scope)));
}

std::shared_ptr<Object> Not::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                   std::shared_ptr<Scope> scope) {
    auto vec = EvalForAll(data, scope);
    if (vec.size() != 1) {
        throw RuntimeError("Incorrect arguments for Not");
    }
    return GetBool(!IsTrue(vec[0]));
}

std::shared_ptr<Object> And::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                   std::shared_ptr<Scope> scope) {
    if (data.empty()) {
        return GetBool(true);
    }
    std::shared_ptr<Object> last;
    for (size_t id = 0; id < data.size(); ++id) {
        last = data[id]->Eval(scope);
        if (!IsTrue(last)) {
            return last;
        }
    }
    return last;
}

std::shared_ptr<Object> Or::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                  std::shared_ptr<Scope> scope) {
    if (data.empty()) {
        return GetBool(false);
    }
    std::shared_ptr<Object> last;
    for (size_t id = 0; id < data.size(); ++id) {
        last = data[id]->Eval(scope);
        if (IsTrue(last)) {
            return last;
        }
    }
    return last;
}

////////////// Quote function

std::shared_ptr<Object> Quote::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                     std::shared_ptr<Scope>) {
    if (data.empty()) {
        return nullptr;
    }
    if (data.size() != 1) {
        throw SyntaxError("Bad quote arguments");
    }
    return data[0];
}

std::shared_ptr<Object> IsPair::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() != 1 || data[0] == nullptr) {
        throw RuntimeError("Bad IsPair arguments");
    }
    auto res = data[0]->Eval(scope);
    return GetBool(Is<Cell>(res));
}

std::shared_ptr<Object> IsNull::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() != 1 || data[0] == nullptr) {
        throw RuntimeError("Bad IsNull arguments");
    }
    auto res = data[0]->Eval(scope);
    return GetBool(res == nullptr);
}

std::shared_ptr<Object> IsSymbol::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                        std::shared_ptr<Scope> scope) {
    if (data.size() != 1 || data[0] == nullptr) {
        throw RuntimeError("Bad IsSymbol arguments");
    }
    auto res = data[0]->Eval(scope);
    return GetBool(Is<Symbol>(res) && As<Symbol>(res)->GetName() != "#f" &&
                   As<Symbol>(res)->GetName() != "#t");
}

std::shared_ptr<Object> IsList::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() != 1 || data[0] == nullptr) {
        throw RuntimeError("Bad IsList arguments");
    }
    auto res = data[0]->Eval(scope);
    return GetBool(IsProperList(res));
}

std::shared_ptr<Object> ConstructPair::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                             std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw RuntimeError("Bad ConstructPair arguments");
    }
    auto res = EvalForAll(data, scope);
    return std::make_shared<Cell>(res[0], res[1]);
}

std::shared_ptr<Object> FirstOfPair::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                           std::shared_ptr<Scope> scope) {
    if (data.size() != 1) {
        throw RuntimeError("Bad FirstOfPair arguments");
    }
    auto res = data[0]->Eval(scope);
    if (res == nullptr) {
        throw RuntimeError("Bad FirstOfPair arguments");
    }
    return As<Cell>(res)->GetFirst();
}

std::shared_ptr<Object> SecondOfPair::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                            std::shared_ptr<Scope> scope) {
    if (data.size() != 1) {
        throw RuntimeError("Bad FirstOfPair arguments");
    }
    auto res = data[0]->Eval(scope);
    if (res == nullptr) {
        throw RuntimeError("Bad FirstOfPair arguments");
    }
    return As<Cell>(res)->GetSecond();
}

std::shared_ptr<Object> ConstructList::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                             std::shared_ptr<Scope> scope) {
    auto res = EvalForAll(data, scope);
    return GetProperList(res);
}

std::shared_ptr<Object> ListInd::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                       std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw RuntimeError("Bad ListInd arguments");
    }
    auto res = EvalForAll(data, scope);
    std::vector<std::shared_ptr<Object>> vec;
    if (IsProperList(res[0])) {
        vec = UnwrapProperList(As<Cell>(res[0]));
    } else {
        throw RuntimeError("Bad ListInd arguments");
    }
    if (Is<Number>(res[1]) && As<Number>(res[1])->GetValue() >= 0 &&
        As<Number>(res[1])->GetValue() < static_cast<int64_t>(vec.size())) {
        return vec[As<Number>(res[1])->GetValue()];
    }
    throw RuntimeError("Bad ListInd arguments");
}

std::shared_ptr<Object> ListTail::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                        std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw RuntimeError("Bad ListTail arguments");
    }
    auto res = EvalForAll(data, scope);
    std::vector<std::shared_ptr<Object>> vec;
    if (IsProperList(res[0])) {
        vec = UnwrapProperList(As<Cell>(res[0]));
    } else {
        throw RuntimeError("Bad ListTail arguments");
    }
    if (Is<Number>(res[1]) && As<Number>(res[1])->GetValue() >= 0 &&
        As<Number>(res[1])->GetValue() <= static_cast<int64_t>(vec.size())) {
        vec.erase(vec.begin(), vec.begin() + As<Number>(res[1])->GetValue());
        return GetProperList(vec);
    }
    throw RuntimeError("Bad ListTail arguments");
}

std::shared_ptr<Object> If::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                  std::shared_ptr<Scope> scope) {
    if ((data.size() != 2 && data.size() != 3)) {
        throw SyntaxError("Bad If arguments");
    }
    if (data[0] == nullptr) {
        throw RuntimeError("Bad If arguments");
    }
    auto if_exp = data[0]->Eval(scope);
    if (IsTrue(if_exp)) {
        if (data[1] == nullptr) {
            throw RuntimeError("Bad If arguments");
        }
        return data[1]->Eval(scope);
    } else {
        if (data.size() == 2) {
            return nullptr;
        }
        if (data[2] == nullptr) {
            throw RuntimeError("Bad If arguments");
        }
        return data[2]->Eval(scope);
    }
}

Scope::Scope() = default;

Scope::Scope(const std::shared_ptr<Scope>& parent) : parent_(parent) {
}

std::shared_ptr<Object> Scope::Get(const std::string& name) const {
    if (table_.count(name)) {
        return table_.at(name);
    }
    if (parent_ != nullptr) {
        return parent_->Get(name);
    }
    return nullptr;
}

void Scope::Set(const std::string& name, std::shared_ptr<Object> value) {
    table_[name] = value;
}

bool Scope::InTable(const std::string& name) const {
    return table_.count(name);
}
bool Scope::InScope(const std::string& name) const {
    if (InTable(name)) {
        return true;
    }
    return parent_ != nullptr && parent_->InScope(name);
}

Scope::Scope(const std::unordered_map<std::string, std::shared_ptr<Object>>& val)
    : parent_(nullptr), table_(val) {
}

std::shared_ptr<Scope> Scope::Parent() const {
    return parent_;
}

std::shared_ptr<Object> LambdaCommand::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                             std::shared_ptr<Scope> scope) {
    if (data.size() < 2) {
        throw SyntaxError("Bad args for LambdaCommand");
    }
    std::cout << "OKEY COCKY" << std::endl;
    auto cp = data;
    cp.erase(cp.begin());
    if (IsProperList(data[0]) && IsForAll<Symbol>(UnwrapProperList(As<Cell>(data[0])))) {
        return std::make_shared<Lambda>(UnwrapProperList(As<Cell>(data[0])), cp, scope);
    } else {
        throw RuntimeError("Bad args for LambdaCommand");
    }
}

Lambda::Lambda(const std::vector<std::shared_ptr<Object>>& args,
               const std::vector<std::shared_ptr<Object>>& body, std::shared_ptr<Scope> scope)
    : args_(args), body_(body), scope_(scope) {
    if (!IsForAll<Symbol>(args)) {
        throw RuntimeError("Bad args for make lambda");
    }
}

std::shared_ptr<Object> Lambda::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> par_scope) {
    if (data.size() != args_.size()) {
        throw RuntimeError("Bad arguments for apply lambda");
    }
    auto scope = std::make_shared<Scope>(scope_);
    for (size_t id = 0; id < args_.size(); ++id) {
        scope->Set(As<Symbol>(args_[id])->GetName(), data[id]->Eval(par_scope));
    }
    return (this)->Eval(scope);
}

std::shared_ptr<Object> Lambda::Eval(std::shared_ptr<Scope> scope) {
    // if (scope_->!= args_.size()) {
    //     throw RuntimeError("Bad arguments for apply lambda");
    // }

    for (size_t id = 0; id < body_.size(); ++id) {
        if (body_[id] == nullptr) {
            throw RuntimeError("Trying evaluate ()");
        }
        auto res = body_[id]->Eval(scope);
        if (id + 1 == body_.size()) {
            return res;
        }
    }
    throw RuntimeError("Empty body");
}

std::string Lambda::ToString() const {
    std::string str = "(lambda (";
    if (!IsForAll<Symbol>(args_)) {
        throw RuntimeError("Bad args for ToString lambda");
    }
    for (auto i : args_) {
        str += i->ToString() + " ";
    }
    str.pop_back();
    str += ") ";
    for (auto v : body_) {
        if (v == nullptr) {
            str += "() ";
        } else {
            str += v->ToString() + " ";
        }
    }
    str.pop_back();
    str += ")";
    return str;
}

std::shared_ptr<Object> Define::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() < 2 || data[0] == nullptr) {
        throw SyntaxError("Bad args for define");
    }
    if (Is<Symbol>(data[0])) {
        if (data.size() != 2 || data[1] == nullptr) {
            throw SyntaxError("Bad args for define");
        }
        // scope->Set(As<Symbol>(data[0])->GetName(), nullptr);  // Is okay?
        scope->Set(As<Symbol>(data[0])->GetName(), data[1]->Eval(scope));
        return nullptr;
    } else if (IsProperList(data[0])) {
        auto args = UnwrapProperList(As<Cell>(data[0]));
        if (!IsForAll<Symbol>(args)) {
            throw RuntimeError("Bad args for define");
        }
        auto cp = args;
        cp.erase(cp.begin());
        auto dt = data;
        dt.erase(dt.begin());
        auto lm = std::make_shared<Lambda>(cp, dt, scope);
        // scope->Set(As<Symbol>(data[0])->GetName(), nullptr);  // Is okay?
        scope->Set(As<Symbol>(args[0])->GetName(), lm);
        return nullptr;
    } else {
        throw SyntaxError("Bad args for define");
    }
}

std::shared_ptr<Object> Set::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                   std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw SyntaxError("Bad args for set");
    }
    if (data[1] == nullptr || data[0] == nullptr || !Is<Symbol>(data[0])) {
        throw RuntimeError("Bad args for set");
    }
    auto ptr = scope;
    for (; ptr != nullptr; ptr = ptr->Parent()) {
        if (ptr->InTable(As<Symbol>(data[0])->GetName())) {
            ptr->Set(As<Symbol>(data[0])->GetName(), data[1]->Eval(scope));
            std::cout << "How" << std::endl;
            return nullptr;
        }
    }
    throw NameError("Bad args for set");
}

std::shared_ptr<Object> SetCar::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw SyntaxError("Bad args for SetCar size");
    }
    if (data[0] == nullptr) {
        throw RuntimeError("Bad args for SetCar nil");
    }
    auto pair = data[0]->Eval(scope);
    if (!Is<Cell>(pair) || data[1] == nullptr) {
        throw RuntimeError("Bad args for SetCar is cell");
    }
    As<Cell>(pair)->SetFirst(data[1]->Eval(scope));
    return nullptr;
}

std::shared_ptr<Object> SetCdr::Apply(const std::vector<std::shared_ptr<Object>>& data,
                                      std::shared_ptr<Scope> scope) {
    if (data.size() != 2) {
        throw SyntaxError("Bad args for set");
    }
    if (data[0] == nullptr) {
        throw RuntimeError("Bad args for set");
    }
    auto pair = data[0]->Eval(scope);
    if (!Is<Cell>(pair) || data[1] == nullptr) {
        throw RuntimeError("Bad args for set");
    }
    As<Cell>(pair)->SetSecond(data[1]->Eval(scope));
    return nullptr;
}