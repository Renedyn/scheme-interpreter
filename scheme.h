#pragma once

#include <memory>
#include <string>
#include "object.h"
#include "object_func.h"

class Interpreter {
public:
    std::string Run(const std::string&);

    std::shared_ptr<Scope> global_scope =
        std::make_shared<Scope>(std::unordered_map<std::string, std::shared_ptr<Object>>{
            {"+", std::make_shared<Addition>()},
            {"-", std::make_shared<Subtraction>()},
            {"*", std::make_shared<Multiplying>()},
            {"/", std::make_shared<Division>()},
            {"number?", std::make_shared<CheckInteger>()},
            {"=", std::make_shared<Equal>()},
            {"<", std::make_shared<Less>()},
            {"<=", std::make_shared<LessEqual>()},
            {">", std::make_shared<More>()},
            {">=", std::make_shared<MoreEqual>()},
            {"min", std::make_shared<Min>()},
            {"max", std::make_shared<Max>()},
            {"abs", std::make_shared<Absolute>()},
            {"boolean?", std::make_shared<CheckBoolean>()},
            {"not", std::make_shared<Not>()},
            {"and", std::make_shared<And>()},
            {"or", std::make_shared<Or>()},
            {"quote", std::make_shared<Quote>()},
            {"pair?", std::make_shared<IsPair>()},
            {"null?", std::make_shared<IsNull>()},
            {"list?", std::make_shared<IsList>()},
            {"cons", std::make_shared<ConstructPair>()},
            {"car", std::make_shared<FirstOfPair>()},
            {"cdr", std::make_shared<SecondOfPair>()},
            {"list", std::make_shared<ConstructList>()},
            {"list-ref", std::make_shared<ListInd>()},
            {"list-tail", std::make_shared<ListTail>()},
            {"if", std::make_shared<If>()},
            {"lambda", std::make_shared<LambdaCommand>()},
            {"define", std::make_shared<Define>()},
            {"set!", std::make_shared<Set>()},
            {"symbol?", std::make_shared<IsSymbol>()},
            {"set-car!", std::make_shared<SetCar>()},
            {"set-cdr!", std::make_shared<SetCdr>()}});
};
