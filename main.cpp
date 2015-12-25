//
//  main.cpp
//  C++ClassBST
//
//  Basic BST class implementation with insert, search, remove, recursive inorder print,
//  and iterative inorder print.
//
//  The data type of value in the leaf struct is templateized.
//
//  Created by Christopher Gleeson on 12/21/15.
//  Copyright © 2015 Christopher Gleeson. All rights reserved.
//

#include <iostream>
#include <stack>
#include <cassert>

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
    void destroy_tree(struct leaf<T> *lf){
        if(lf == NULL){
            return;
        }
        destroy_tree(lf->left);
        destroy_tree(lf->right);
        delete lf;
    }
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
    void insert_hlpr(T key, struct leaf<T> *lf){
        if(key < lf->value){
            if(lf->left == NULL){
                lf->left = new struct leaf<T>;
                lf->left->value = key;
                lf->left->left = NULL;
                lf->left->right = NULL;
                std::cout << "Inserting " << key << " into tree.\n";
            }else{
                insert_hlpr(key, lf->left);
            }
        }else if(key > lf->value){
            if(lf->right == NULL){
                lf->right = new struct leaf<T>;
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
    void print_inorder_hlpr(struct leaf<T> *lf){
        if(lf == NULL) return;
        print_inorder_hlpr(lf->left);
        std::cout << lf->value << "\n";
        print_inorder_hlpr(lf->right);
    }
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

template <typename T>
BST<T>::BST(){
    root = NULL;
}

template <typename T>
BST<T>::~BST(){
    destroy_tree(root);
}

template <typename T>
void BST<T>::insert(T key){
    if(root != NULL){
        insert_hlpr(key, root);
        return;
    }
    root = new struct leaf<T>;
    root->left = NULL;
    root->right = NULL;
    root->value = key;
    std::cout << "Inserting " << key << " into tree.\n";
}

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

template <typename T>
void BST<T>::print_tree_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        print_inorder_hlpr(root);
    }
}

template <typename T>
void BST<T>::print_tree_it_inorder(){
    if(root == NULL){
        std::cout << "Tree is empty!\n";
    }else{
        print_it_inorder_hlpr();
    }
}

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

int main(int argc, const char * argv[]){
    //create a tree object
    BST<int> *myTree = new BST<int>;
    
    //print the empty tree
    myTree->print_tree_inorder();
    
    //add some elements
    myTree->insert(8);
    myTree->insert(4);
    myTree->insert(2);
    myTree->insert(-1);
    myTree->insert(12);
    myTree->insert(14);
    myTree->insert(-16);
    myTree->insert(3);
    myTree->insert(5);
    myTree->insert(10);
    
    //print the tree recursively
    std::cout << "Print of tree, inorder, recursive:\n";
    myTree->print_tree_inorder();
    
    //print the tree iteratively
    std::cout << "Print of tree, inorder, iterative:\n";
    myTree->print_tree_it_inorder();
    
    //search for a node
    std::cout << "Searching for node with value 4, expect success.\n";
    int skey = 4;
    bool result = myTree->search(skey);
    if(!result){
        std::cout << "Searched for key of " << skey << ", key NOT found!\n";
    }else{
        std::cout << "Found key of " << skey << " successfully!\n";
    }
    std::cout << "Searching for node with value 13, expect failure.\n";
    skey = 13;
    result = myTree->search(skey);
    if(!result){
        std::cout << "Searched for key of " << skey << ", key NOT found!\n";
    }else{
        std::cout << "Found key of " << skey << " successfully!\n";
    }
    
    //test inorder predecessor
    std::cout << "Testing checks for inorder predecessor, 8,4,2,12 expect pass, 14 expect fail.\n";
    myTree->inorder_predecessor(8);
    myTree->inorder_predecessor(4);
    myTree->inorder_predecessor(2);
    myTree->inorder_predecessor(12);
    myTree->inorder_predecessor(14);
    
    //test find parent
    std::cout << "Testing find parent for node with value 4, expect to find parent with value 8.\n";
    myTree->locate_parent(4);
    
    //test deletion
    std::cout << "Testing node deletion of node with value 14, expect success.\n";
    myTree->remove(14);
    myTree->print_tree_inorder();

    delete myTree;
    return 0;
}










