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
 * @file: BSTree.hpp
 * 
 * Define a Binary Search Tree and its node, that henrit the proprieties
 * from a basic Tree and Node defined in 'TreeBase.hpp'
 * 
 * @example:
 * BSTree<T> bst;
 * bst.insert(t1);  // ou  bst << t2;
 * bst.remove(t1);  // ou bst.remove(t1, FUSION)
 * T *t = bst.get(t2);
 * bst.print(cout, INORDER)  // ou cout << bst;
 *
 * */

#ifndef _BSTREE_HPP_
#define _BSTREE_HPP_

#include <memory>
#include <utility>
#include <iostream>
#include "TreeBase.hpp"


/*******************************
 * Tree Data Structures
 *******************************/
namespace trees {


namespace base {

    /** BSTNode base: generic abstract class, base for derived nodes */
    template <class T, template<typename ...> class N>
    struct BSTNode : public Node<T, N> {
        N<T> *left = nullptr;
        N<T> *right = nullptr;
        using Node<T, N>::Node;
        virtual ~BSTNode() = 0;
    };
    // required definition of pure abstract desctructor
    template <class T, template<typename ...> class N>
    BSTNode<T, N>::~BSTNode() {}

}  // namespace base



/** Concrete BSTNode, for BSTree */
template <class T>
struct BSTNode : public base::BSTNode<T, BSTNode> {
    using base::BSTNode<T, BSTNode>::BSTNode;
    ~BSTNode() {}
};


/** Reading modes */
enum eOrder {
    INORDER,
    PREORDER,
    POSTORDER
};

/** Removal modes */
enum eRemove {
    COPY,
    FUSION
};


/* ^^^^^^^^^^^^^^^^^^
 * Binary Search Tree
 * ^^^^^^^^^^^^^^^^^^ */
template <class T, template<typename ...> class N = BSTNode>
class BSTree : public base::Tree<T, N> {
    using Base = base::Tree<T, N>;

 public:
    using Node = N<T>;  // aliases for the node type

    BSTree() : Base() {}
    virtual ~BSTree() { destroy(root); }

    /* External Methods */
    // bool Tree::isEmpty() const;
    void clear() override;
    T* get(T key) const override;
    T* getMax() const;
    T* getMin() const;
    bool insert(T key) override;
    std::unique_ptr<T> remove(T key) override;
    std::unique_ptr<T> remove(T key, eRemove mode);
    virtual std::unique_ptr<T> removeMax();
    virtual std::unique_ptr<T> removeMin();
    std::ostream& print(std::ostream& os, eOrder order = INORDER) const;

    template <class _T> friend BSTree<_T>& operator<<(BSTree<_T>& bst, _T key);
    template <class _T> friend std::ostream& operator<<(std::ostream& os, const BSTree<_T>& bst);

 protected:
    using Base::root;

    /* Internal recursive Methods */
    void destroy(Node *root);
    T* get(Node *node, T &key) const;
    virtual bool insert(Node *&node, T &key);
    std::unique_ptr<T> removeByCopy(Node *&node, T &key);
    std::unique_ptr<T> removeByFusion(Node *&node, T &key);
    Node*& findMax(Node *&root);
    Node*& findMin(Node *&root);
    void inorder(std::ostream& os, const Node *node) const;
    void preorder(std::ostream& os, const Node *node) const;
    void postorder(std::ostream& os, const Node *node) const;
};






/**
 * >> BSTree implementation <<
 * */

/** 
 * Clear the tree, deleting all nodes
 * */
template <class T, template<typename ...> class N>
void BSTree<T, N>::clear() {
    destroy(root);
    root = nullptr;
}

/**
 * Delete a node and its sub-trees
 * @see clear()
 * */
template <class T, template<typename ...> class N>
void BSTree<T, N>::destroy(Node *root) {
    if (root != nullptr) {
        destroy(root->left);
        destroy(root->right);
        delete root;
    }
}

/**
 * Search for a element, from root
 * @return a pointer to the element, or nullptr if it does not exist
 * */
template <class T, template<typename ...> class N>
T* BSTree<T, N>::get(T key) const {
    return get(root, key);
}

/**
 * @see get(T key)
 * */
template <class T, template<typename ...> class N>
T* BSTree<T, N>::get(Node *node, T &key) const {
    if (node == nullptr) return nullptr;
    if (key < node->key) return get(node->left, key);
    if (key > node->key) return get(node->right, key);
    return &node->key;
}

/**
 * Search for the greater element in the tree
 * @return a pointer to the element, or nullptr if it does not exist (empty tree)
 * */
template <class T, template<typename ...> class N>
T* BSTree<T, N>::getMax() const {
    if (root == nullptr) return nullptr;
    Node *max = root;
    while (max->right != nullptr)
        max = max->right;
    return &max->key;
}

/**
 * Search for the lesser element in the tree
 * @return a pointer to the element, or nullptr if it does not exist (empty tree)
 * */
template <class T, template<typename ...> class N>
T* BSTree<T, N>::getMin() const {
    if (root == nullptr) return nullptr;
    Node *min = root;
    while (min->left != nullptr)
        min = min->left;
    return &min->key;
}

/**
 * Insert a element in the tree
 * @return true if element was inserted succefully, or false if it already exists
 * */
template <class T, template<typename ...> class N>
bool BSTree<T, N>::insert(T key) {
    return insert(root, key);
}

/**
 * @see insert(T key)
 * */
template <class T, template<typename ...> class N>
bool BSTree<T, N>::insert(Node *&node, T &key) {
    if (node == nullptr) {
        node = new Node(key);
        return true;
    }
    if (key < node->key) return insert(node->left, key);
    if (key > node->key) return insert(node->right, key);
    return false;
}

/**
 * Remove a element from the tree by COPY
 * default method for remove(T key, eRemove mode)
 * @return a pointer to the removed element, or nullptr if it does not exist
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::remove(T key) {
    return remove(key, COPY);
}

/**
 * Remove a element from the tree by COPY or FUSION
 * @return a pointer to the removed element, or nullptr if it does not exist
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::remove(T key, eRemove mode) {
    switch (mode) {
        case COPY:
            return removeByCopy(root, key);
            break;
        case FUSION:
            return removeByFusion(root, key);
            break;
        default:
            return nullptr;
    }
}

/**
 * Remove the lesser element from the tree by FUSION (-> better performance)
 * @return a pointer to the removed element, or nullptr if it does not exist (empty tree)
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::removeMax() {
    if (root == nullptr) return nullptr;
    Node *&max = findMax(root);
    return removeByFusion(max, max->key);
}

/**
 * Remove the lesser element from the tree by FUSION (-> better performance)
 * @return a pointer to the removed element, or nullptr if it does not exist (empty tree)
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::removeMin() {
    if (root == nullptr) return nullptr;
    Node *&min = findMin(root);
    return removeByFusion(min, min->key);
}

/**
 * Search for the Node of greater element recursivesly
 * in a sub-tree. Used for removal
 * */
template <class T, template<typename ...> class N>
auto BSTree<T, N>::findMax(Node *&root) -> Node*& {
    return root->right ? findMax(root->right) : root;
}

/**
 * Search for the Node of lesser element recursivesly
 * in a sub-tree. Used for removal
 * */
template <class T, template<typename ...> class N>
auto BSTree<T, N>::findMin(Node *&root) -> Node*& {
    return root->left ? findMin(root->left) : root;
}

/**
 * Perform a removal by COPY
 * @see remove(T key, eRemove mode)
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::removeByCopy(Node *&node, T &key) {
    if (node == nullptr) return nullptr;
    if (node->key > key) return removeByCopy(node->left, key);
    if (node->key < key) return removeByCopy(node->right, key);
    auto keyptr = std::make_unique<T>(std::move(node->key));
    if (node->left != nullptr) {
        Node *&max = findMax(node->left);
        node->key = max->key;
        Node *temp = max->left;
        delete max;
        max = temp;
    } else {
        Node *temp = node->right;
        delete node;
        node = temp;
    }
    return keyptr;
}

/**
 * Perform a removal by FUSION
 * @see remove(T key, eRemove mode)
 * */
template <class T, template<typename ...> class N>
std::unique_ptr<T> BSTree<T, N>::removeByFusion(Node *&node, T &key) {
    if (node == nullptr) return nullptr;
    if (node->key > key) return removeByFusion(node->left, key);
    if (node->key < key) return removeByFusion(node->right, key);
    auto keyptr = std::make_unique<T>(std::move(node->key));
    Node *temp = node->right;
    if (node->left != nullptr) {
        temp = node->left;
        findMax(node->left) ->right = node->right;
    }
    delete node;
    node = temp;
    return keyptr;
}

/**
 * Print to console in a given order
 * */
template <class T, template<typename ...> class N>
std::ostream& BSTree<T, N>::print(std::ostream& os, eOrder order) const {
    switch (order) {
        case INORDER:
            inorder(os, root);
            break;
        case PREORDER:
            preorder(os, root);
            break;
        case POSTORDER:
            postorder(os, root);
            break;
    }
    return os;
}

/** @see  display(eOrder order) */
template <class T, template<typename ...> class N>
void BSTree<T, N>::inorder(std::ostream& os, const Node *node) const {
    if (node == nullptr) return;
    inorder(os, node->left);
    os << *node << " ";
    inorder(os, node->right);
}

/** @see  display(eOrder order) */
template <class T, template<typename ...> class N>
void BSTree<T, N>::preorder(std::ostream& os, const Node *node) const {
    if (node == nullptr) return;
    os << *node << " ";
    preorder(os, node->left);
    preorder(os, node->right);
}

/** @see  display(eOrder order) */
template <class T, template<typename ...> class N>
void BSTree<T, N>::postorder(std::ostream& os, const Node *node) const {
    if (node == nullptr) return;
    postorder(os, node->left);
    postorder(os, node->right);
    os << *node << " ";
}

/**
 * Overloading for insertion like: bst << key;
 * */
template <class T>
inline BSTree<T>& operator<<(BSTree<T>& bst, T key) {
    bst.insert(key);
    return bst;
}

/**
 * Overloading for printing like: std::cout << bst;
 * */
template <class T>
inline std::ostream& operator<<(std::ostream& os, const BSTree<T>& bst) {
    bst.inorder(os, bst.root);
    return os;
}


}  // namespace trees








/**
 * Bonus:
 * >> Natural (and cool) utilities for printing
 * 
 * @example
 * BSTree<T> bst;
 * std::cout << inorder(bst);
 * std::cout << preorder(bst);
 * std::cout << postorder(bst);
 * */
namespace trees {

template <class T>
inline std::pair<BSTree<T>&, eOrder> inorder(BSTree<T>& bst) {
    return std::make_pair(std::ref(bst), INORDER);
}

template <class T>
inline std::pair<BSTree<T>&, eOrder> preorder(BSTree<T>& bst) {
    return std::make_pair(std::ref(bst), PREORDER);
}

template <class T>
inline std::pair<BSTree<T>&, eOrder> postorder(BSTree<T>& bst) {
    return std::make_pair(std::ref(bst), POSTORDER);
}

template <class T>
inline std::ostream& operator<<(std::ostream& os, std::pair<BSTree<T>&, eOrder> p) {
    return p.first.print(os, p.second);
}

}  // namespace trees



#endif  // end of include guard: _BSTREE_HPP_
