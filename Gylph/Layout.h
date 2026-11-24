#pragma once
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
#include "Style.h"

struct EdgeSize {
	float left = 0.f;
	float right = 0.f;
	float top = 0.f;
	float bottom = 0.f;
};

struct Rect {
	float x = 0.f;
	float y = 0.f;
	float width = 0.f;
	float height = 0.f;

	Rect ExpandedBy(EdgeSize edge) const {
		return {
			.x = x - edge.left,
			.y = y - edge.top,
			.width = width + edge.left + edge.right,
			.height = height + edge.top + edge.bottom,
		};
	}
};

struct Dimension {
	Rect content;

	EdgeSize padding;
	EdgeSize border;
	EdgeSize margin;

	// Area of content + padding
	Rect PaddingBox() const {
		return content.ExpandedBy(padding);
	}

	// Area of content + padding + borders
	Rect BorderBox() const {
		return PaddingBox().ExpandedBy(border);
	}

	// Area of content + padding + borders + margin
	Rect MarginBox() const {
		return BorderBox().ExpandedBy(margin);
	}
};

enum class BoxType : uint8_t {
	BlockNode,
	InlineNode,
	AnonymousBlock
};

struct LayoutBox {
	BoxType type = BoxType::BlockNode;
	Dimension dimension;
	const StyledNode* styledNode = nullptr;
	std::vector<LayoutBox> children;

	LayoutBox& GetInlineContainer() {
		switch (type) {
		case BoxType::BlockNode: {
			// Reuse trailing AnonymousBlock if present; otherwise create a new one
			if (!children.empty() && children.back().type == BoxType::AnonymousBlock) {
				return children.back();
			}

			LayoutBox& anon = children.emplace_back();
			anon.type = BoxType::AnonymousBlock;
			return anon;
		}
		case BoxType::AnonymousBlock:
		case BoxType::InlineNode:
			return *this;
		}

		return *this;
	}

	static std::optional<LayoutBox> BuildLayoutTree(const StyledNode* node);

	std::optional<Color> GetColor(const std::string& str) const {
		if (!styledNode) {
			return std::nullopt;
		}

		if (type == BoxType::BlockNode || type == BoxType::InlineNode) {
			if (auto value = styledNode->GetValue(str)) {
				if (auto color = std::get_if<Color>(&*value)) {
					return *color;
				}
			}
		}

		return std::nullopt;
	}

	void Layout(const Dimension& containingBlock) {
		switch (type) {
		case BoxType::BlockNode:
			LayoutBlock(containingBlock);
			break;
		case BoxType::AnonymousBlock:
			LayoutAnonymousBlock(containingBlock);
			break;
		case BoxType::InlineNode:
			// Inline layout pass goes here once inline/text flow is added
			break;
		}
	}

	void LayoutAnonymousBlock(const Dimension& containingBlock) {
		dimension.content.x = containingBlock.content.x;
		dimension.content.y = containingBlock.content.y + containingBlock.content.height;
		dimension.content.width = containingBlock.content.width;
		dimension.content.height = 0.f;

		for (auto& child : children) {
			child.Layout(dimension);
			dimension.content.height += child.dimension.MarginBox().height;
		}
	}

	void LayoutBlock(const Dimension& containingBlock) {
		dimension.content.height = 0.f;

		CalculateBlockWidth(containingBlock);
		CalculateBlockPosition(containingBlock);
		LayoutBlockChildren();
		CalculateBlockHeight(containingBlock);
	}

	void CalculateBlockWidth(const Dimension& containingBlock);

	void CalculateBlockHeight(const Dimension& containingBlock) {
		if (!styledNode) {
			return;
		}

		if (auto value = styledNode->GetValue("height")) {
			dimension.content.height = static_cast<float>(toPx(*value));
		}
	}

	void CalculateBlockPosition(const Dimension& containingBlock);

	void LayoutBlockChildren() {
		for (auto& child : children) {
			child.Layout(dimension);
			dimension.content.height += child.dimension.MarginBox().height;
		}
	}
};