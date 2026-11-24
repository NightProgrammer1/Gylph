#include "css.h"
#include <iostream>
#include <tuple>
#include <algorithm>
#include "css_colors.h"


std::optional<Selector> CSSParser::ParseSelector() {
    Selector selector;
    bool hasPart = false;

    while (auto c = PeekChar()) {
        if (*c == '#') {
            Advance(); // consume '#'
            auto id = ParseIdentifier();
            if (!id.empty()) {
                selector.id = std::move(id);
                hasPart = true;
            }
            else {
                return std::nullopt;
            }
        }
        else if (*c == '.') {
            Advance(); // consume '.'
            auto cls = ParseIdentifier();
            if (!cls.empty()) {
                selector.classes.push_back(std::move(cls));
                hasPart = true;
            }
            else {
                return std::nullopt;
            }
        }
        else if (*c == '*') {
            Advance(); // consume '*'
            hasPart = true;
        }
        else if (ValidIdentifierChar(*c)) {
            auto tag = ParseIdentifier();
            if (!tag.empty()) {
                selector.tagName = std::move(tag);
                hasPart = true;
            }
        }
        else {
            break; // Stop on '{', ',', whitespace, etc.; let caller handle it
        }
    }

    if (!hasPart) {
        return std::nullopt;
    }
    return selector;
}

std::vector<Selector> CSSParser::ParseSelectors() {
    std::vector<Selector> selectors;
    ConsumeWhiteSpace();

    while (auto current = PeekChar()) {
        if (auto selec = ParseSelector()) {
            selectors.push_back(std::move(*selec));
        }

        ConsumeWhiteSpace();

        auto next = PeekChar();
        if (!next) {
            break;
        }

        if (*next == ',') {
            Advance(); // consume ','
            ConsumeWhiteSpace();
        }
        else if (*next == '{') {
            break; // leave '{' for ParseRule to consume
        }
        else {
            std::cerr << "Unexpected character '" << *next << "' in selector list\n";
            Advance(); // make forward progress so we never loop infinitely
        }
    }

    std::sort(selectors.begin(), selectors.end(), [](auto& a, auto& b) { if (a.specificity() > b.specificity()) { return true; } return false; });
    return selectors;
}

std::vector<Declaration> CSSParser::ParseDeclarations() {
    std::vector<Declaration> declarations;

    while (true) {
        ConsumeWhiteSpace();

        auto c = PeekChar();
        if (!c) {
            break;
        }
        if (*c == '}') {
            Advance(); // consume closing '}' of the rule block
            break;
        }

        size_t before = _index;
        if (auto decl = ParseDeclaration()) {
            declarations.push_back(std::move(*decl));
        }

        // Safety guard: guarantee forward progress even on unexpected input
        if (_index == before && PeekChar()) {
            Advance();
        }
    }

    return declarations;
}

std::optional<Declaration> CSSParser::ParseDeclaration() {
    ConsumeWhiteSpace();

    auto c = PeekChar();
    if (!c || *c == '}') {
        return std::nullopt;
    }

    // Handle empty/stray semicolons gracefully
    if (*c == ';') {
        Advance();
        return std::nullopt;
    }

    std::string property = ParseIdentifier();
    if (property.empty()) {
        // Skip forward until ';' or '}' to recover
        while (auto ch = PeekChar()) {
            if (*ch == ';' || *ch == '}') break;
            Advance();
        }
        if (auto ch = PeekChar(); ch && *ch == ';') {
            Advance();
        }
        return std::nullopt;
    }

    ConsumeWhiteSpace();
    if (!Consume(':')) {
        // Malformed declaration: skip to next ';' or '}'
        while (auto ch = PeekChar()) {
            if (*ch == ';' || *ch == '}') break;
            Advance();
        }
        if (auto ch = PeekChar(); ch && *ch == ';') {
            Advance();
        }
        return std::nullopt;
    }

    ConsumeWhiteSpace();

    Declaration decl;
    decl.name = property;

    if (property == "color" || property == "background") {
        std::string valIdent = ParseIdentifier();

      
        if (!valIdent.empty()) {
            // To lowercase
            std::transform(valIdent.begin(), valIdent.end(), valIdent.begin(),
                [](unsigned char c) { return std::tolower(c); });
            if (valIdent.at(0) == '#') {
                if (auto color = ParseHexColor(valIdent)) {
                    decl.value = *color;
                }
                else {
                    decl.value = Keyword{ valIdent };
                }
            }
            else
            {
                if (auto color = ResolveNamedColor(valIdent)) {
                    decl.value = *color;
                }
                else {
                    decl.value = Keyword{ valIdent };
                }
            }
        }
        
        
    }
    else if (property == "display") {
        std::string valIdent = ParseIdentifier();
        if (!valIdent.empty()) {
            decl.value = Keyword{ valIdent };
        }
        else {
            // Skip to ';' or '}' if malformed
            while (auto ch = PeekChar()) {
                if (*ch == ';' || *ch == '}') break;
                Advance();
            }
            return std::nullopt;
        }
    }
    else if (property == "padding") {
        if (auto len = ParseLength()) {
            decl.value = *len;
        }
        else {
            while (auto ch = PeekChar()) {
                if (*ch == ';' || *ch == '}') break;
                Advance();
            }
            return std::nullopt;
        }
    }
    else {
        // Placeholder until other properties/values are added
        std::string rawVal;
        while (auto ch = PeekChar()) {
            if (*ch == ';' || *ch == '}') break;
            rawVal.push_back(*ch);
            Advance();
        }
        decl.value = rawVal;
    }
    return decl;

}

bool CSSParser::Consume(char expected) {
    auto ch = PeekChar();
    if (!ch) {
        std::cerr << "Expected '" << expected << "' but reached end of input\n";
        return false;
    }
    if (*ch != expected) {
        std::cerr << "Expected '" << expected << "' but got '" << *ch << "'\n";
        return false;
    }
    Advance();
    return true;
}

Rule CSSParser::ParseRule() {
    auto selectors = ParseSelectors();
    ConsumeWhiteSpace();

    if (!Consume('{')) {
        // Recover forward to next '}' or ';' 
        while (auto ch = PeekChar()) {
            Advance();
            if (*ch == '}' || *ch == ';') {
                break;
            }
        }
        return Rule{ std::move(selectors),{} };
    }

    auto declarations = ParseDeclarations();
    return Rule{ std::move(selectors), std::move(declarations) };
}


std::optional<Length> CSSParser::ParseLength() {
    ConsumeWhiteSpace();

    std::string numStr;
    bool seenDot = false;

    if (auto c = PeekChar(); c && (*c == '+' || *c == '-')) {
        numStr.push_back(*c);
        Advance();
    }

    while (auto c = PeekChar()) {
        if (std::isdigit(static_cast<unsigned char>(*c))) {
            numStr.push_back(*c);
            Advance();
        }
        else if (*c == '.' && !seenDot) {
            seenDot = true;
            numStr.push_back(*c);
            Advance();
        }
        else {
            break;
        }
    }

    if (numStr.empty() || numStr == "+" || numStr == "-" || numStr == ".") {
        return std::nullopt;
    }

    std::string unit;
    while (auto c = PeekChar()) {
        if (std::isalpha(static_cast<unsigned char>(*c)) || *c == '%') {
            unit.push_back(*c);
            Advance();
        }
        else {
            break;
        }
    }

    double value = 0.0;
    try {
        value = std::stod(numStr);
    }
    catch (...) {
        return std::nullopt;
    }

    // Allow unitless 0 in CSS, otherwise require a unit like "px"
    if (unit.empty()) {
        if (value == 0.0) {
            unit = "px";
        }
        else {
            return std::nullopt;
        }
    }

    return Length{ value, unit };
}
double toPx(const Value& val, double autoVal)
{
    if (const auto* len = std::get_if<Length>(&val)) {
        return len->value;
    }
    if (const auto* kw = std::get_if<Keyword>(&val)) {
        if (kw->name == "auto") {
            return autoVal;
        }
    }
    return 0.0;
};
