#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <optional>



enum class Token : uint8_t {
	StartTag,       // <div>
	EndTag,         // </div>
	SelfClosingTag, // <br /> or <img />
	Text,           // Raw character text between tags
	Comment,        // <!-- comment -->
	Doctype,        // <!DOCTYPE html>
	EndOfFile       // Stream termination
};


struct TokenObject {
	Token type;
	std::string value;                                       // Holds tag name, text content, or comment text
	std::unordered_map<std::string, std::string> attributes; // Populated for StartTag and SelfClosingTag
};

class Tokenizator
{

public:
	Tokenizator(std::string content) : text(std::move(content)) {}

	std::vector<TokenObject> Tokenize() {

		std::vector<TokenObject> tokens;
		
		while (auto c = PeekChar()) {
			if (Match('<') && !Match('/', 1)) {
				tokens.push_back(ParseElementStart());
			}
			else if (Match('<') && Match('/', 1)) {
				tokens.push_back(ParseElementEnd());
			}
			else if (std::isspace(static_cast<unsigned char>(*c)))
			{
				ConsumeWhiteSpace();
			}
			else
			{
				tokens.push_back(ParseText());
			}
		}

		return tokens;
	}

private:

	
	std::string ParseName()
	{
		std::string name = "";
		while (auto c = PeekChar())
		{
			// Allow letters, numbers, hyphens, and underscores for tags and attributes
			if (std::isalnum(*c) || *c == '-' || *c == '_') {
				name.push_back(*c);
				Advance();
			}
			else {
				break;
			}
		}
		return name;
	}
	std::unordered_map<std::string, std::string> ParseAttributes()
	{
		std::unordered_map<std::string, std::string> attributes;

		while (true) {
			ConsumeWhiteSpace();

			// Stop if we hit the end of the tag (> or />) or EOF
			if (Match('>') || (Match('/') && Match('>', 1)) || !PeekChar().has_value()) {
				break;
			}

			auto name = ParseName();
			if (name.empty()) {
				break; // Safety check to prevent infinite loops on malformed HTML
			}

			ConsumeWhiteSpace();

			std::string value = "";
			// Check if this attribute has a value assignment
			if (Match('=')) {
				Expect('=');
				ConsumeWhiteSpace();

				auto quoteOpt = Advance();
				if (!quoteOpt.has_value()) break;

				char quote = *quoteOpt;
				if (quote == '"' || quote == '\'') {
					// Read characters until the matching closing quote
					while (auto c = PeekChar()) {
						if (*c == quote) {
							Advance(); // Consume the closing quote
							break;
						}
						value.push_back(*c);
						Advance();
					}
				}
			}

			attributes[name] = value;
		}

		return attributes;
	}

	//Parses string of text inside elements
	TokenObject ParseText() {
		auto n = PeekChar();
		std::string text = "";
		while (auto c = PeekChar()) {

			if (Match('<')) {
				break;
			}
			else if (std::isalnum(*c) || *c == '-' || *c == '_' || *c == ' ') {
				text.push_back(*c);
				Advance();
			}
			else {
				break;
			}
		}

		return TokenObject{Token::Text, text};
	}

	TokenObject ParseElementStart() {
		TokenObject object;
		Expect('<');
		auto tag = ParseName();
		auto attributesMap = ParseAttributes(); 

		if (Match('/') && Match('>', 1)) {
			Expect('/');
			object.type = Token::SelfClosingTag;
		}
		else {
			object.type = Token::StartTag;
		}

		Expect('>');

		object.value = tag;
		object.attributes = std::move(attributesMap);

		return object;
	}
	TokenObject ParseElementEnd() {

		TokenObject object;
		Expect('<'); 
		Expect('/');
		auto tag = ParseName();
		Expect('>');

		object.value = tag;
		object.type = Token::EndTag;
		return object;
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

	std::optional<char> Advance()
	{
		if (_index < text.length())
		{
			return text.at(_index++);
		}

		return {};
	}

	std::optional<char> PeekChar(size_t i = 0) {
		if (_index + i >= text.length())
		{
			return {};
		}
		return text.at(i + _index);
	}
	bool Match(char c, size_t offset = 0) {
		if (auto value = PeekChar(offset))
		{
			if (*value == c) return true;
		}

		return false;
	}
	void Expect(char c);

	
	size_t _index = 0;
	std::string text;
};