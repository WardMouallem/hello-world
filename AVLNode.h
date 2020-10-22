//
// Created by ward_ on 10/3/2020.
//
#include<new>
#include<iostream>
#ifndef MYAVLTREE_AVLNODE_H
#define MYAVLTREE_AVLNODE_H


template <class T>
class AVLNode {
public:
    int key;
    T* value;
    AVLNode<T>* parent;
    AVLNode<T>* left_son;
    AVLNode<T>* right_son;
    int height;
    int additional_info; // assuming for now it's the size of the subtree
    int balance_factor;


    AVLNode (int key, T *value, AVLNode<T> *parent= nullptr, AVLNode<T> *left_son = nullptr,
            AVLNode<T> *right_son= nullptr,int height=0,int additional_info=1, int balance_factor=0) : key(key), value(value), parent(parent),
            left_son(left_son), right_son(right_son),height(height), additional_info(additional_info), balance_factor(balance_factor) {}

     ~AVLNode() {
        parent= nullptr;
        left_son= nullptr;
        right_son= nullptr;
        delete value;
     }

    bool operator==(const AVLNode &node) const {
        return key == node.key;
    }

    bool operator!=(const AVLNode &node) const {
        return !(node == *this);
    }

    bool operator<(const AVLNode &node) const {
        return key < node.key;
    }

    bool operator>(const AVLNode &node) const {
        return node < *this;
    }

    bool operator<=(const AVLNode &node) const {
        return !(node < *this);
    }

    bool operator>=(const AVLNode &node) const {
        return !(*this < node);
    }
};
#endif //MYAVLTREE_AVLNODE_H