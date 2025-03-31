#pragma once

#include "object.h"
#include <memory>
#include "error.h"

//////////// Integer functions

class CheckInteger : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Equal : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Less : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class LessEqual : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class More : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class MoreEqual : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Addition : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Subtraction : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Multiplying : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Division : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Min : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Max : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Absolute : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

//////////// Boolean functions

class CheckBoolean : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Not : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class And : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Or : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

//////////// Quote function
class Quote : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

/////////// List functions
class IsPair : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class IsNull : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class IsList : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class IsSymbol : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class ConstructPair : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class FirstOfPair : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class SecondOfPair : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class ConstructList : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class ListInd : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class ListTail : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class If : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class LambdaCommand : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class SetCar : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class SetCdr : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Lambda : public Object {
public:
    Lambda(const std::vector<std::shared_ptr<Object>>& args,
           const std::vector<std::shared_ptr<Object>>& body, std::shared_ptr<Scope> scope);
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
    virtual std::shared_ptr<Object> Eval(std::shared_ptr<Scope> scope) override;
    virtual std::string ToString() const override;

private:
    std::vector<std::shared_ptr<Object>> args_;
    std::vector<std::shared_ptr<Object>> body_;
    std::shared_ptr<Scope> scope_;
};

class Define : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};

class Set : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
                                          std::shared_ptr<Scope> scope) override;
};
/*
class Begin : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
std::shared_ptr<Scope> scope) override;
};

class Eval : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
std::shared_ptr<Scope> scope) override;
};

class Apply : public Object {
public:
    virtual std::shared_ptr<Object> Apply(const std::vector<std::shared_ptr<Object>>& data,
std::shared_ptr<Scope> scope) override;
};*/
