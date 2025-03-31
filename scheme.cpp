#include "scheme.h"
#include <sstream>
#include "error.h"
#include "tokenizer.h"
#include "parser.h"
#include <object_func.h>

std::string Interpreter::Run(const std::string &inp) {
    std::stringstream str(inp);
    auto tokn = Tokenizer(&str);
    auto ast = Read(&tokn);
    if (ast == nullptr) {
        throw RuntimeError("AST is nullptr");
    }
    auto res = ast->Eval(global_scope);
    if (res == nullptr) {
        return "()";
    }
    return res->ToString();
}
