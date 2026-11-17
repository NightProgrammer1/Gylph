#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

struct Node;

struct TextNode {
	std::string content;
};

struct ElementNode {
	std::string tagName;
	std::unordered_map<std::string, std::string> attributes;
	std::vector<Node*> children; 
};

struct Node {
	std::variant<TextNode, ElementNode> data;
};