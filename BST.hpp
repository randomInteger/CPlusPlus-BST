//
//  BST.hpp
//
//  A class based implementation of a simple binary search tree (unbalanced).
//  value data type is a template, any numeric type can be used.
//
//  Created by Christopher Gleeson on 1/7/16.
//  Copyright © 2016 Christopher Gleeson. All rights reserved.
//

#ifndef BST_h
#define BST_h

template <typename T>
struct leaf {
    T value;
    struct leaf<T> * left;
    struct leaf<T> * right;
};

template <typename T>
class BST {
private:
    
    struct leaf<T> *root;
    
    //Private helper to destroy the tree
    void _destroy_tree(struct leaf<T> *current){
        if(current == NULL){
            return;
        }
        _destroy_tree(current->left);
        _destroy_tree(current->right);
        delete current;
    }
    
    //Private helper to insert a node
    void _insert_hlpr(struct leaf<T> *current, T value){
        if(value < current->value){
            if(current->left == NULL){
                try {
                    current->left = new struct leaf<T>;
                } catch (std::bad_alloc& exc) {
                    std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
                    std::terminate();
                }
                current->left->value = value;
                current->left->left = NULL;
                current->left->right = NULL;
                std::cout << "Inserting " << value << " into tree.\n";
            }else{
                _insert_hlpr(current->left, value);
            }
        }else if(value > current->value){
            if(current->right == NULL){
                try {
                    current->right = new struct leaf<T>;
                } catch (std::bad_alloc& exc) {
                    std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
                    std::terminate();
                }
                current->right->value = value;
                current->right->left = NULL;
                current->right->right = NULL;
                std::cout << "Inserting " << value << " into tree.\n";
            }else{
                _insert_hlpr(current->right, value);
            }
        }
        //there is nothing left to do if we reach here, node already exists
    }
    
    //Private helper to find a node
    struct leaf<T> * _search_hlpr(struct leaf<T> * current, T value){
        if(!current){
            //do nothing
        }else if(value < current->value){
            current = _search_hlpr(current->left, value);
        }else if(value > current->value){
            current = _search_hlpr(current->right, value);
        }
        return current;
    }
    
    //Private helper to remove a node.
    struct leaf<T> * _remove_hlpr(struct leaf<T> *current, T value){
        if(!current){
            return current;
        }else if(value < current->value){
            current->left = _remove_hlpr(current->left, value);
        }else if(value > current->value){
            current->right = _remove_hlpr(current->right, value);
        }else{
            //current->value == value
            if(!current->right && !current->left){
                //Case 1: Current has no children
                delete(current);
                current = nullptr;
            }else if(current->left && !current->right){
                //Case 2:  Current has only a left child
                delete(current);
                current = current->left;
            }else if(current->right && !current->left){
                //Case 3:  Current has only a right child
                delete(current);
                current = current->right;
            }else if(current->left && current->right){
                //Case 4:  Current has both left and right children
                auto iop = _inorder_pred_hlpr(current);
                current->value = iop->value;
                current->left = _remove_hlpr(current->left, iop->value);
            }
        }
        return current;
    }
    
    //Private helper to find the in-order predecessor, needed for deletion
    struct leaf<T> * _inorder_pred_hlpr(struct leaf<T> *current){
        //can be called on a node with no children
        if(!current->left && !current->right){
            return nullptr;
        }else{
            auto iop = current->left;
            while(iop->right != NULL){
                iop = iop->right;
            }
            return iop;
        }
    }
    
    //Private helper for printing the tree inorder, recursive.
    void _print_inorder_hlpr(struct leaf<T> *current){
        if(current == NULL) return;
        _print_inorder_hlpr(current->left);
        std::cout << current->value << "\n";
        _print_inorder_hlpr(current->right);
    }
    
    //Private helper for printing the tree inorder, iterative.
    void _print_it_inorder_hlpr(){
        if(root == NULL) {
            std::cout << "Tree is empty!\n";
            return;
        }
        std::stack <struct leaf<T> *> myStack;
        struct leaf<T> *current = root;
        //Inorder iterative traversal algorithm:
        //0.  set current to root.
        //1.  while stack is not empty OR current != NULL
        //2.  if current != NULL push it onto the stack and set current = current->left
        //3.  then, pop the stack, print the value and set
        //current to the the right child of the popped element
        //4.  continue this loop until the stack is empty.
        while(!myStack.empty() || current != NULL){
            if(current != NULL){
                myStack.push(current);
                current = current->left;
                continue;
            }
            struct leaf<T> *popped = myStack.top();
            std::cout << popped->value << "\n";
            current = popped->right;
            myStack.pop();
        }
    }
public:
    BST();
    ~BST();
    void insert(T value);
    bool search(T value);
    void remove(T value);
    void print_tree_inorder();
    void print_tree_it_inorder();
    void inorder_predecessor(T value); //for testing only
};

//Constructor
template <typename T>
BST<T>::BST(){
    root = NULL;
}

//Destructor
template <typename T>
BST<T>::~BST(){
    _destroy_tree(root);
}

//Inserts a value into the tree
//Error:  badalloc causes terminate.
template <typename T>
void BST<T>::insert(T value){
    if(root != NULL){
        _insert_hlpr(root, value);
        return;
    }else{
        try {
            root = new struct leaf<T>;
        } catch (std::bad_alloc& exc) {
            std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
            std::terminate();
        }
        root->left = NULL;
        root->right = NULL;
        root->value = value;
        std::cout << "Inserting " << value << " into tree.\n";
    }
}

//Search for a value in the tree
template <typename T>
bool BST<T>::search(T value){
    //search for value in the tree and return a
    //pointer to the node containing the value,
    //else return a NULL pointer
    if(root == NULL){
        return false;
    }else{
        auto *found = _search_hlpr(root, value);
        if(found != NULL){
            std::cout << "found " << found->value << "\n";
            return true;
        }else{
            return false;
        }
    }
}

//Remove a value from the tree
template <typename T>
void BST<T>::remove(T value){
    if(root){
        root = _remove_hlpr(root, value);
    }else{
        std::cout << "Remove called on an empty tree.\n";
    }
}

//Print the tree inorder, recursive
template <typename T>
void BST<T>::print_tree_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        _print_inorder_hlpr(root);
    }
}

//Print the tree inorder, iterative.
template <typename T>
void BST<T>::print_tree_it_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        _print_it_inorder_hlpr();
    }
}

//Find the inorder predecessor of a value in the tree if it exists.
template <typename T>
void BST<T>::inorder_predecessor(T value){
    auto found = _search_hlpr(root, value);
    auto iop = _inorder_pred_hlpr(found);
    
    if(iop == NULL){
        std::cout << "There is no inorder predecessor for value of " << value << "\n";
        return;
    }else{
        std::cout << "Value of inorder predecessor for value of " << value << " is " << iop->value << "\n";
        return;
    }
}

#endif /* BST_h */
