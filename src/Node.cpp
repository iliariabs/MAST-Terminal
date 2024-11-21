#include "Node.h"

Node::Node(NodeType nodeType, int newValue)
    : type(nodeType), value(newValue), left(nullptr), right(nullptr) {}

Node::Node(NodeType nodeType, char op)
    : type(nodeType), op(op), left(nullptr), right(nullptr) {}

Node::Node()
    : type(NUMBER), value(0), left(nullptr), right(nullptr) {}

Node::~Node() {
    left = nullptr;
    right = nullptr;
}

NodeType Node::getType() const { return type; }

int Node::getValue() const {
    if (type == NUMBER) {
        return value;
    }
    throw std::logic_error("This node is not of type NUMBER.");
}

char Node::getOperator() const {
    if (type == OPERATOR) {
        return op;
    }
    throw std::logic_error("This node is not of type OPERATOR.");
}

Node* Node::getLeft() const { return left; }
Node* Node::getRight() const { return right; }

void Node::setType(NodeType newType) { type = newType; }
void Node::setValue(int newValue) {
    if (type == NUMBER) {
        value = newValue;
    }
}

void Node::setOperator(char newOp) {
    if (type == OPERATOR) {
        op = newOp;
    }
}

void Node::setLeft(Node* newLeft) { left = newLeft; }
void Node::setRight(Node* newRight) { right = newRight; }

bool Node::isChildOf(const Node* parent) const {
    if (!parent) {
        return false;
    }
    return (parent->left == this || parent->right == this); 
}

void printNode(std::ostream& os, const Node& node, int indentLevel) {
    std::string indent;
    indent.append(indentLevel, ' ');
    if (node.type == NUMBER) {
        os << indent << "   " << node.value << "\n";
    } else if (node.type == OPERATOR) {
        os << indent << "   " << node.op << "\n"; 
    }
    
    if (node.left || node.right) { 
        os << indent << " /   \\" << "\n"; 

        if (node.left) {
            os << indent << node.left->value;  
        } else {
            os << indent << " ";
        }

        if (node.right) {
            if (node.right->getType() == OPERATOR){
                os << "   " << node.right->op << "\n";
                printNode(os, *node.right, indentLevel + 3); 
            }
            else{
                os << "   " << node.right->value << "\n";
            }
        } else {
            os << indent << "     " << " " << "\n";
        }
    }
}

std::ostream& operator<<(std::ostream& os, const Node& node) {
    printNode(os, node, 0); 
    return os;
}
