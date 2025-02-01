#include <iostream>
#include <string>

#include "../tokenizer/tokenizer.cpp"

using namespace std;

int main(int argc, char *argv[]) {
    string input = string(argv[1]);
    std::cout << "Input: " << input << std::endl;

    auto res = std_cells::tokenize(input);
    std::cout << res.size() << " tokens: " << std::endl;
    std::cout << '[';
    cout << '"' << res.front().value << '"';
    res.erase(res.begin());
    for (auto token : res) {
        cout << ", ";
        cout << '"' << token.value << '"';
    }
    std::cout << ']' << std::endl;
    return 0;
}
