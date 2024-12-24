#include <cmath>
#include <string>
#include <unordered_map>

#include "imgui.h"

class Colour {
  public:
    struct colour_data {
        int r, g, b;
        float a;
    };

    colour_data value;

    Colour(int r = 255, int g = 255, int b = 255, float a = 1.0f) {
        value.r = r;
        value.g = g;
        value.b = b;
        value.a = a;
    }

    void opacity(float a) {
        value.a = a;
    }

    void set(int r, int g, int b) {
        value.r = r;
        value.g = g;
        value.b = b;
    }

    int hex() {
        return (value.r << 16) + (value.g << 8) + value.b;
    }
    ImU32 imu32() {
        return IM_COL32(value.r, value.g, value.b, std::round(value.a * 255));
    }

  private:
    std::string name;
};

class Colours {
  private:
    std::unordered_map<std::string, Colour> colours;

  public:
    void add(std::string name, Colour colour) {
        if (colours.find(name) != colours.end()) {
            return;
        }

        colours[name] = colour;
    }

    void set(std::string name, int r, int g, int b, float a = 1.0f) {
        colours[name].set(r, g, b);
    }

    void remove(std::string name) {
        if (colours.find(name) == colours.end()) {
            return;
        }

        colours.erase(name);
    }

    Colour get(std::string name) {
        if (colours.find(name) == colours.end()) {
            return Colour();
        }

        return colours[name];
    }

    enum ColourPresets {
        BASIC_COLOURS,
    };
    void add_preset(ColourPresets preset) {
        switch (preset) {
            case BASIC_COLOURS:
                add("white", Colour(255, 255, 255));
                add("black", Colour(0, 0, 0));
                add("red", Colour(255, 0, 0));
                add("green", Colour(0, 255, 0));
                add("blue", Colour(0, 119, 182));
                add("yellow", Colour(255, 255, 0));
                add("cyan", Colour(0, 255, 255));
                add("magenta", Colour(255, 0, 255));
                add("grey", Colour(128, 128, 128));
                add("light_grey", Colour(192, 192, 192));
                add("dark_grey", Colour(64, 64, 64));
                add("orange", Colour(255, 165, 0));
                add("purple", Colour(128, 0, 128));
                add("brown", Colour(165, 42, 42));
                add("pink", Colour(255, 192, 203));
                add("turquoise", Colour(64, 224, 208));
                add("gold", Colour(255, 215, 0));
                add("silver", Colour(192, 192, 192));
                add("bronze", Colour(205, 127, 50));
                add("maroon", Colour(128, 0, 0));
                add("olive", Colour(128, 128, 0));
                add("lime", Colour(0, 255, 0));
                add("teal", Colour(0, 128, 128));
                add("navy", Colour(0, 0, 128));
                add("indigo", Colour(75, 0, 130));
                add("violet", Colour(238, 130, 238));
                add("peach", Colour(255, 218, 185));
                add("apricot", Colour(251, 206, 177));
                add("beige", Colour(245, 245, 220));
                add("mint", Colour(152, 251, 152));
                add("lavender", Colour(230, 230, 250));
                break;
        }
    }
};
