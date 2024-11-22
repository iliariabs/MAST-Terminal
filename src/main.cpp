#include "Node.h"
#include <iostream>

int main() {
     std::string expression = "2 + 10 + 52 *531";
    try {
        Node* tree = buildExpressionTree("2+2*2");
        tree->saveToJsonFile("expression.json");
        tree->renderTree("output.png");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
