#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <unordered_set>
struct Node;

struct TextNode {
	std::string content;
};

struct ElementNode {
	std::string tagName;
	std::unordered_map<std::string, std::string> attributes;
	std::vector<Node*> children; 

	std::optional<std::string> GetID() const {
		auto it = attributes.find("id");

		if (it != attributes.end()) {
			return it->second;
		}
		return std::nullopt;
	}

	std::optional<std::unordered_set<std::string>> GetClasses() const {
		std::unordered_set<std::string> set;
		
		auto it = attributes.find("class");

		if (it != attributes.end()) {

			std::string value = "";
			for (auto c : it->second) {
				value += c;
				if (c == ' ' && !value.empty()) {
					set.insert(value);
					value = "";
				}
			}
			if (!value.empty()) {
				set.insert(value);
			}
			if (!set.empty()) {
				return { set };
			}
		}
		

		return std::nullopt;
	}
};

// Element or text node
struct Node {
	std::variant<TextNode, ElementNode> data;
};