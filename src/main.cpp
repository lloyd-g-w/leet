#include "frontend/application.hpp"

int main() {
    app holysheet = app("Holy Sheet", 1920, 1080, 50, 50);

    holysheet.run();

    return 0;
}
