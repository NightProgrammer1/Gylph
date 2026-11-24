#pragma once
#include <string_view>
#include <unordered_map>
#include "css.h" 

inline const std::unordered_map<std::string_view, Color> kNamedColors = {
    { "aliceblue",            Color{ 240, 248, 255, 255 } },
    { "antiquewhite",         Color{ 250, 235, 215, 255 } },
    { "aqua",                 Color{ 0, 255, 255, 255 } },
    { "aquamarine",           Color{ 127, 255, 212, 255 } },
    { "azure",                Color{ 240, 255, 255, 255 } },
    { "beige",                Color{ 245, 245, 220, 255 } },
    { "bisque",               Color{ 255, 228, 196, 255 } },
    { "black",                Color{ 0, 0, 0, 255 } },
    { "blanchedalmond",       Color{ 255, 235, 205, 255 } },
    { "blue",                 Color{ 0, 0, 255, 255 } },
    { "blueviolet",           Color{ 138, 43, 226, 255 } },
    { "brown",                Color{ 165, 42, 42, 255 } },
    { "burlywood",            Color{ 222, 184, 135, 255 } },
    { "cadetblue",            Color{ 95, 158, 160, 255 } },
    { "chartreuse",           Color{ 127, 255, 0, 255 } },
    { "chocolate",            Color{ 210, 105, 30, 255 } },
    { "coral",                Color{ 255, 127, 80, 255 } },
    { "cornflowerblue",       Color{ 100, 149, 237, 255 } },
    { "cornsilk",             Color{ 255, 248, 220, 255 } },
    { "crimson",              Color{ 220, 20, 60, 255 } },
    { "cyan",                 Color{ 0, 255, 255, 255 } },
    { "darkblue",             Color{ 0, 0, 139, 255 } },
    { "darkcyan",             Color{ 0, 139, 139, 255 } },
    { "darkgoldenrod",        Color{ 184, 134, 11, 255 } },
    { "darkgray",             Color{ 169, 169, 169, 255 } },
    { "darkgreen",            Color{ 0, 100, 0, 255 } },
    { "darkgrey",             Color{ 169, 169, 169, 255 } },
    { "darkkhaki",            Color{ 189, 183, 107, 255 } },
    { "darkmagenta",          Color{ 139, 0, 139, 255 } },
    { "darkolivegreen",       Color{ 85, 107, 47, 255 } },
    { "darkorange",           Color{ 255, 140, 0, 255 } },
    { "darkorchid",           Color{ 153, 50, 204, 255 } },
    { "darkred",              Color{ 139, 0, 0, 255 } },
    { "darksalmon",           Color{ 233, 150, 122, 255 } },
    { "darkseagreen",         Color{ 143, 188, 143, 255 } },
    { "darkslateblue",        Color{ 72, 61, 139, 255 } },
    { "darkslategray",        Color{ 47, 79, 79, 255 } },
    { "darkslategrey",        Color{ 47, 79, 79, 255 } },
    { "darkturquoise",        Color{ 0, 206, 209, 255 } },
    { "darkviolet",           Color{ 148, 0, 211, 255 } },
    { "deeppink",             Color{ 255, 20, 147, 255 } },
    { "deepskyblue",          Color{ 0, 191, 255, 255 } },
    { "dimgray",              Color{ 105, 105, 105, 255 } },
    { "dimgrey",              Color{ 105, 105, 105, 255 } },
    { "dodgerblue",           Color{ 30, 144, 255, 255 } },
    { "firebrick",            Color{ 178, 34, 34, 255 } },
    { "floralwhite",          Color{ 255, 250, 240, 255 } },
    { "forestgreen",          Color{ 34, 139, 34, 255 } },
    { "fuchsia",              Color{ 255, 0, 255, 255 } },
    { "gainsboro",            Color{ 220, 220, 220, 255 } },
    { "ghostwhite",           Color{ 248, 248, 255, 255 } },
    { "gold",                 Color{ 255, 215, 0, 255 } },
    { "goldenrod",            Color{ 218, 165, 32, 255 } },
    { "gray",                 Color{ 128, 128, 128, 255 } },
    { "green",                Color{ 0, 128, 0, 255 } },
    { "greenyellow",          Color{ 173, 255, 47, 255 } },
    { "grey",                 Color{ 128, 128, 128, 255 } },
    { "honeydew",             Color{ 240, 255, 240, 255 } },
    { "hotpink",              Color{ 255, 105, 180, 255 } },
    { "indianred",            Color{ 205, 92, 92, 255 } },
    { "indigo",               Color{ 75, 0, 130, 255 } },
    { "ivory",                Color{ 255, 255, 240, 255 } },
    { "khaki",                Color{ 240, 230, 140, 255 } },
    { "lavender",             Color{ 230, 230, 250, 255 } },
    { "lavenderblush",        Color{ 255, 240, 245, 255 } },
    { "lawngreen",            Color{ 124, 252, 0, 255 } },
    { "lemonchiffon",         Color{ 255, 250, 205, 255 } },
    { "lightblue",            Color{ 173, 216, 230, 255 } },
    { "lightcoral",           Color{ 240, 128, 128, 255 } },
    { "lightcyan",            Color{ 224, 255, 255, 255 } },
    { "lightgoldenrodyellow", Color{ 250, 250, 210, 255 } },
    { "lightgray",            Color{ 211, 211, 211, 255 } },
    { "lightgreen",           Color{ 144, 238, 144, 255 } },
    { "lightgrey",            Color{ 211, 211, 211, 255 } },
    { "lightpink",            Color{ 255, 182, 193, 255 } },
    { "lightsalmon",          Color{ 255, 160, 122, 255 } },
    { "lightseagreen",        Color{ 32, 178, 170, 255 } },
    { "lightskyblue",         Color{ 135, 206, 250, 255 } },
    { "lightslategray",       Color{ 119, 136, 153, 255 } },
    { "lightslategrey",       Color{ 119, 136, 153, 255 } },
    { "lightsteelblue",       Color{ 176, 196, 222, 255 } },
    { "lightyellow",          Color{ 255, 255, 224, 255 } },
    { "lime",                 Color{ 0, 255, 0, 255 } },
    { "limegreen",            Color{ 50, 205, 50, 255 } },
    { "linen",                Color{ 250, 240, 230, 255 } },
    { "magenta",              Color{ 255, 0, 255, 255 } },
    { "maroon",               Color{ 128, 0, 0, 255 } },
    { "mediumaquamarine",     Color{ 102, 205, 170, 255 } },
    { "mediumblue",           Color{ 0, 0, 205, 255 } },
    { "mediumorchid",         Color{ 186, 85, 211, 255 } },
    { "mediumpurple",         Color{ 147, 112, 219, 255 } },
    { "mediumseagreen",       Color{ 60, 179, 113, 255 } },
    { "mediumslateblue",      Color{ 123, 104, 238, 255 } },
    { "mediumspringgreen",    Color{ 0, 250, 154, 255 } },
    { "mediumturquoise",      Color{ 72, 209, 204, 255 } },
    { "mediumvioletred",      Color{ 199, 21, 133, 255 } },
    { "midnightblue",         Color{ 25, 25, 112, 255 } },
    { "mintcream",            Color{ 245, 255, 250, 255 } },
    { "mistyrose",            Color{ 255, 228, 225, 255 } },
    { "moccasin",             Color{ 255, 228, 181, 255 } },
    { "navajowhite",          Color{ 255, 222, 173, 255 } },
    { "navy",                 Color{ 0, 0, 128, 255 } },
    { "oldlace",              Color{ 253, 245, 230, 255 } },
    { "olive",                Color{ 128, 128, 0, 255 } },
    { "olivedrab",            Color{ 107, 142, 35, 255 } },
    { "orange",               Color{ 255, 165, 0, 255 } },
    { "orangered",            Color{ 255, 69, 0, 255 } },
    { "orchid",               Color{ 218, 112, 214, 255 } },
    { "palegoldenrod",        Color{ 238, 232, 170, 255 } },
    { "palegreen",            Color{ 152, 251, 152, 255 } },
    { "paleturquoise",        Color{ 175, 238, 238, 255 } },
    { "palevioletred",        Color{ 219, 112, 147, 255 } },
    { "papayawhip",           Color{ 255, 239, 213, 255 } },
    { "peachpuff",            Color{ 255, 218, 185, 255 } },
    { "peru",                 Color{ 205, 133, 63, 255 } },
    { "pink",                 Color{ 255, 192, 203, 255 } },
    { "plum",                 Color{ 221, 160, 221, 255 } },
    { "powderblue",           Color{ 176, 224, 230, 255 } },
    { "purple",               Color{ 128, 0, 128, 255 } },
    { "rebeccapurple",        Color{ 102, 51, 153, 255 } },
    { "red",                  Color{ 255, 0, 0, 255 } },
    { "rosybrown",            Color{ 188, 143, 143, 255 } },
    { "royalblue",            Color{ 65, 105, 225, 255 } },
    { "saddlebrown",          Color{ 139, 69, 19, 255 } },
    { "salmon",               Color{ 250, 128, 114, 255 } },
    { "sandybrown",           Color{ 244, 164, 96, 255 } },
    { "seagreen",             Color{ 46, 139, 87, 255 } },
    { "seashell",             Color{ 255, 245, 238, 255 } },
    { "sienna",               Color{ 160, 82, 45, 255 } },
    { "silver",               Color{ 192, 192, 192, 255 } },
    { "skyblue",              Color{ 135, 206, 235, 255 } },
    { "slateblue",            Color{ 106, 90, 205, 255 } },
    { "slategray",            Color{ 112, 128, 144, 255 } },
    { "slategrey",            Color{ 112, 128, 144, 255 } },
    { "snow",                 Color{ 255, 250, 250, 255 } },
    { "springgreen",          Color{ 0, 255, 127, 255 } },
    { "steelblue",            Color{ 70, 130, 180, 255 } },
    { "tan",                  Color{ 210, 180, 140, 255 } },
    { "teal",                 Color{ 0, 128, 128, 255 } },
    { "thistle",              Color{ 216, 191, 216, 255 } },
    { "tomato",               Color{ 255, 99, 71, 255 } },
    { "turquoise",            Color{ 64, 224, 208, 255 } },
    { "violet",               Color{ 238, 130, 238, 255 } },
    { "wheat",                Color{ 245, 222, 179, 255 } },
    { "white",                Color{ 255, 255, 255, 255 } },
    { "whitesmoke",           Color{ 245, 245, 245, 255 } },
    { "yellow",               Color{ 255, 255, 0, 255 } },
    { "yellowgreen",          Color{ 154, 205, 50, 255 } },
    { "transparent",          Color{ 0, 0, 0, 0 } },
};

// Return a rgba color code for a string
 std::optional<Color> ResolveNamedColor(std::string_view name) {
    if (auto it = kNamedColors.find(name); it != kNamedColors.end()) {
        return it->second;
    }
    return std::nullopt;
}

 namespace {

     std::optional<uint8_t> HexDigitValue(char c) {
         unsigned char uc = static_cast<unsigned char>(c);
         if (uc >= '0' && uc <= '9') return static_cast<uint8_t>(uc - '0');
         if (uc >= 'a' && uc <= 'f') return static_cast<uint8_t>(10 + (uc - 'a'));
         if (uc >= 'A' && uc <= 'F') return static_cast<uint8_t>(10 + (uc - 'A'));
         return std::nullopt;
     }

     std::optional<uint8_t> ParseHexByte(std::string_view hex, std::size_t pos) {
         auto hi = HexDigitValue(hex[pos]);
         auto lo = HexDigitValue(hex[pos + 1]);
         if (!hi || !lo) return std::nullopt;
         return static_cast<uint8_t>((*hi << 4) | *lo);
     }

     std::optional<uint8_t> ParseExpandedNibble(std::string_view hex, std::size_t pos) {
         auto d = HexDigitValue(hex[pos]);
         if (!d) return std::nullopt;
         // 0xf -> 0xff (255), 0x8 -> 0x88 (136)
         return static_cast<uint8_t>((*d << 4) | *d);
     }

 } 

 // Expects input with or without a leading '#'
 std::optional<Color> ParseHexColor(std::string_view input) {
     if (!input.empty() && input.front() == '#') {
         input.remove_prefix(1);
     }

     if (input.size() == 3 || input.size() == 4) {
         auto r = ParseExpandedNibble(input, 0);
         auto g = ParseExpandedNibble(input, 1);
         auto b = ParseExpandedNibble(input, 2);
         if (!r || !g || !b) return std::nullopt;

         uint8_t a = 255;
         if (input.size() == 4) {
             auto alpha = ParseExpandedNibble(input, 3);
             if (!alpha) return std::nullopt;
             a = *alpha;
         }
         return Color{ *r, *g, *b, a };
     }

     if (input.size() == 6 || input.size() == 8) {
         auto r = ParseHexByte(input, 0);
         auto g = ParseHexByte(input, 2);
         auto b = ParseHexByte(input, 4);
         if (!r || !g || !b) return std::nullopt;

         uint8_t a = 255;
         if (input.size() == 8) {
             auto alpha = ParseHexByte(input, 6);
             if (!alpha) return std::nullopt;
             a = *alpha;
         }
         return Color{ *r, *g, *b, a };
     }

     return std::nullopt;
 }