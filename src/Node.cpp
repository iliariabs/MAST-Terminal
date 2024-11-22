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



std::string Node::toJson() const {
    std::ostringstream os;
    toJsonHelper(os);
    return os.str();
}


void Node::toJsonHelper(std::ostringstream& os) const {
    os << "{";
    if (type == NUMBER) {
        os << "\"value\": " << value;
    } else if (type == OPERATOR) {
        os << "\"operator\": \"" << op << "\"";
    }

    if (left || right) {
        os << ", \"left\": ";
        if (left) {
            left->toJsonHelper(os);
        } else {
            os << "null";
        }

        os << ", \"right\": ";
        if (right) {
            right->toJsonHelper(os);
        } else {
            os << "null";
        }
    }

    os << "}";
}

void Node::saveToJsonFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::ios_base::failure("Failed to open file: " + filename);
    }
    file << toJson();
    file.close();
}

Node* buildExpressionTree(const std::string& expression) {
    std::stack<Node*> nodes;
    std::stack<char> operators;

    auto applyOperator = [&]() {
        if (operators.empty() || nodes.size() < 2) {
            throw std::logic_error("Invalid expression.");
        }

        char op = operators.top();
        operators.pop();

        Node* right = nodes.top(); nodes.pop();
        Node* left = nodes.top(); nodes.pop();

        Node* parent = new Node(OPERATOR, op);
        parent->setLeft(left);
        parent->setRight(right);
        nodes.push(parent);
    };

    auto precedence = [](char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    };

    for (size_t i = 0; i < expression.size(); i++) {
        char ch = expression[i];

        if (std::isspace(ch)) {
            continue;
        } else if (std::isdigit(ch)) {
            int value = 0;
            while (i < expression.size() && std::isdigit(expression[i])) {
                value = value * 10 + (expression[i] - '0');
                i++;
            }
            i--;
            nodes.push(new Node(NUMBER, value));
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(ch)) {
                applyOperator();
            }
            operators.push(ch);
        } else {
            throw std::invalid_argument(std::string("Invalid character in expression: ") + ch);
        }
    }

    while (!operators.empty()) {
        applyOperator();
    }

    if (nodes.size() != 1) {
        throw std::logic_error("Invalid expression structure.");
    }

    return nodes.top();
}

void drawNode(std::vector<unsigned char>& image, int width, int x, int y, bool isNumber) {
    int radius = 20;


    unsigned char r = isNumber ? 0 : 255;
    unsigned char g = isNumber ? 255 : 0;
    unsigned char b = 0;

    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            if (dx * dx + dy * dy <= radius * radius) {
                int px = x + dx;
                int py = y + dy;
                if (px >= 0 && px < width && py >= 0 && py < width) {
                    int idx = 4 * (py * width + px);
                    image[idx] = r;
                    image[idx + 1] = g;
                    image[idx + 2] = b;
                    image[idx + 3] = 255;
                }
            }
        }
    }



}


void drawLine(std::vector<unsigned char>& image, int width, int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true) {
        if (x1 >= 0 && x1 < width && y1 >= 0 && y1 < width) {
            int idx = 4 * (y1 * width + x1);
            image[idx] = 0;
            image[idx + 1] = 255;
            image[idx + 2] = 0;
            image[idx + 3] = 255;
        }

        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

int Node::getDepth() const {
    int leftDepth = left ? left->getDepth() : 0;
    int rightDepth = right ? right->getDepth() : 0;
    return 1 + std::max(leftDepth, rightDepth);
}


void Node::renderHelper(std::vector<unsigned char>& image, int width, int x, int y, int offsetX, int offsetY) const {
    drawNode(image, width, x, y, type == NUMBER);

    if (left) {
        int newOffsetX = std::max(offsetX / 2, 20);
        drawLine(image, width, x, y, x - newOffsetX, y + offsetY);
        left->renderHelper(image, width, x - newOffsetX, y + offsetY, newOffsetX, offsetY);
    }
    if (right) {
        int newOffsetX = std::max(offsetX / 2, 20);
        drawLine(image, width, x, y, x + newOffsetX, y + offsetY);
        right->renderHelper(image, width, x + newOffsetX, y + offsetY, newOffsetX, offsetY);
    }
}


void Node::renderTree(const std::string& filename) const {
    int depth = getDepth();
    int height = depth * 150;
    int width = min(1000, std::pow(2, depth) * 100);

    std::vector<unsigned char> image(width * height * 4, 255);
    renderHelper(image, width, width / 2, 50, width / 4, 100);

    unsigned error = lodepng::encode(filename, image, width, height);
    if (error) {
        throw std::runtime_error("PNG encoding error: " + std::string(lodepng_error_text(error)));
    }
}
