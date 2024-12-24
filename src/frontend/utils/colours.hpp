#include <cmath>
#include <string>
#include <unordered_map>

namespace colour_space {
class colour;
class colour_group;

// Standard types
typedef std::string str;
typedef std::unordered_map<str, colour> colour_map;
struct vec3 {
    int a, b, c;
    vec3(int a = 0, int b = 0, int c = 0) : a(a), b(b), c(c) {}
};
struct vec4 {
    int a, b, c, d;
    vec4(int a = 0, int b = 0, int c = 0, int d = 0) : a(a), b(b), c(c), d(d) {}
};

class colour {
  public:
    colour(int r = 255, int g = 255, int b = 255, float a = 1.0f);

    // Actions
    void opacity(float opacity);
    void set(int r, int g, int b);

    // Converters / Getters
    int hex();
    colour_space::vec3 vec3();
    colour_space::vec4 vec4();
    unsigned int imgui();

  private:
    struct colour_data {
        int r, g, b;
        float a;
    } data;

    friend class colour_group;
};

class colour_group {
  public:
    enum preset {
        BASIC_COLOURS,
        HOLY_SHEET
    };

    colour_group(std::initializer_list<preset> presets = {});

    // Actions
    void add(str name, colour colour);
    void set(str name, colour colour);
    void remove(str name);
    colour get(str name);
    void add_preset(preset preset);

  private:
    colour_map colours;
    void add_basic_preset();
    void add_holy_sheet();
};
}  // namespace colour_space
