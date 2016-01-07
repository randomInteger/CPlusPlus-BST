//
//  main.cpp
//  C++ClassBST
//
//  Tester for the BST class implemented in BST.hpp.
//
//
//  Created by Christopher Gleeson on 12/21/15.
//  Copyright © 2015 Christopher Gleeson. All rights reserved.
//

#include <iostream>
#include <stack>
#include <cassert>
#include <exception>
#include "BST.hpp"

int main(int argc, const char * argv[]){
    //create a tree object
    BST<int> *myTree;
    
    try {
        myTree = new BST<int>;
    } catch (std::bad_alloc& exc) {
        std::cerr << "ERROR: could not allocate storage.  Terminating!\n";
        std::terminate();
    }
    
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

    std::cout << "Destroying tree.\n";
    delete myTree;
    return 0;
}










