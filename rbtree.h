#include <iostream>
using namespace std;

enum class NodeColor
{
    RED,
    BLACK
};

struct Node
{
    int value;
    NodeColor color;
    Node *leftTree, *rightTree, *parent;

    Node() : value(0), color(NodeColor::RED),
        leftTree(nullptr), rightTree(nullptr), parent(nullptr) {}
    
    Node* grandparent() {
        if (parent == nullptr) {
            return nullptr;
        }
        return parent->parent;
    }

    Node* uncle() {
        if (grandparent() == nullptr) {
            return nullptr;
        }
        if (parent == grandparent()->rightTree) {
            return grandparent()->leftTree;
        } else {
            return grandparent()->rightTree;
        }
    }

    Node* sibling() {
        if (parent == nullptr) {
            return nullptr;
        } else if (parent->leftTree == this) {
            return parent->rightTree;
        } else {
            return parent->leftTree;
        }
    }
};

class rbtree
{
public:
    rbtree() : root(nullptr) {
        NIL = new Node();
        NIL->color = NodeColor::BLACK;
    }

    ~rbtree() {
        if (root) delete_tree(root);
        delete NIL;
    }

    void inorder() {
        if (root == nullptr) return;
        inorder(root);
        cout << endl;
    }

    void insert(int x) {
        if (root == nullptr) {
            root = new Node();
            root->color = NodeColor::BLACK;
            root->leftTree = root->rightTree = NIL;
            root->value = x;
        } else {
            insert(root, x);
        }
    }

    bool delete_value(int data) {
        return delete_child(root, data);
    }

private:
    void delete_tree(Node *p) {
        if (!p || p == NIL) {
            return;
        }
        delete_tree(p->leftTree);
        delete_tree(p->rightTree);
        delete p;
    }

    void inorder(Node *p) {
        if (p == NIL)
            return;

        if (p->leftTree)
            inorder(p->leftTree);

        cout << p->value << " ";

        if (p->rightTree)
            inorder(p->rightTree);
    }

    void insert(Node *p, int data) {
        if (p->value >= data) {
            if (p->leftTree != NIL) {
                insert(p->leftTree, data);
            } else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = p;
                p->leftTree = tmp;
                insert_case(tmp);
            }
        } else {
            if (p->rightTree != NIL) {
                insert(p->rightTree, data);
            } else {
                Node *tmp = new Node();
                tmp->value = data;
                tmp->leftTree = tmp->rightTree = NIL;
                tmp->parent = p;
                p->rightTree = tmp;
                insert_case(tmp);
            }
        }
    }

    void insert_case(Node *p) {
        if (p->parent == nullptr) {
            root = p;
            p->color = NodeColor::BLACK;
            return;
        }

        if (p->parent->color == NodeColor::BLACK) {
            return;
        }

        // p->parent->color == NodeColor::RED
        if (p->uncle()->color == NodeColor::RED) {
            p->parent->color = p->uncle()->color = NodeColor::BLACK;
            p->grandparent()->color = NodeColor::RED;
            insert_case(p->grandparent());
        } else {
            if (p->parent->rightTree == p && p->grandparent()->leftTree == p->parent) {
                rotate_left(p);
                rotate_right(p);
                p->color = NodeColor::BLACK;
                p->leftTree->color = p->rightTree->color = NodeColor::RED;
            } else if (p->parent->leftTree == p && p->grandparent()->rightTree == p->parent) {
                rotate_right(p);
                rotate_left(p);
                p->color = NodeColor::BLACK;
                p->leftTree->color = p->rightTree->color = NodeColor::RED;
            } else if (p->parent->leftTree == p && p->grandparent()->leftTree == p->parent) {
                p->parent->color = NodeColor::BLACK;
                p->grandparent()->color = NodeColor::RED;
                rotate_right(p->parent);
            } else if (p->parent->rightTree == p && p->grandparent()->rightTree == p->parent) {
                p->parent->color = NodeColor::BLACK;
                p->grandparent()->color = NodeColor::RED;
                rotate_left(p->parent);
            }
        }
    }

    bool delete_child(Node *p, int data) {
        if (p->value > data) {
            if (p->leftTree == NIL) {
                return false;
            }
            return delete_child(p->leftTree, data);
        } else if (p->value < data) {
            if (p->rightTree == NIL) {
                return false;
            }
            return delete_child(p->rightTree, data);
        } else if (p->value == data) {
            if (p->rightTree == NIL) {
                delete_one_child(p);
                return true;
            }
            Node *smallest = get_smallest_child(p->rightTree);
            swap(p->value, smallest->value);
            delete_one_child(smallest);

            return true;
        }
    }

    void delete_one_child(Node *p) {
        Node *child = p->leftTree == NIL ? p->rightTree : p->leftTree;
        
        if (p->parent == nullptr && p->leftTree == NIL && p->rightTree == NIL) {
            delete p;
            root = nullptr;
            return;
        }

        if (p->parent == nullptr) {
            delete p;
            child->parent = nullptr;
            child->color = NodeColor::BLACK;
            root = child;
            return;
        }

        if (p->parent->leftTree == p) {
            p->parent->leftTree = child;
        } else {
            p->parent->rightTree = child;
        }
        child->parent = p->parent;

        if (p->color == NodeColor::BLACK) {
            if(child->color == NodeColor::RED) {
                child->color = NodeColor::BLACK;
            } else {
                delete_case(child);
            }
        }
        delete p;
    }

    void delete_case(Node *p) {
        if (p->parent == nullptr) {
            p->color = NodeColor::BLACK;
            return;
        }

        if (p->sibling()->color == NodeColor::RED) {
            p->parent->color = NodeColor::RED;
            p->sibling()->color = NodeColor::BLACK;
            if (p == p->parent->leftTree) {
                rotate_left(p->sibling());
            } else {
                rotate_right(p->sibling());
            }
        }

        if (p->parent->color == NodeColor::BLACK
            && p->sibling()->color == NodeColor::BLACK
            && p->sibling()->leftTree->color == NodeColor::BLACK
            && p->sibling()->rightTree->color == NodeColor::BLACK) {
            p->sibling()->color = NodeColor::RED;
            delete_case(p->parent);

        } else if (p->parent->color == NodeColor::RED
            && p->sibling()->color == NodeColor::BLACK
            && p->sibling()->leftTree->color == NodeColor::BLACK
            && p->sibling()->rightTree->color == NodeColor::BLACK) {
            p->sibling()->color = NodeColor::RED;
            p->parent->color = NodeColor::BLACK;

        } else {
            if (p->sibling()->color == NodeColor::BLACK) {
                if (p == p->parent->leftTree
                    && p->sibling()->leftTree->color == NodeColor::RED
                    && p->sibling()->rightTree->color == NodeColor::BLACK) {
                    p->sibling()->color = NodeColor::RED;
                    p->sibling()->leftTree->color = NodeColor::BLACK;
                    rotate_right(p->sibling()->leftTree);
                    
                } else if (p == p->parent->rightTree
                    && p->sibling()->leftTree->color == NodeColor::BLACK
                    && p->sibling()->rightTree->color == NodeColor::RED) {
                    p->sibling()->color = NodeColor::RED;
                    p->sibling()->rightTree->color = NodeColor::BLACK;
                    rotate_left(p->sibling()->rightTree);
                }
                
                p->sibling()->color = p->parent->color;
                p->parent->color = NodeColor::BLACK;
                if (p == p->parent->leftTree) {
                    p->sibling()->rightTree->color = NodeColor::BLACK;
                    rotate_left(p->sibling());
                } else {
                    p->sibling()->leftTree->color = NodeColor::BLACK;
                    rotate_right(p->sibling());
                }
            }
        }
    }

    Node* get_smallest_child(Node *p) {
        if (p->leftTree == NIL) {
            return p;
        }
        return get_smallest_child(p->leftTree);
    }

    void rotate_left(Node *p) {
        if (p->parent == nullptr) {
            root = p;
            return;
        }

        Node *gp = p->grandparent();
        Node *fa = p->parent;
        Node *y = p->leftTree;

        fa->rightTree = y;
        if (y != NIL) y->parent = fa;

        p->leftTree = fa;
        fa->parent = p;

        if (root == fa) root = p;
        
        p->parent = gp;
        if (gp != nullptr) {
            if (gp->leftTree == fa) {
                gp->leftTree = p;
            } else {
                gp->rightTree = p;
            }
        }
    }

    void rotate_right(Node *p) {
        if (p->parent == nullptr) {
            root = p;
            return;
        }

        Node *gp = p->grandparent();
        Node *fa = p->parent;
        Node *y = p->rightTree;

        fa->leftTree = y;
        if (y != NIL) y->parent = fa;

        p->rightTree = fa;
        fa->parent = p;

        if (root == fa) root = p;

        p->parent = gp;
        if (gp != nullptr) {
            if (gp->leftTree == fa) {
                gp->leftTree = p;
            } else {
                gp->rightTree = p;
            }
        }
    }

// For test
public:
    Node *root;

private:
    Node *NIL;
};