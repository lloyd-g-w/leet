#include "frontend/application.hpp"

int main() {
    app appl = app("Lead", 1920, 1080, 50, 50);

    appl.run();

    return 0;
}
