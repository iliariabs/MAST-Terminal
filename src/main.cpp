#include "Node.h"
#include <iostream>

int main() {
     std::string expression = "2 + 10 + 52 *531";
    try {
        Node* root = buildExpressionTree(expression);
        root->saveToJsonFile("expression.json");
        delete root;
        Node* tree = buildExpressionTree("3 + 5 * 2 - 8");
        tree->saveToJsonFile("expression.json");
        tree->renderTree("expression_tree.png");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}
