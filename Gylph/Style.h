#pragma once
#include "parser.h"
#include "css.h"
#include <algorithm>

using PropertyMap = std::unordered_map<std::string, Value>;
					//Specificity
using MatchedRule = std::pair<std::tuple<int, int, int>, Rule>;


enum class Display : uint8_t {
	Inline,
	Block,
	None,
};

struct StyledNode {
	Node* node = nullptr;
	PropertyMap properties;
	std::vector<StyledNode> children;

	std::optional<Value> GetValue(const std::string& str) const {
		auto it = properties.find(str);

		if (it != properties.end()) {
			return it->second;
		}

		return std::nullopt;
	}

	//Defaults to inline
	Display GetDisplay() const {
		if (auto display = GetValue("display")) {
			if (auto val = std::get_if<Keyword>(&(*display))) {
				if (val->name == "block") {
					return Display::Block;
				}
				if (val->name == "none") {
					return Display::None;
				}
			}
		}

		return Display::Inline;
	}

	//lookup two options consecutivly if not found return value
	Value LookUp(const std::string& option1, const std::string& option2, const Value& value) const {

		if (auto first = GetValue(option1)) {
			return *first;
		}
		if (auto second = GetValue(option2)) {
			return *second;
		}
		return value;
	}

};


class Style
{
public:
	StyledNode BuildTree(Node* node, const StyleSheet& style) {
		StyledNode tree;
		tree.node = node;

		//If root is a node Element
		if (auto root = std::get_if<ElementNode>(&node->data)) { 
			tree.properties = StyleElement(*root, style);
			for (auto& child : root->children) {
				tree.children.push_back(std::move(BuildTree(child, style)));
			}
		}
		return tree;
	}

private:
	//check if an elementNode matches with a selector
	bool Matches(const ElementNode& node, const Selector& selec) const {
		//Check each components until one matches
		if (selec.tagName && node.tagName != *selec.tagName) {
			return false;
		}
		if (selec.id) {
			auto nodeId = node.GetID();
			if (!nodeId || *nodeId != *selec.id) {
				return false;
			}
		}
		if (!selec.classes.empty()) {
			auto nodeClasses = node.GetClasses();
			if (!nodeClasses) {
				return false;
			}
			for (const auto& cl : selec.classes) {
				if (!nodeClasses->contains(cl)) {
					return false;
				}
			}
		}
		return true; // succesfully matched
	}
	//Find using the highest specificity selector if a rule matches with a node Element
	std::optional<MatchedRule> MatchRule(const ElementNode& node, const Rule& rule) const {
		
		auto it = std::find_if(rule.selectors.begin(), rule.selectors.end(), [this, node](const auto& selec) { return Matches(node, selec); });

		if (it != rule.selectors.end()) {
			return MatchedRule{ it->specificity(), rule };
		}

		return std::nullopt;
		
	}

	//Find all the rules that matches an node Element 
	std::vector<MatchedRule> MatchRules(const ElementNode& node, const StyleSheet& style) const {
		std::vector<MatchedRule> matched;

		for (const auto& rule : style.rules) {
			if (auto m = MatchRule(node, rule)) {
				matched.push_back(std::move(*m));
			}
		}

		return matched;
	}

	//Apply style to a node Element and return the property map 
	PropertyMap StyleElement(const ElementNode& node, const StyleSheet& style) const{
		PropertyMap values;

		auto rules = MatchRules(node, style);
		// Sort from lowest to highest specificity
		std::sort(rules.begin(), rules.end(), [](auto& a, auto& b) { if (a.first < b.first) { return true; } return false; });

		for (const auto& rule : rules) {
			for (auto& decl : rule.second.declarations) {
				values[decl.name] = decl.value;
			}
		}
		return values;
	}
};