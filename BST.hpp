//
//  BST.hpp
//
//  A class based implementation of a simple binary search tree (unbalanced).
//  Key data type is a template, any numeric type can be used.
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
    void destroy_tree(struct leaf<T> *lf){
        if(lf == NULL){
            return;
        }
        destroy_tree(lf->left);
        destroy_tree(lf->right);
        delete lf;
    }
    
    //Private helper to find the parent of a given node
    struct leaf<T> * find_parent(struct leaf<T> *child, struct leaf<T> * current){
        if(current == NULL){
            return NULL;
        }else if(current->left == child || current->right == child){
            return current;
        }else if(child->value < current->value){
            return find_parent(child, current->left);
        }else if(child->value > current->value){
            return find_parent(child, current->right);
        }
        return NULL;
    }
    
    //Private helper to insert a node
    void insert_hlpr(T key, struct leaf<T> *lf){
        if(key < lf->value){
            if(lf->left == NULL){
                try {
                    lf->left = new struct leaf<T>;
                } catch (std::bad_alloc& exc) {
                    std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
                    std::terminate();
                }
                lf->left->value = key;
                lf->left->left = NULL;
                lf->left->right = NULL;
                std::cout << "Inserting " << key << " into tree.\n";
            }else{
                insert_hlpr(key, lf->left);
            }
        }else if(key > lf->value){
            if(lf->right == NULL){
                try {
                    lf->right = new struct leaf<T>;
                } catch (std::bad_alloc& exc) {
                    std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
                    std::terminate();
                }
                lf->right->value = key;
                lf->right->left = NULL;
                lf->right->right = NULL;
                std::cout << "Inserting " << key << " into tree.\n";
            }else{
                insert_hlpr(key, lf->right);
            }
        }else{
            //key is already in the tree, do nothing
        }
    }
    
    //Private helper to find a node
    struct leaf<T> * search_hlpr(T key, struct leaf<T> * lf){
        if(lf == NULL){
            return lf;
        }else if(key == lf->value){
            return lf;
        }else if(key < lf->value){
            return search_hlpr(key, lf->left);
        }else if(key > lf->value){
            return search_hlpr(key, lf->right);
        }
        return NULL;
    }
    
    //Private helper to remove a node.
    void remove_hlpr(struct leaf<T> *lf){
        //this function takes a node to be removed that is in the tree
        //cases:  1. node has no children, just remove the node.
        //2. node has one child, copy the value of child to node and delete child.
        //3. node has two children:  copy inorder predecessor into node, call delete
        //on the inorder predecessor, which will then hit case 1 or 2.
        //case 1
        if(lf->left == NULL && lf->right == NULL){
            //must set the parent's pointer to lf to null before we delete
            struct leaf<T> *parent = find_parent(lf, root);
            if(lf->value < parent->value){
                parent->left = NULL;
            }else{
                parent->right = NULL;
            }
            delete lf;
            return;
        }
        //case 2
        if(lf->left == NULL && lf->right != NULL){
            lf->value = lf->right->value;
            remove_hlpr(lf->right);
            return;
        }
        if(lf->left != NULL && lf->right == NULL){
            lf->value = lf->left->value;
            remove_hlpr(lf->left);
            return;
        }
        //case 3
        if(lf->left != NULL && lf->right != NULL){
            struct leaf<T> *iop = inorder_pred_hlpr(lf);
            if(iop!=NULL){
                lf->value = iop->value;
                remove_hlpr(iop);
                return;
            }else{
                //this should not be necessary, but its good to handle this case
                std::cerr << "Critical error in node deletion! Abort!\n";
                assert(iop != NULL);
            }
        }
    }
    
    //Private helper to find the in-order predecessor, needed for deletion
    struct leaf<T> * inorder_pred_hlpr(struct leaf<T> *lf){
        //to find the inorder predecessor we look to the left child
        //then traverse as far right as possible.  when we can no
        //longer traverse right, we are at the IOP.
        struct leaf<T> *iop = NULL;
        if(lf->left == NULL){
            return iop;
        }else{
            iop = lf->left;
        }
        while(iop->right != NULL){
            iop = iop->right;
        }
        return iop;
    }
    
    //Private helper for printing the tree inorder, recursive.
    void print_inorder_hlpr(struct leaf<T> *lf){
        if(lf == NULL) return;
        print_inorder_hlpr(lf->left);
        std::cout << lf->value << "\n";
        print_inorder_hlpr(lf->right);
    }
    
    //Private helper for printing the tree inorder, iterative.
    void print_it_inorder_hlpr(){
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
    void insert(T key);
    bool search(T key);
    void remove(T key);
    void print_tree_inorder();
    void print_tree_it_inorder();
    void inorder_predecessor(T key); //for testing only
    void locate_parent(T key); //for testing only
};

//Constructor
template <typename T>
BST<T>::BST(){
    root = NULL;
}

//Destructor
template <typename T>
BST<T>::~BST(){
    destroy_tree(root);
}

//Inserts a key into the tree
//Error:  badalloc causes terminate.
template <typename T>
void BST<T>::insert(T key){
    if(root != NULL){
        insert_hlpr(key, root);
        return;
    }
    try {
        root = new struct leaf<T>;
    } catch (std::bad_alloc& exc) {
        std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
        std::terminate();
    }
    root->left = NULL;
    root->right = NULL;
    root->value = key;
    std::cout << "Inserting " << key << " into tree.\n";
}

//Search for a key in the tree
template <typename T>
bool BST<T>::search(T key){
    //search for key in the tree and return a
    //pointer to the node containing the key,
    //else return a NULL pointer
    if(root == NULL){
        return false;
    }else{
        struct leaf<T> *found = search_hlpr(key, root);
        if(found != NULL){
            return true;
        }else{
            return false;
        }
    }
}

//Remove a key from the tree
template <typename T>
void BST<T>::remove(T key){
    if(!search(key)){
        std::cout << "Key not in tree, no removal possible.\n";
    }else{
        struct leaf<T> *rem = search_hlpr(key, root);
        std::cout << "Value to remove is " << rem->value << "\n";
        remove_hlpr(rem);
    }
}

//Print the tree inorder, recursive
template <typename T>
void BST<T>::print_tree_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        print_inorder_hlpr(root);
    }
}

//Print the tree inorder, iterative.
template <typename T>
void BST<T>::print_tree_it_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        print_it_inorder_hlpr();
    }
}

//Find the inorder predecessor of a key in the tree if it exists.
template <typename T>
void BST<T>::inorder_predecessor(T key){
    struct leaf<T> *found = search_hlpr(key, root);
    struct leaf<T> *iop = inorder_pred_hlpr(found);
    
    if(iop == NULL){
        std::cout << "There is no inorder predecessor for key of " << key << "\n";
        return;
    }else{
        std::cout << "Value of inorder predecessor for key of " << key << " is " << iop->value << "\n";
        return;
    }
}

//Find the parent of a key in the tree.
template <typename T>
void BST<T>::locate_parent(T key){
    struct leaf<T> * par = NULL;
    struct leaf<T> * child = search_hlpr(key, root);
    if(child != NULL){
        par = find_parent(child, root);
    }
    if(par != NULL){
        std::cout << "Found parent of node with value " << child->value << ", parent has value " << par->value << "\n";
    }
}


#endif /* BST_h */
