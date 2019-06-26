#ifndef NEVMEM_SET
#define NEVMEM_SET

#include <algorithm>
#include <cstdlib>

namespace NevMem {

    template <typename T>
    class AVLNode {
     public:
        T value;
        AVLNode* parent;
        AVLNode* left;
        AVLNode* right;
        size_t size, depth;

        AVLNode(const T& nodeValue) :
            value(nodeValue), parent(nullptr), left(nullptr), right(nullptr), size(1), depth(1) {}
    };

    template <typename T>
    size_t get_size_(const AVLNode<T>* v) {
        if (!v) return 0;
        return v->size;
    }
    
    template <typename T>
    size_t get_depth_(const AVLNode<T>* v) {
        if (!v) return 0;
        return v->depth;
    }

    template <typename T>
    T* leftmost_(T* from) {
        if (!from) return nullptr;
        while (from->left)
            from = from->left;
        return from;
    }

    template <typename T>
    T* rightmost_(T* from) {
        if (!from) return nullptr;
        while (from->right)
            from = from->right;
        return from;
    }

    template <typename T>
    class Set {
     private:
        AVLNode<T>* root;

        AVLNode<T>* recursive_copy_node_(const AVLNode<T>* v) {
            if (!v) return nullptr;
            auto current = new AVLNode<T>(v->value);
            try {
                current->left = recursive_copy_node_(v->left);
            } catch (std::bad_alloc e) {
                delete current;
                throw e;
            }
            try {
                current->right = recursive_copy_node_(v->right);
            } catch (std::bad_alloc e) {
                recursive_delete_(current->left);
                delete current;
                throw e;
            }
            recalc_(current);
            set_parent_(current->left, current);
            set_parent_(current->right, current);
            return current;
        }
        
        void recalc_(AVLNode<T>* v) {
            if (!v) return;
            v->size = get_size_(v->left) + 1 + get_size_(v->right);
            v->depth = std::max(get_depth_(v->left), get_depth_(v->right)) + 1;
        }

        AVLNode<T>* create_node_(const T& value, AVLNode<T>* parent) {
            auto node = new AVLNode<T>(value);
            node->parent = parent;
            return node;
        }

        int depth_difference_(AVLNode<T>* v) {
            if (!v) return 0;
            return static_cast<int>(get_depth_(v->left)) - static_cast<int>(get_depth_(v->right));
        }

        void change_son_(AVLNode<T>* v, AVLNode<T>* from, AVLNode<T>* to) {
            if (!v) return;
            if (v->left == from)
                v->left = to;
            if (v->right == from)
                v->right = to;
        }

        void set_parent_(AVLNode<T>* v, AVLNode<T>* new_parent) {
            if (!v) return;
            v->parent = new_parent;
        }

        void small_left_rotate_(AVLNode<T>* a, AVLNode<T>* b) {
            AVLNode<T> *parent = a->parent;
            change_son_(parent, a, b);
            b->parent = parent;
            a->parent = b;
            set_parent_(b->right, a);
            a->left = b->right;
            b->right = a;
            recalc_(a);
            recalc_(b);
            if (parent == nullptr)
                root = b;
        }

        void small_right_rotate_(AVLNode<T>* a, AVLNode<T>* b) {
            AVLNode<T>* parent = a->parent;
            change_son_(parent, a, b);
            b->parent = parent;
            a->parent = b;
            set_parent_(b->left, a);
            a->right = b->left;
            b->left = a;
            recalc_(a);
            recalc_(b);
            if (parent == nullptr)
                root = b;
        }

        void resolve_(AVLNode<T>* v) {
            if (!v) return;
            if (get_depth_(v->right) > get_depth_(v->left) + 1) {
                if (depth_difference_(v->right) >= 1)
                    small_left_rotate_(v->right, v->right->left);
                small_right_rotate_(v, v->right);
            }
            if (get_depth_(v->left) > get_depth_(v->right) + 1){
                if (depth_difference_(v->left) <= -1)
                    small_right_rotate_(v->left, v->left->right);
                small_left_rotate_(v, v->left);
            }
        }

        void insert_(AVLNode<T>* v, const T& value) {
            if (!v) return;
            if (v->value < value && !v->right) {
                v->right = create_node_(value, v);
                recalc_(v);
                return;
            }
            if (value < v->value && !v->left) {
                v->left = create_node_(value, v);
                recalc_(v);
                return;
            }
            if (value < v->value) {
                insert_(v->left, value);
            } else if (v->value < value) {
                insert_(v->right, value);
            }
            recalc_(v);
            resolve_(v);
        }

        const AVLNode<T>* const_find_(const AVLNode<T>* v, const T& value) const {
            if (!v) return nullptr;
            if (v->value < value) return const_find_(v->right, value);
            else if (value < v->value) return const_find_(v->left, value);
            return v;
        }

        AVLNode<T>* find_(AVLNode<T>* v, const T& value) {
            if (!v) return nullptr;
            if (v->value < value) return find_(v->right, value);
            else if (value < v->value) return find_(v->left, value);
            return v;
        }

        void recursive_resolve_(AVLNode<T>* node) {
            while (node) {
                recalc_(node);
                resolve_(node);
                node = node->parent;
            }
        }

        void erase_(AVLNode<T>* node) {
            if (!node) return;
            if (!node->left && !node->right) {
                change_son_(node->parent, node, nullptr);
                if (node == root) root = nullptr;
                recursive_resolve_(node->parent);
                delete node;
            } else {
                if (get_depth_(node->left) > get_depth_(node->right)) {
                    auto ptr = rightmost_(node->left);
                    node->value = ptr->value;
                    erase_(ptr);
                } else {
                    auto ptr = leftmost_(node->right);
                    node->value = ptr->value;
                    erase_(ptr);
                }
            }
        }

        void recursive_delete_(AVLNode<T>* current) {
            if (!current) return;
            recursive_delete_(current->left);
            recursive_delete_(current->right);
            delete current;
        }

        const AVLNode<T>* lower_bound_(const AVLNode<T>* current, const T& value) const {
            if (!current) return nullptr;
            if (current->value < value) {
                return lower_bound_(current->right, value);
            } else if (value < current->value) {
                auto left = lower_bound_(current->left, value);
                if (!left) return current;
                return left;
            } else {
                return current;
            }
        }
     public:
        Set() : root(nullptr) {}

        Set(const Set& other) : Set() {
            root = recursive_copy_node_(other.root);
        }

        template <typename InputIter>
        Set(InputIter begin, InputIter end) : Set() {
            try {
                while (begin != end) {
                    insert(*begin);
                    ++begin;
                }
            } catch (std::bad_alloc e) {
                recursive_copy_node_(root);
                throw e;
            }
        }

        Set(std::initializer_list<T> list) : Set(list.begin(), list.end()) {}

        Set& operator=(const Set& other) {
            if (root != other.root) {
                recursive_delete_(root);
                root = recursive_copy_node_(other.root);
            }
            return *this;
        }

        class iterator {
         private:
            const AVLNode<T>* root;
            const AVLNode<T>* current;

         public:
            iterator() : root(nullptr), current(nullptr) {}

            iterator(const AVLNode<T>* rt, const AVLNode<T>* cur) : iterator() {
                root = rt;
                current = cur;
            }

            const T& operator*() const {
                return current->value;
            }

            const T* operator->() const {
                return &current->value;
            }

            iterator& operator++() {
                if (!current) {
                    current = leftmost_(root);
                    return *this;
                }
                if (current->right) {
                    current = leftmost_(current->right);
                } else {
                    while (current->parent && current->parent->right == current) {
                        current = current->parent;
                    }
                    current = current->parent;
                }
                return *this;
            }

            iterator operator++(int) {
                auto tmp(*this);
                ++(*this);
                return tmp;
            }

            iterator& operator--() {
                if (!current) {
                    current = rightmost_(root);
                    return *this;
                }
                if (current->left) {
                    current = rightmost_(current->left);
                } else {
                    while (current->parent && current->parent->left == current) {
                        current = current->parent;
                    }
                    current = current->parent;
                }
                return *this;
            }
            
            iterator operator--(int) {
                auto tmp(*this);
                --(*this);
                return tmp;
            }

            bool operator==(const iterator& other) const {
                return current == other.current && root == other.root;
            }

            bool operator!=(const iterator& other) const {
                return !(*this == other);
            }
        };

        iterator lower_bound(const T& value) const {
            return iterator(root, lower_bound_(root, value));
        }

        iterator begin() const {
            return iterator(root, leftmost_(root));
        }

        iterator end() const {
            return iterator(root, nullptr);
        }

        void insert(const T& value) {
            if (root == nullptr) {
                root = new AVLNode<T>(value);
            } else {
                insert_(root, value);
            }
        }

        void erase(const T& value) {
            auto node = find_(root, value);
            erase_(node);
        }

        iterator find(const T& value) const {
            return iterator(root, const_find_(root, value));
        }

        bool empty() const {
            return size() == 0;
        }

        size_t size() const {
            return get_size_(root);
        }

        ~Set() {
            recursive_delete_(root);
            root = nullptr;
        }
    };
}

template <typename T>
using Set = NevMem::Set<T>;

#endif