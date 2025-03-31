#pragma once

#include <cctype>
#include <iostream>
#include <string_view>
#include <variant>
#include <istream>
#include "error.h"

struct SymbolToken {
    std::string name;
    SymbolToken() = default;
    SymbolToken(std::string_view str) : name(str){};
    SymbolToken(char ch) : name(1, ch){};

    bool operator==(const SymbolToken& other) const {
        return name == other.name;
    }

    constexpr static std::string_view kCharsFirst = "<=>*/#";    // and alpha
    constexpr static std::string_view kCharsRest = "<=>*/#?!-";  // and alpha and digits
};

struct QuoteToken {
    bool operator==(const QuoteToken&) const {
        return true;
    }
};

struct DotToken {
    bool operator==(const DotToken&) const {
        return true;
    }
};

enum class BracketToken { OPEN, CLOSE };

struct ConstantToken {
    int64_t value;
    ConstantToken() : value(0) {
    }
    ConstantToken(int64_t val) : value(val) {
    }

    bool operator==(const ConstantToken& other) const {
        return value == other.value;
    }
};

using Token = std::variant<ConstantToken, BracketToken, SymbolToken, QuoteToken, DotToken>;

class Tokenizer {
public:
    Tokenizer(std::istream* in) : in_(in), is_ended_(false) {
        Next();
    }

    bool IsEnd() {
        return is_ended_;
    }

    void Next() {
        while (isspace(in_->peek())) {  // Think again
            in_->get();
        }

        int fch = in_->get();
        if (fch == EOF) {
            is_ended_ = true;
            return;
        }

        if (fch == '\'') {  // Quote
            last_token_ = QuoteToken();
            return;
        }

        if (fch == '.') {  // Dot
            last_token_ = DotToken();
            return;
        }

        if (fch == '(') {  // OpenBracket
            last_token_ = BracketToken::OPEN;
            return;
        }

        if (fch == ')') {  // CloseBracket
            last_token_ = BracketToken::CLOSE;
            return;
        }

        if ((fch == '+' || fch == '-') && !isdigit(in_->peek())) {  // Symbol
            last_token_ = SymbolToken(static_cast<char>(fch));
            return;
        }

        if (((fch == '+' || fch == '-') && isdigit(in_->peek())) || isdigit(fch)) {  // Constant
            std::string str;
            for (int ch = fch;; ch = in_->get()) {
                str += ch;
                if (!isdigit(in_->peek())) {
                    break;
                }
            }
            last_token_ = ConstantToken(stoll(str));
            return;
        }

        if (isalpha(fch) || SymbolToken::kCharsFirst.find(fch) != std::string::npos) {  // Symbol
            std::string str;
            for (int ch = fch;; ch = in_->get()) {
                str += ch;
                if (!std::isalnum(in_->peek()) &&
                    SymbolToken::kCharsRest.find(in_->peek()) == std::string::npos) {
                    break;
                }
            }

            last_token_ = SymbolToken(str);
            return;
        }

        throw SyntaxError("Bad token");
    }

    Token GetToken() {
        return last_token_;
    }

private:
    std::istream* in_;
    Token last_token_;
    bool is_ended_;
};