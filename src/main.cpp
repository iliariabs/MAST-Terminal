#include "Node.h"
#include <iostream>

int main() {
    Node opNode(OPERATOR, '+');
    Node leftChild(NUMBER, 3);
    Node rightChild(OPERATOR, '+');
    Node rightrightChild(NUMBER, 523);
    Node rightleftChild(NUMBER, 614);

    opNode.setLeft(&leftChild);
    opNode.setRight(&rightChild);
    rightChild.setLeft(&rightleftChild);
    rightChild.setRight(&rightrightChild);

    std::cout << opNode.toJson() << std::endl;

    opNode.saveToJsonFile("output.json");

    return 0;
}
