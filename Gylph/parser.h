#pragma once
#include "tokenization.h"
#include "dom.h"
#include <memory_resource>

class HTMLParser 
{
public:

	
	HTMLParser(const std::vector<TokenObject>& tokens, std::pmr::polymorphic_allocator<char>& allocator) : _tokens(tokens), _allocator(allocator) {}
	// Returns the dom representation of the html file
	std::vector<Node*> ParseNodes() {


		while (auto token = PeekToken()) {

			if (token->type == Token::EndTag) {
				continue;
			}
			_nodes.push_back(ParseNode(*token));
			Advance();
		}

		return _nodes;
	}
	

private:
	
	//Takes a TokenObject and return a node object
	Node* ParseNode(const TokenObject& object)
	{
		Node* node = nullptr;

		if (object.type == Token::StartTag)
		{
			node = _allocator.new_object<Node>(ElementNode{ object.value, object.attributes });
			Advance();
			ParseElement(node);
		}
		else if (object.type == Token::Text){
			node = _allocator.new_object<Node>(TextNode{ object.value});
		}
		return node;
	}

	//Parses a node element and populate it with children
	Node* ParseElement(Node* node) {
		if (!node) return nullptr;

		while (auto token = PeekToken()) {

			if (auto element = std::get_if<ElementNode>(&node->data)) { 
				if (token->type == Token::EndTag && element->tagName == token->value) { // EndTag break
					break;
				}
				element->children.push_back(ParseNode(*token));
			}
			Advance();
		}
		return node;
	}

	std::optional<TokenObject> PeekToken(int offset = 0 ) {
		if (_index + offset < _tokens.size()) {
			return _tokens.at(_index + offset);
		}

		return std::nullopt;
	}
	void Advance() {
		if (_index < _tokens.size())
		{
			_index++;
		}

	}

	std::vector<TokenObject> _tokens;
	std::vector<Node*> _nodes;

	std::pmr::polymorphic_allocator<char>& _allocator;

	size_t _index = 0;
};