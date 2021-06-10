#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <set>
#include <unordered_set>
#include <cassert>
#include <typeinfo>
#include "bst.hpp"
#include "node.hpp"
#include "iterator.hpp"

int main(){
    Bst<int, int> albero;
    for(int i = 0; i < 100; i++){
        albero.emplace(i, i*2);
    } 
    std::cout << albero;
    albero.balance(); 
    //std::cout << *albero.root;
    albero.clear();
    std::cout << albero;
}   

// valgrind --leak-check=full          --show-leak-kinds=all          --track-origins=yes          --verbose          --log-file=valgrind-out.txt    ./main.exe 