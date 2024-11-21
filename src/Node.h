#ifndef NODE_H
#define NODE_H

#include <iostream>

enum NodeType { NUMBER, OPERATOR };

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

    friend void printNode(std::ostream& os, const Node& node, int indentLevel);

    friend std::ostream& operator<<(std::ostream& os, const Node& node);

private:
    NodeType type;
    union {
        int value;   
        char op;  
    };
    Node* left;
    Node* right;
};


#endif
