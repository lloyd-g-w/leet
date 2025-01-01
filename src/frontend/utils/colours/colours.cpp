#include "colours.hpp"

namespace colours {
// -------------------- COLOUR CLASS --------------------
colour::colour(int r, int g, int b, float a) {
    data = {r, g, b, a};
}

// Colour actions
void colour::opacity(float opacity) {
    data.a = opacity;
}

void colour::set(int r, int g, int b) {
    data.r = r;
    data.g = g;
    data.b = b;
}

// Colour conversions
int colour::hex() {
    return (data.r << 16) + (data.g << 8) + data.b;
}

vec4 colour::vec4() {
    return colours::vec4(data.r, data.g, data.b, std::round(data.a * 255));
}

vec3 colour::vec3() {
    return colours::vec3(data.r, data.g, data.b);
}

unsigned int colour::imgui() {
    return (((unsigned int)(std ::round(data.a * 255)) << 24) |
            ((unsigned int)(data.b) << 16) | ((unsigned int)(data.g) << 8) |
            ((unsigned int)(data.r) << 0));
};

// -------------------- COLOUR_GROUP CLASS --------------------
palette::palette(std::initializer_list<preset> presets) {
    for (auto preset : presets) {
        add_preset(preset);
    }
}

void palette::add(std::string name, colour colour) {
    if (colours.find(name) != colours.end()) {
        return;
    }

    colours[name] = colour;
};

void palette::set(std::string name, colour colour) {
    colours[name] = colour;
};

void palette::remove(std::string name) {
    if (colours.find(name) == colours.end()) {
        return;
    }

    colours.erase(name);
};

colour palette::get(std::string name) {
    if (colours.find(name) == colours.end()) {
        return colour();
    }

    return colours[name];
};
void palette::add_preset(preset preset) {
    switch (preset) {
        case BASIC_COLOURS: palette::add_basic_preset(); break;
        case HOLY_SHEET: palette::add_holy_sheet(); break;
    }
}

void palette::add_basic_preset() {
    add("white", colour(255, 255, 255));
    add("black", colour(0, 0, 0));
    add("red", colour(255, 0, 0));
    add("green", colour(0, 255, 0));
    add("blue", colour(0, 0, 255));
    add("yellow", colour(255, 255, 0));
    add("cyan", colour(0, 255, 255));
    add("magenta", colour(255, 0, 255));
    add("grey", colour(128, 128, 128));
    add("light_grey", colour(192, 192, 192));
    add("dark_grey", colour(64, 64, 64));
    add("orange", colour(255, 165, 0));
    add("purple", colour(128, 0, 128));
    add("brown", colour(165, 42, 42));
    add("pink", colour(255, 192, 203));
    add("turquoise", colour(64, 224, 208));
    add("gold", colour(255, 215, 0));
    add("silver", colour(192, 192, 192));
    add("bronze", colour(205, 127, 50));
    add("maroon", colour(128, 0, 0));
    add("olive", colour(128, 128, 0));
    add("lime", colour(0, 255, 0));
    add("teal", colour(0, 128, 128));
    add("navy", colour(0, 0, 128));
    add("indigo", colour(75, 0, 130));
    add("violet", colour(238, 130, 238));
    add("peach", colour(255, 218, 185));
    add("apricot", colour(251, 206, 177));
    add("beige", colour(245, 245, 220));
    add("mint", colour(152, 251, 152));
    add("lavender", colour(230, 230, 250));
}

void palette::add_holy_sheet() {
    palette::add_basic_preset();
    add("active_blue", colour(0, 119, 182));
}
}  // namespace colours
