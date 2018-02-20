/* =========================================================================
This library is placed under the MIT License
Copyright 2017 Natanael Josue Rabello. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to
deal in the Software without restriction, including without limitation the
rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.
 ========================================================================= */


/**
 * @file: AVLTree.hpp
 * 
 * Define a AVL Tree, a form of self-balancing Binary Search Tree.
 * Since AVLTree contains the proprieties of a BST, it inherits the BSTree defined in 'BSTree.hpp'
 * as well as a AVLNode inherits BSTNode.
 * @see description in BSTree.hpp
 * 
 * @example:
 * AVLTree<T> avl;
 * avl.insert(t1)  // ou avl << t2;
 * avl.remove(t1)
 * T *t = avl.get(t2);
 * avl.print(cout, INORDER);  // ou cout << avl;
 * 
 * */

#ifndef _AVLTREE_HPP_
#define _AVLTREE_HPP_

#include <memory>
#include <utility>
#include <ostream>
#include "BSTree.hpp"


/*******************************
 * Tree Data Structures
 *******************************/
namespace trees {


namespace base {

    /** AVLNode base, generic abstract class, base for derived nodes */
    template <class T, template<typename ...> class N>
    struct AVLNode : public BSTNode<T, N> {
        int height = 1;
        using BSTNode<T, N>::BSTNode;
        virtual ~AVLNode() = 0;
        int updateHeight();
    };
    // required definition of pure abstract desctructor
    template <class T, template<typename ...> class N>
    AVLNode<T, N>::~AVLNode() {}

}  // namespace base



/** Concrete AVLNode, for AVLTree */
template <class T>
struct AVLNode : public base::AVLNode<T, AVLNode> {
    using base::AVLNode<T, AVLNode>::AVLNode;
    ~AVLNode() {}
};



/* ^^^^^^^^^
 * AVL Tree
 * ^^^^^^^^^ */
template <class T>
class AVLTree : public BSTree<T, AVLNode> {
    using Base = BSTree<T, AVLNode>;

 public:
    using Node = AVLNode<T>;  // aliases for the node type

    AVLTree() : Base() {}
    ~AVLTree() {}

    /* Metodos externos */
    // bool Tree::isEmpty() const;
    // void BSTree::clear();
    // T* BSTree::get(T key) const;
    // T* BSTree::getMax() const;
    // T* BSTree::getMin() const;
    using Base::insert;  // use of the same insert(T key) from base which calls protected insert
    std::unique_ptr<T> remove(T key) override;
    std::unique_ptr<T> removeMax() override;
    std::unique_ptr<T> removeMin() override;
    // std::ostream& BSTree::print(std::ostream& os, eOrder order = INORDER) const;

    template <class _T> friend AVLTree<_T>& operator<<(AVLTree<_T>& avl, _T key);
    template <class _T> friend std::ostream& operator<<(std::ostream& os, const AVLTree<_T>& avl);

 protected:
    using Base::root;

    /* Metodos internos */
    bool insert(Node *&node, T &key) override;
    std::unique_ptr<T> remove(Node *&node, T &key);
    Node* pullMax(Node *&node);
    void balance(Node *&node);
    int  bFactor(Node *node);
    void rotateLeft(Node *&node);
    void rotateRight(Node *&node);
};





/**
 * >> AVLTree implementation <<
 * */

/**
 * (AVLNode)
 * */
template <class T, template<typename ...> class N>
int base::AVLNode<T, N>::updateHeight() {
    int leftH = this->left ? this->left->height : 0;
    int rightH = this->right ? this->right->height : 0;
    return (height = (leftH > rightH ? leftH : rightH) + 1);
}

/**
 * Insert a element in the tree
 * @return true if element was inserted succefully, or false if it already exists
 * @see BSTree::insert(T key)
 * */
template <class T>
bool AVLTree<T>::insert(Node *&node, T &key) {
    if (node == nullptr) {
        node = new Node(key);
        return true;
    }
    if (key == node->key) return false;
    if (key < node->key) {
        if (!insert(node->left, key)) return false;
    } else {
        if (!insert(node->right, key)) return false;
    }
    balance(node);
    return true;
}

/**
 * Remove a element from the tree
 * @return a pointer to the removed element, or nullptr if it does not exist
 * */
template <class T>
std::unique_ptr<T> AVLTree<T>::remove(T key) {
    return remove(root, key);
}

/**
 * @see remove(T key)
 * @note: Removal is done by copy (better for AVLTree -> fewer rotations)
 * */
template <class T>
std::unique_ptr<T> AVLTree<T>::remove(Node *&node, T &key) {
    if (node == nullptr) return nullptr;
    auto keyptr = std::unique_ptr<T>{nullptr};
    if (key < node->key) {
        if (!(keyptr = remove(node->left, key))) return keyptr;
    } else if (key > node->key) {
        if (!(keyptr = remove(node->right, key))) return keyptr;
    } else {  // achou
        keyptr = std::make_unique<T>(std::move(node->key));
        if (node->left != nullptr) {
            Node *max = pullMax(node->left);
            node->key = max->key;
            delete max;
        } else {
            Node *temp = node->right;
            delete node;
            node = temp;
            return keyptr;  // soh 1 filho, nao precisa balancear
        }
    }
    balance(node);
    return keyptr;
}

/**
 * Remove the greater element from the tree
 * @return a pointer to the removed element, or nullptr if it does not exist (empty tree)
 * */
template <class T>
std::unique_ptr<T> AVLTree<T>::removeMax() {
    if (root == nullptr) return std::unique_ptr<T>{nullptr};
    Node *&max = Base::findMax(root);
    return remove(max, max->key);
}

/**
 * Remove the lesser element from the tree
 * @return a pointer to the removed element, or nullptr if it does not exist (empty tree)
 * */
template <class T>
std::unique_ptr<T> AVLTree<T>::removeMin() {
    if (root == nullptr) return std::unique_ptr<T>{nullptr};
    Node *&min = Base::findMin(root);
    return remove(min, min->key);
}

/**
 * Remove the node of maximum element but do not destroy it, just extract it from
 * its place and return it, balancing the sub-tree recursively. (used in remove())
 * @see remove(Node *&node, T &key)
 * */
template <class T>
auto AVLTree<T>::pullMax(Node *&node) -> Node* {
    Node *max;
    if (node->right != nullptr) {
        max = pullMax(node->right);
        balance(node);
    } else {
        max = node;
        node = node->left;
    }
    return max;
}

/**
 * Balance the tree if needed
 * */
template <class T>
void AVLTree<T>::balance(Node *&node) {
    node->updateHeight();
    int bf = bFactor(node);
    if (bf == 2) {
        if (bFactor(node->left) < 0)
            rotateLeft(node->left);
        rotateRight(node);
    } else if (bf == -2) {
        if (bFactor(node->right) > 0)
            rotateRight(node->right);
        rotateLeft(node);
    }
}

/**
 * Balance Factor
 * left sub-tree height minus right sub-tree height
 * */
template <class T>
inline int AVLTree<T>::bFactor(Node *node) {
    return (node->left ? node->left->height : 0)
        - (node->right ? node->right->height : 0);
}

/**
 * Simple left rotation
 * */
template <class T>
void AVLTree<T>::rotateLeft(Node *&node) {
    Node *temp = node->right->left;
    node->right->left = node;
    node = node->right;
    node->left->right = temp;
    node->left->updateHeight();
    node->updateHeight();
}

/**
 * Simple right rotation
 * */
template <class T>
void AVLTree<T>::rotateRight(Node *&node) {
    Node *temp = node->left->right;
    node->left->right = node;
    node = node->left;
    node->right->left = temp;
    node->right->updateHeight();
    node->updateHeight();
}

/**
 * Overloading for insertion like: avl << key;
 * */
template <class T>
inline AVLTree<T>& operator<<(AVLTree<T>& avl, T key) {
    avl.insert(key);
    return avl;
}

/**
 * Overloading for printing like: std::cout << avl;
 * */
template <class T>
inline std::ostream& operator<<(std::ostream& os, const AVLTree<T>& avl) {
    avl.inorder(os, avl.root);
    return os;
}


}  // namespace trees








/**
 * Bonus:
 *  >> Natural (and cool) utilities for printing
 * 
 * @example
 * AVLTree<T> avl;
 * std::cout << inorder(avl);
 * std::cout << preorder(avl);
 * std::cout << postorder(avl);
 * */
namespace trees {

template <class T>
inline std::pair<AVLTree<T>&, eOrder> inorder(AVLTree<T>& avl) {
    return std::make_pair(std::ref(avl), INORDER);
}

template <class T>
inline std::pair<AVLTree<T>&, eOrder> preorder(AVLTree<T>& avl) {
    return std::make_pair(std::ref(avl), PREORDER);
}

template <class T>
inline std::pair<AVLTree<T>&, eOrder> postorder(AVLTree<T>& avl) {
    return std::make_pair(std::ref(avl), POSTORDER);
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, std::pair<AVLTree<T>&, eOrder> p) {
    return p.first.print(os, p.second);
}

}  // namespace trees



#endif  // end of include guard: _AVLTREE_HPP_
