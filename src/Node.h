#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
#include <cmath>
#include "libs/lodepng.h"

enum NodeType { NUMBER, OPERATOR };

template<typename T1, typename T2>
auto min(T1 a, T2 b) -> decltype((a < b) ? a : b) {
    return (a < b) ? a : b;
}

class Node {
public:
    Node(NodeType nodeType, char op);
    Node(NodeType nodeType, int value);
    Node();

    ~Node();

    NodeType getType() const;
    int getValue() const;
    char getOperator() const;
    Node* getLeft() const;
    Node* getRight() const;

    void setType(NodeType newType);
    void setValue(int newValue);
    void setOperator(char op);
    void setLeft(Node* newLeft);
    void setRight(Node* newRight);

    bool isChildOf(const Node* parent) const;

    std::string toJson() const;
    void toJsonHelper(std::ostringstream& os) const;

    void saveToJsonFile(const std::string& filename) const;

    int getDepth() const;
    void renderTree(const std::string& filename) const;
    void renderHelper(std::vector<unsigned char>& image, int width, int x, int y, int offsetX, int offsetY) const;
private:
    NodeType type;
    union {
        int value;
        char op;
    };
    Node* left;
    Node* right;
};

Node* buildExpressionTree(const std::string& expression);

#endif
