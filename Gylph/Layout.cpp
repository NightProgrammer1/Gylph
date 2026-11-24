#include "Layout.h"

namespace {

	bool IsAuto(const Value& v) {
		const auto* kw = std::get_if<Keyword>(&v);
		return kw && kw->name == "auto";
	}

	double PxOrZero(const Value& v) {
		if (IsAuto(v)) {
			return 0.0;
		}
		if (const auto* len = std::get_if<Length>(&v)) {
			return len->value;
		}
		return toPx(v);
	}

} 

std::optional<LayoutBox> LayoutBox::BuildLayoutTree(const StyledNode* node) {
	if (!node) {
		return std::nullopt;
	}

	Display display = node->GetDisplay();
	if (display == Display::None) {
		return std::nullopt;
	}

	LayoutBox root;
	root.styledNode = node;
	root.type = (display == Display::Inline) ? BoxType::InlineNode : BoxType::BlockNode;

	for (const auto& child : node->children) {
		Display childDisplay = child.GetDisplay();
		if (childDisplay == Display::None) {
			continue;
		}

		auto childBox = BuildLayoutTree(&child);
		if (!childBox) {
			continue;
		}

		if (childDisplay == Display::Block) {
			root.children.push_back(std::move(*childBox));
		}
		else if (childDisplay == Display::Inline) {
			root.GetInlineContainer().children.push_back(std::move(*childBox));
		}
	}

	return root;
}

void LayoutBox::CalculateBlockWidth(const Dimension& containingBlock) {
	if (!styledNode) {
		return;
	}

	// Width defaults to auto when omitted
	Value width = styledNode->GetValue("width").value_or(Value{ Keyword{ "auto" } });
	Value zero = Value{ Length{ 0.0, "px" } };

	Value marginLeft = styledNode->LookUp("margin-left", "margin", zero);
	Value marginRight = styledNode->LookUp("margin-right", "margin", zero);

	Value borderLeft = styledNode->LookUp("border-left-width", "border-width", zero);
	Value borderRight = styledNode->LookUp("border-right-width", "border-width", zero);

	Value paddingLeft = styledNode->LookUp("padding-left", "padding", zero);
	Value paddingRight = styledNode->LookUp("padding-right", "padding", zero);

	double total = PxOrZero(marginLeft)
		+ PxOrZero(marginRight)
		+ PxOrZero(borderLeft)
		+ PxOrZero(borderRight)
		+ PxOrZero(paddingLeft)
		+ PxOrZero(paddingRight)
		+ PxOrZero(width);

	// Negative underflow means the box overflows the containing block
	double underflow = static_cast<double>(containingBlock.content.width) - total;

	bool widthAuto = IsAuto(width);
	bool leftAuto = IsAuto(marginLeft);
	bool rightAuto = IsAuto(marginRight);

	if (!widthAuto && leftAuto && rightAuto) {
		if (underflow >= 0.0) {
			marginLeft = Value{ Length{ underflow * 0.5, "px" } };
			marginRight = Value{ Length{ underflow * 0.5, "px" } };
		}
		else {
			marginLeft = Value{ Length{ 0.0, "px" } };
			marginRight = Value{ Length{ underflow, "px" } };
		}
	}
	else if (!widthAuto && leftAuto && !rightAuto) {
		marginLeft = Value{ Length{ underflow, "px" } };
	}
	else if (!widthAuto && !leftAuto && rightAuto) {
		marginRight = Value{ Length{ underflow, "px" } };
	}
	else if (widthAuto) {
		if (leftAuto) { marginLeft = Value{ Length{ 0.0, "px" } }; }
		if (rightAuto) { marginRight = Value{ Length{ 0.0, "px" } }; }

		double nonWidthTotal = PxOrZero(marginLeft)
			+ PxOrZero(marginRight)
			+ PxOrZero(borderLeft)
			+ PxOrZero(borderRight)
			+ PxOrZero(paddingLeft)
			+ PxOrZero(paddingRight);

		double autoWidth = static_cast<double>(containingBlock.content.width) - nonWidthTotal;
		if (autoWidth >= 0.0) {
			width = Value{ Length{ autoWidth, "px" } };
		}
		else {
			width = Value{ Length{ 0.0, "px" } };
			marginRight = Value{ Length{ PxOrZero(marginRight) + autoWidth, "px" } };
		}
	}
	else if (!widthAuto && !leftAuto && !rightAuto) {
		marginRight = Value{ Length{ PxOrZero(marginRight) + underflow, "px" } };
	}

	dimension.content.width = static_cast<float>(PxOrZero(width));
	dimension.padding.left = static_cast<float>(PxOrZero(paddingLeft));
	dimension.padding.right = static_cast<float>(PxOrZero(paddingRight));
	dimension.border.left = static_cast<float>(PxOrZero(borderLeft));
	dimension.border.right = static_cast<float>(PxOrZero(borderRight));
	dimension.margin.left = static_cast<float>(PxOrZero(marginLeft));
	dimension.margin.right = static_cast<float>(PxOrZero(marginRight));
}

void LayoutBox::CalculateBlockPosition(const Dimension& containingBlock) {
	if (!styledNode) {
		return;
	}

	Value zero = Value{ Length{ 0.0, "px" } };

	dimension.margin.top = static_cast<float>(PxOrZero(styledNode->LookUp("margin-top", "margin", zero)));
	dimension.margin.bottom = static_cast<float>(PxOrZero(styledNode->LookUp("margin-bottom", "margin", zero)));

	dimension.border.top = static_cast<float>(PxOrZero(styledNode->LookUp("border-top-width", "border-width", zero)));
	dimension.border.bottom = static_cast<float>(PxOrZero(styledNode->LookUp("border-bottom-width", "border-width", zero)));

	dimension.padding.top = static_cast<float>(PxOrZero(styledNode->LookUp("padding-top", "padding", zero)));
	dimension.padding.bottom = static_cast<float>(PxOrZero(styledNode->LookUp("padding-bottom", "padding", zero)));

	dimension.content.x = containingBlock.content.x
		+ dimension.margin.left + dimension.border.left + dimension.padding.left;

	dimension.content.y = containingBlock.content.y + containingBlock.content.height
		+ dimension.margin.top + dimension.border.top + dimension.padding.top;
}