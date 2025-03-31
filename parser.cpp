#include <parser.h>
#include <memory>
#include "error.h"
#include "tokenizer.h"

std::shared_ptr<Object> ReadAny(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Empty string");
    }
    std::shared_ptr<Object> root;
    if (std::holds_alternative<SymbolToken>(tokenizer->GetToken())) {
        root = std::make_shared<Symbol>(std::get<SymbolToken>(tokenizer->GetToken()).name);
    } else if (std::holds_alternative<ConstantToken>(tokenizer->GetToken())) {
        root = std::make_shared<Number>(std::get<ConstantToken>(tokenizer->GetToken()).value);
    } else if (std::holds_alternative<QuoteToken>(tokenizer->GetToken())) {
        tokenizer->Next();
        root = std::make_shared<Cell>(std::make_shared<Symbol>("quote"),
                                      std::make_shared<Cell>(ReadAny(tokenizer), nullptr));
        return root;
    } else if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
               get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE) {
        throw SyntaxError("Bad parens");
    }

    if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
        get<BracketToken>(tokenizer->GetToken()) == BracketToken::OPEN) {
        tokenizer->Next();
        return ReadList(tokenizer);
    } else {
        tokenizer->Next();
        return root;
    }
}

std::shared_ptr<Object> ReadList(Tokenizer* tokenizer) {
    if (tokenizer->IsEnd()) {
        throw SyntaxError("Broken start of a list");
    }
    std::shared_ptr<Cell> root = std::make_shared<Cell>();
    std::shared_ptr<Cell> cur = root;
    bool first_iteration = true;
    while (!(std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
             get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE)) {
        if (std::holds_alternative<DotToken>(tokenizer->GetToken())) {
            if (first_iteration) {
                throw SyntaxError("Bad dot position");
            }
            tokenizer->Next();
            if (tokenizer->IsEnd()) {
                throw SyntaxError("ill formed list 1");
            }
            cur->SetSecond(ReadAny(tokenizer));
            if (tokenizer->IsEnd()) {
                throw SyntaxError("ill formed list 2");
            }
            if (std::holds_alternative<BracketToken>(tokenizer->GetToken()) &&
                get<BracketToken>(tokenizer->GetToken()) == BracketToken::CLOSE) {
                break;
            } else {
                throw SyntaxError("ill formed list 3");
            }
        }
        if (!first_iteration) {
            cur->SetSecond(std::make_shared<Cell>());
            cur = std::dynamic_pointer_cast<Cell>(cur->GetSecond());
        }
        first_iteration = false;
        cur->SetFirst(ReadAny(tokenizer));

        if (tokenizer->IsEnd()) {
            throw SyntaxError("Broken pair");
        }
    }
    tokenizer->Next();
    if (first_iteration) {
        return nullptr;
    }
    return root;
}

std::shared_ptr<Object> Read(Tokenizer* tokenizer) {
    auto ptr = ReadAny(tokenizer);
    if (!tokenizer->IsEnd()) {
        throw SyntaxError("Extra symbols");
    }
    return ptr;
}