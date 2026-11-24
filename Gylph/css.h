#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <variant>
#include <vector>

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255; 
};

struct Length {
    double value = 0.0f;
    std::string unit; // "px", "em", "rem", "%"
};

struct Keyword {
    std::string name; // "block", "inline", "none", "auto"
};

using Value = std::variant<Color, Length, Keyword, std::string>;


double toPx(const Value& val, double autoVal = 0.0);

struct Declaration {
    std::string name;
    Value value;
};


struct Selector {
    std::optional<std::string> tagName; // "div", "p", or std::nullopt
    std::optional<std::string> id;      // "main" from #main
    std::vector<std::string> classes;   // {"btn", "primary"} from .btn.primary

    // Returns specificity tuple in priority order: (id, class, tag)
    std::tuple<int, int, int> specificity() const {
        return {
            static_cast<int>(id.has_value()),
            static_cast<int>(classes.size()),
            static_cast<int>(tagName.has_value())
        };
    }
};

struct Rule {
    std::vector<Selector> selectors;
    std::vector<Declaration> declarations;
};

struct StyleSheet {
    std::vector<Rule> rules;
};



class CSSParser {
public:

    CSSParser(std::string str) : text(std::move(str)){}
    StyleSheet GetStyleSheet() {

        std::vector<Rule> rules;

        while (auto c = PeekChar()) {
            rules.push_back(std::move(ParseRule()));
        }

        return StyleSheet{ std::move(rules) };
    }
    
private:

    // Parses a single <selector> : declaration rule
    Rule ParseRule();
    //Parses a single selector
    std::optional<Selector> ParseSelector();
    //Parses comma-separated selectors
    std::vector<Selector> ParseSelectors();
    std::vector<Declaration> ParseDeclarations();
    
    std::optional<Declaration> ParseDeclaration();

   
    std::string ParseIdentifier() {
        std::string identifier = "";

        while (auto c = PeekChar()) {

            if (ValidIdentifierChar(*c)) {
                identifier += *c;
                Advance();
            }
            else {
                break;
            }
        }

        return identifier;
    }

    std::optional<Length> ParseLength();
    std::optional<char> PeekChar(size_t offset = 0) {
        if (offset + _index < text.size()) {
            return text[offset + _index];
        }
        return {};
    }
    bool Advance() {
        if (_index < text.size()) {
            _index++;
            return true;
        }
        return false;
    }
    bool ValidIdentifierChar(char c) const {
        return (std::isalnum(c) || c == '-' || c == '_' || c =='#');
    }
    void ConsumeWhiteSpace() {
        while (auto c = PeekChar()) {
            if (std::isspace(static_cast<unsigned char>(*c))) {
                Advance();
            }
            else {
                break;
            }
        }
    }


    bool Consume(char c);
    std::string text;

    size_t _index = 0;
};