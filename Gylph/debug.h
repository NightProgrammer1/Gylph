#pragma once
#include "parser.h"
#include <iostream>

void PrintDom(const Node* node, int depth = 0) {
	if (!node) return;

	std::string indent(static_cast<size_t>(depth) * 2, ' ');

	std::visit([&](const auto& payload) {
		using T = std::decay_t<decltype(payload)>;

		if constexpr (std::is_same_v<T, TextNode>) { //If text
			std::cout << indent << "\"" << payload.content << "\"\n";
		}
		else if constexpr (std::is_same_v<T, ElementNode>) { // If element
			std::cout << indent << "<" << payload.tagName;
			for (const auto& [key, val] : payload.attributes) {
				std::cout << " " << key << "=\"" << val << "\"";
			}
			std::cout << ">\n";

			for (const Node* child : payload.children) {
				PrintDom(child, depth + 1);
			}
		}
		}, node->data);
}