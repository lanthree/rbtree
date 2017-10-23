#include <iostream>
#include <cstdlib>
#include "rbtree.h"

int main()
{
    rbtree _rbt;
    for (int i = 0; i < 20; i++) {
        int randint = random() % 1000;
        cout << "inserting " << randint << endl;
        _rbt.insert(randint);
        _rbt.inorder();
    }

    for (int i = 0; i < 20; i++) {
        int d_value = _rbt.root->value;
        cout << "deleting " << d_value << endl;
        _rbt.delete_value(d_value);
        _rbt.inorder();
    }

    return 0;
}
