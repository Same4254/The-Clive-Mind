#include "json.hpp"
#include <fstream>
#include <iomanip>

using json = nlohmann::json;

int main() {
    json j;
    j["pi"] = 3.1415;
    j["hello"] = "bye";

    std::ofstream outputStream("Test.json");
    outputStream << std::setw(4) << j;

    double a = 5;

    outputStream << a;
}