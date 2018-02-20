/* Copyright 2017 Natanael Josue Rabello */

#include <iostream>
#include "BSTree.hpp"
#include "AVLTree.hpp"

using namespace std;

int main() {
    using namespace trees;

    AVLTree<char> avl;

    avl.insert('g');
    avl.remove('g');
    char *c = avl.get('h');
    avl.print(cout, INORDER);

    return 0;
}
