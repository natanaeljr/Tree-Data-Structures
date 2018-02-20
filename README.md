# Tree Data Structures

Templated Binary Search Tree and AVL Tree implementation, written in C++.

### Usage

#### Example:

```C++
#include <iostream>
#include "AVLTree.hpp"
#include "BSTree.hpp"

int main() {
    trees::AVLTree<int> avl;
    trees::BSTree<char> bst;

    // insert elements
    const int n = 5;
    avl << n << 50 << 10 << 2;
    bst << 'f' << 'm' << 'e' << 'a';
    // insert with check
    if (!bst.insert('a')) std::cout << "char 'a' already exists!" << std::endl;

    // remove elements
    bst.removeMin();  // removes 'a'
    if (avl.remove(5)) std::cout << "5 removed successfully!" << std::endl;
    avl.removeMax();  // removes 50

    // get elements (if exists)
    int *x = avl.get(10);
    if (x) std::cout << "Got " << *x << std::endl;
    if (!bst['b']) std::cerr << "'b' does not exists in bst" << std::endl;
    std::cout << "Max in bst: " << bst.getMax() << std::endl;

    // print trees
    avl.print(std::cout, trees::INORDER);  // or
    std::cout << '\n' << inorder(bst) << "\n" << preorder(avl) << std::endl;

    // clear trees
    bst.clear();
    avl.clear();
    if (bst.isEmpty()) std::cout << "bst is empty" << std::endl;
    if (avl) std::cerr << "avl not empty" << std::endl;

    return 0;
}
```

---

Copyright © 2017 Natanael Josue Rabello [_natanael.rabello@outlook.com_]
