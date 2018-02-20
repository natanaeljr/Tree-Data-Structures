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
 * @file: TreeBase.hpp
 * 
 * Define basic generic Tree and Node
 * for building specialized Trees.
 * 
 * */

#ifndef _TREEBASE_HPP_
#define _TREEBASE_HPP_

#include <memory>
#include <ostream>

/*******************************
 * Tree Data Structures
 *******************************/
namespace trees {

namespace base {

    /** Tree Base: generic abstract class for specialized Tree  */
    template <class T, template<typename ...> class N>
    class Tree {
     protected:
        N<T>* root = nullptr;
     public:
        virtual ~Tree() {}
        operator bool() const {return root != nullptr;}
        bool isEmpty() const {return root == nullptr;}
        // for implementing
        virtual void clear() = 0;
        virtual T* get(T key) const = 0;
        virtual bool insert(T key) = 0;
        virtual std::unique_ptr<T> remove(T key) = 0;
        T* operator[](T key) { return get(key); }
    };

    /** Node Base: generic abstract class for specialized Node */
    template <class T, template<typename ...> class N>
    struct Node {
        T key;
        explicit Node(T key) : key(key) {}
        virtual ~Node() = 0;
        friend std::ostream& operator<<(std::ostream& os, const N<T>& node) {
            os << node.key; return os;
        }
    };
    // required definition of pure abstract desctructor
    template <class T, template<typename ...> class N>
    Node<T, N>::~Node() {}


}  // namespace base

}  // namespace trees


#endif  // end of include guard: _TREEBASE_HPP_
