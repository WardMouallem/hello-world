//
// Created by ward_ on 10/3/2020.
//
/**
 * Generic AVL Tree
 * the tree is implemented as ranked tree. for a regular AVL tree, make the "idditional_info" field a note.
 * the following functions are available:
 * Constructor       - Creates a new empty AVL Tree.
 * Destructor        - Deletes an existing AVL Tree.
 *
 * max               - returns the maximum between two integers.
 *
 * setBalance        - updates the height balance of the received node based
 *                        on the equation height(leftSon)-height(RightSon)
 *
 * RightRotate       - executes an LL Rotation. (node is B)
 *
 * LeftRotate        - executes an RR Rotation. (node is B)
 *
 * LRRotate          - executes an RR Rotation and then an LL Rotation.
 *
 * RLRotate          - executes an LL Rotation and then an RR Rotation.
 *
 * reBalance         - rearranges the AVL tree to be balanced after adding
 *                     or removing a node.
 *
 * Find              - checks if the AVL tree contains a node with the received key.
 *
 * FindAndReturn     - checks if the AVL tree contains a node with the received
 *                      key and returns a pointer.
 *
 * Find_aux          - Finds a node in the AVL tree based on the matching data/value
 *                     and returns a pointer to the node.
 *
 * add               - creates a new node with the received parameters, adds
 *                     it to the AVL tree and reorganizes the tree to
 *                     keep it balanced. returns true if added successfully.
 *
 * updateParents     - given a root of an avl tree with only pointing down arrows,
 *                     it updates the parent of each node in the tree.
 *
 * Delete            - Deletes a node in the list that contains the same data/value as
 *                     the received one. returns true if the node is found and
 *                     deleted.
 *
 * DeleteByPointer   - Deletes a node in the list that matches the node pointed
 *                     at by the received pointer, and reorganizes the tree to
 *                     keep it balanced.
 *
 * Size              - returns the size of the AVL tree in the received pointer
 *                     and returns true.
 *
 * ClearTree         - Deletes all the left and right subroots of the given node
 *                     and then deletes the node itself.DOES NOT maintain the tree balanced
 *
 * inorder           - executes an inorder tour over the AVL tree nodes and returns
 *                     (god knows what , ask fadi)
 *
 * PrintTree         - prints the tree as how it should look graphically.
 */

#ifndef MYAVLTREE_AVLTREE_H
#define MYAVLTREE_AVLTREE_H

#include "AVLNode.h"

using std::cout;
using std::endl;

template<class T>
class AVLTree {
public:
    AVLNode<T> *root;
    int size;
    int additional_info_for_tree; // for example, the key of the node with the max value.

    //*********************************************************************
    explicit AVLTree(AVLNode<T> *root = nullptr, int size = 0, int additional_info_for_tree = -1) : root(root),
                                            size(size),additional_info_for_tree(additional_info_for_tree) {}
    //*********************************************************************
    int abs(int x) {
        if (x < 0) return -x;
        return x;
    }
    //*********************************************************************
    int max(int a, int b) {
        if (a > b) return a;
        if (a < b) return b;
        // else they are equal, return first parameter.
        return a;
    }
    //*********************************************************************
    /**
     * returns the index(+1) of the node with the matching key if it was in a sorted array
     * @param key
     * @return index+1 in the sorted array
     */
    int Rank(int key) {
        int r = 0;
        AVLNode<T> *current = root;
        if (!root) return -1;
        while (true) {
            if (!current) return -1;
            if (key > current->key) {
                int left_size = 0;
                if (current->left_son) {
                    left_size = current->left_son->additional_info;
                }
                r = r + left_size + 1;
                current = current->right_son;
            } else if (key < current->key) {
                current = current->left_son;
            } else {// there is a match
                int left_size = 0;
                if (current->left_son) {
                    left_size = current->left_son->additional_info;
                }
                r = r + left_size + 1;
                return r;
            }
        }

    }
    //*********************************************************************
    /**
     * assuming the tree was sorted in an array, the algorithm returns the node which is in the received index of the array.
     * @param index
     * @return ptr to the matching node with the index.
     */
    AVLNode<T> *Select(int index) {
        AVLNode<T> *current = root;
        if (!root) return nullptr;
        while (true) {
            if (!current) return nullptr;
            if (current->left_son && current->left_son->additional_info > (index - 1)) {
                current = current->left_son;
            } else if (current->left_son && current->left_son->additional_info == (index - 1) || index == 1) {
                return current;
            } else if (current->right_son) {
                index = index - (current->left_son->additional_info) - 1;
                current = current->right_son;
            }
        }
    }
    //*********************************************************************
    /**
     * prints the tree for graphic visualisation
     * @param ptr - root of the subtree to be printed.
     * @param level
     */
    void print_tree(AVLNode<T> *ptr, const int &level) {
        if (ptr != nullptr) {
            print_tree(ptr->right_son, level + 1);
            std::cout << std::endl;
            if (ptr == this->root) {
                std::cout << "Root->";
            }
            for (int i = 0; i < level && ptr != root; i++)
                std::cout << "   ";
            std::cout << ptr->key << "(" << ptr->additional_info << ")";;
            print_tree(ptr->left_son, level + 1);
        }
    }
    //*********************************************************************
    /**
     * clears the tree by doing a post-order traversal and deleting the nodes.
     * @param node - the root of the subtree to be cleared.
     */
    void ClearTree(AVLNode<T> *node) {
        if (!node) return;
        ClearTree(node->left_son);
        ClearTree(node->right_son);
        delete node;
    }
    //*********************************************************************
    /**
     * the AVL Tree Destructor. clears all the nodes and resets the tree fields.
     */

    ~AVLTree() {
        ClearTree(root);
        size = 0;
        additional_info_for_tree = -1;
        // make sure that the root is also cleared.
    }
    //*********************************************************************
    /**
     * updates the height and the balance factor of the received node.
     * @param node
     */
    void UpdateBalance(AVLNode<T> *node) {
        AVLNode<T> *left = node->left_son;
        AVLNode<T> *right = node->right_son;
        if (left && right) {
            node->height = max(left->height, right->height) + 1;
            node->balance_factor = (left->height) - (right->height);
        } else if (left) {
            node->height = left->height + 1;
            node->balance_factor += 1;
        } else if (right) {
            node->height = right->height + 1;
            node->balance_factor -= 1;
        } else { // it's a leaf
            node->height = 0;
            node->balance_factor = 0;
        }
        if (!(node->parent)) {
            root = node;
        }
    }
    //*********************************************************************
    /**
     * updates the "additional_info" field in the received node. (in this case, the size of the subtree)
     * @param node
     */
    // in this case the info is the size of the subtree of the node
    void UpdateInfo(AVLNode<T> *node) {
        AVLNode<T> *left = node->left_son;
        AVLNode<T> *right = node->right_son;
        if (left && right) {
            node->additional_info = left->additional_info + right->additional_info + 1;
        } else if (left && !right) {
            node->additional_info = left->additional_info + 1;
        } else if (!left && right) {
            node->additional_info = right->additional_info + 1;
        } else node->additional_info = 1;
        UpdateBalance(node);
    }
    //*********************************************************************
    /**
     * performs a rotation to the left, also known as RRRotation. (for better understanding, draw the pointers)
     * @param node - the node with balance factor of 2.(the A node in the mevne tutorial slide)
     * VARIABLE B - the node with the balance factor of 1.(the B node in the mevne tutorial slide)
     * @return
     */
    AVLNode<T> *LeftRotate(AVLNode<T> *node) {
        AVLNode<T> *B = node->right_son;
        B->parent = node->parent;
        node->right_son = B->left_son;
        if (node->right_son != nullptr) {
            node->right_son->parent = node;
            B->left_son = node;
            node->parent = B;
        } else {
            B->left_son = node;
            node->parent = B;
        }
        if (B->parent != nullptr) {
            if (B->parent->right_son == node) {
                B->parent->right_son = B;
            } else {
                B->parent->left_son = B;
            }
        }
        UpdateInfo(node);
        UpdateInfo(B);
        return B;
    }
    //*********************************************************************
    /**
     * performs a rotation to the right, also known as LLRotation. (for better understanding, draw the pointers)
     * @param node - the node with balance factor of 2. (B node in the mevne tutorial slide)
     * VARIABLE A - the node with the balance factor of 1.(A node in the mevne tutorial slide)
     * @return
     */
    AVLNode<T> *RightRotate(AVLNode<T> *node) {
        AVLNode<T> *A = node->left_son;
        A->parent = node->parent;
        node->left_son = A->right_son;

        if (node->left_son != nullptr) {
            node->left_son->parent = node;
            A->right_son = node;
            node->parent = A;
        } else {
            A->right_son = node;
            node->parent = A;
        }
        if (A->parent != nullptr) {
            if (A->parent->right_son == node) {
                A->parent->right_son = A;
            } else {
                A->parent->left_son = A;
            }
        }
        UpdateInfo(node);
        UpdateInfo(A);
        return A;
    }
    //*********************************************************************
    /**
     * performs a rotation to the left (RR) followed with a rotation to the right (LL)
     * @param node - the node with a balance factor of 2 or -2.
     * @return
     */
    AVLNode<T> *LRRotate(AVLNode<T> *node) {
        node->left_son = LeftRotate(node->left_son);
        return RightRotate(node);
    }
    //*********************************************************************
    /**
     * performs a rotation to the right (LL) followed with a rotation to the left (RR)
     * @param node - the node with a balance factor of 2 or -2.
     * @return
     */
    AVLNode<T> *RLRotate(AVLNode<T> *node) {
        node->right_son = RightRotate(node->right_son);
        return LeftRotate(node);
    }
    //*********************************************************************
    /**
     * recursively updates the height and balance factor of the given node and up, and performs the necessary rotations.
     * @param node
     */
    void UpdateBalanceAndFix(AVLNode<T> *node) { // checks for the current subtree if it's balanced, and if not, then fix it.
        if (!node) return;
        UpdateInfo(node);
        AVLNode<T> *stam = nullptr;
        switch (node->balance_factor) {
            case (2) :
                (node->left_son && node->left_son->balance_factor >= 0) ? stam = RightRotate(node) : stam = LRRotate(
                        node);
                break;
            case (-2) :
                (node->right_son && node->right_son->balance_factor <= 0) ? stam = LeftRotate(node) : stam = RLRotate(
                        node);
                break;
        }
        UpdateBalanceAndFix(node->parent);
    }
    //*********************************************************************
    /**
     * checks if there is an exiting node with the same input key, and returns a pointer to it if found, otherwise nullptr.
     * @param key
     * @return a pointer to the node, else, nullptr.
     */
    AVLNode<T> *find(int key) {
        if (!root) {
            return nullptr;
        }
        AVLNode<T> *current = root;
        while (true) {
            if (key < current->key && current->left_son) {
                current = current->left_son;
                continue;
            } else if (key > current->key && current->right_son) {
                current = current->right_son;
                continue;
            } else if (key == current->key) {
                return current;
            } else return nullptr;
        }
    }
    //*********************************************************************
    /**
     * given a key and a value, find the place that the new node should take, create one and add it.
     * @param key
     * @param value
     */
    void AddNode(int key, T *value) {
        AVLNode<T> *current = root;
        if (root == nullptr) {
            root = new AVLNode<T>(key, value);
            size++;
            // NOTE: update info here (like additional_info_for_tree) if needed before exiting.
            return;
        }
            // NOTE:if you need to update the additional_tree_info field, add a line here. or at the end of the function.
        else
            while (true) {
                // before going left, check that there is a node, if not, update it to point to the new_node.
                if (key < current->key) {
                    if (current->left_son) {
                        current = current->left_son;
                    } else { // if the current node is a leaf with empty left son, add the new node there.
                        AVLNode<T> *new_node = new AVLNode<T>(key, value);
                        current->left_son = new_node;
                        new_node->parent = current;
                        break;
                    }
                }
                    // before going right, check that there is a node, if not, update it to point to the new_node.
                else if (key > current->key) {
                    if (current->right_son) {
                        current = current->right_son;
                    } else { // if the current node is a leaf with empty right son
                        AVLNode<T> *new_node = new AVLNode<T>(key, value);
                        current->right_son = new_node;
                        new_node->parent = current;
                        break;
                    }
                } else if (key == current->key) {
                    cout
                            << "this message is received from the AddNode function because the keys of the nodes matched, you decide what to do here in the future"
                            << endl;
                    return;
                }
            }
        // at this point, the current node is the parent of the new node ( or in case of equality, current = new)
        size++;
        UpdateBalanceAndFix(current);
    }
    //*********************************************************************
    /**
     * given a random and a leaf node, the function switches between them by adjusting their pointers.
     */
    AVLNode<T> *SwitchNodeWithLeaf(AVLNode<T> *matching_node, AVLNode<T> *leaf_node) {
        // in case the matching node is also a leaf node:
        if (!(matching_node->right_son) && !(matching_node->left_son)) {
            return matching_node;
        }
        AVLNode<T> **matching_parent = &(matching_node->parent);
        AVLNode<T> **leaf_parent = &(leaf_node->parent);
        // if node to be deleted is the root:
        if (!(matching_node->parent)) {
            matching_node->parent = leaf_node->parent;
            leaf_node->parent = nullptr;
            leaf_node->parent->left_son = matching_node;
        }
            // if the node to be deleted is a right son:
        else if (matching_node == (*matching_parent)->right_son) {
            // ********* switching pointers with parents
            matching_node->parent->right_son = leaf_node;
            leaf_node->parent->left_son = matching_node;
            matching_node->parent = (*leaf_parent);
            leaf_node->parent = (*matching_parent);
        }
            // if the node to be deleted is a left son:
        else if (matching_node == (*matching_parent)->left_son) {
            // ********* switching pointers with parents
            matching_node->parent->left_son = leaf_node;
            leaf_node->parent->left_son = matching_node;
            matching_node->parent = (*leaf_parent);
            leaf_node->parent = (*matching_parent);
        }
        //*****************************************************
        // ********* switching pointers with sons *************
        leaf_node->right_son = matching_node->right_son;
        if (matching_node->right_son) {
            matching_node->right_son->parent = leaf_node;
        }
        leaf_node->left_son = matching_node->left_son;
        if (matching_node->left_son) {
            matching_node->left_son->parent = leaf_node;
        }
        matching_node->left_son = nullptr;
        matching_node->right_son = nullptr;
        return matching_node;
    }
    //*********************************************************************
    /**
     * given a key, find the matching node and remove it. if the additional info (copies of node) is over 1, then decrease it without deleting the node.
     * @param key
     * @param value
     */
    void RemoveNode(int key) {
        AVLNode<T> *current = root;
        AVLNode<T> *matching_node = nullptr;
        AVLNode<T> *leaf_node = nullptr;
        if (root == nullptr) return;
            // search for the matching node
        else
            while (true) {
                // before going left, check that there is a node, if not, return.
                if (key < current->key) {
                    if (current->left_son) {
                        current = current->left_son;
                    } else return;
                }
                    // before going right, check that there is a node, if not, return.
                else if (key > current->key) {
                    if (current->right_son) {
                        current = current->right_son;
                    } else return;
                } else if (key == current->key) {
                    matching_node = current;
                    break;
                }
            }
        // now look for the inorder successor of the matching node. (one right and all the way down to the left)
        AVLNode<T> *parent = matching_node->parent;
        bool need_to_switch = false;
        if (matching_node->right_son) {
            need_to_switch = true;
            current = current->right_son;
            while (current->left_son) {
                current = current->left_son;
            }
            leaf_node = current;
        }
        if (need_to_switch) {
            AVLNode<T> *to_delete = SwitchNodeWithLeaf(matching_node, leaf_node);
            parent = to_delete->parent;
            if (parent->right_son && parent->right_son == to_delete) parent->right_son = nullptr;
            if (parent->left_son && parent->left_son == to_delete) parent->left_son = nullptr;
            delete to_delete;
            UpdateBalanceAndFix(parent);
        } else {
            if (parent && parent->right_son && parent->right_son == matching_node) parent->right_son = nullptr;
            if (parent && parent->left_son && parent->left_son == matching_node) parent->left_son = nullptr;
            if (root == matching_node) root = nullptr;
            delete matching_node;
            UpdateBalanceAndFix(parent);
        }
        size--;
    }
    //*********************************************************************
    /**
     * performs an in-order traversal and performs the necessary action along the way.
     * @param node - the current node.
     */
    int InOrderTraversal(AVLNode<T> *node, int *arr, int size, int index = 0) {
        if (!node) return index;
        index = InOrderTraversal(node->left_son, arr, size, index);
        // do something;
        cout << node->key << endl;
        if (index >= size) return index;
        arr[index++] = node->key;
        return InOrderTraversal(node->right_son, arr, size, index);
    }
    //*********************************************************************
    /**
     * performs an pre-order traversal and performs the necessary action along the way.
     * @param node - the current node.
     */
    void PreOrderTraversal(AVLNode<T> *node) {
        if (!node) return;
        // do something;
        PreOorderTraversal(node->left_son);
        PreOorderTraversal(node->right_son);
    }
    //*********************************************************************
    /**
     * performs an in-order traversal and performs the necessary action along the way.
     * @param node - the current node.
     */
    void PostOrderTraversal(AVLNode<T> *node) {
        if (!node) return;
        PostOrderTraversal(node->left_son);
        PostOrderTraversal(node->right_son);
        // do something;
    }
    //*********************************************************************
};

#endif //MYAVLTREE_AVLTREE_H
