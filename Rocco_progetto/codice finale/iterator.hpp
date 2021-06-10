#pragma once
#include "bst.hpp"


template<class key_type, class value_type, class compare_type>
class Bst<key_type, value_type, compare_type>::iterator {

    using paio = std::pair<key_type, value_type>;

    public:

    Node* actual;

    iterator(Node* a){
        actual = a;
    }

    iterator operator++(){
        actual = actual -> succ;
        return *this;
    }

    value_type& operator*(){
        return actual -> get_value();
    }

    paio* operator->(){
        return &(actual -> get_data());
    }

    bool operator==(const iterator& i){
        return actual == i.actual;
    }

    bool operator!=(const iterator& i){
        return actual != i.actual;
    }

    paio& data(){
        return actual -> get_data();
    }
};

template <class key_type, class value_type, class compare_type>
class Bst<key_type, value_type, compare_type>::const_iterator {

    using paio = std::pair<const key_type, const value_type>; // il fatto che ci sia const nel tipo della key abbiamo un elemento immutabile.

    Node* actual;

    const_iterator(Node* a){
        actual = a;
    }

    const_iterator operator++(){
        actual = actual -> succ;
        return *this;
    }

    const value_type& operator*(){       // il fatto che ci sia la ref implica che possiamo scrivere: *iteratore = valore_value_type;
        return actual -> get_value();
    }

    const paio& operator->(){
        return actual -> get_data();
    }

    const paio& data() const {
        return actual -> get_data();
    }

    bool operator==(const const_iterator& i) const {//const li significa che all'interno del corpo della funzione non modifico gli attributi interni dellaclasse
        return actual == i.actual;
    }

    bool operator!=(const const_iterator& i) const {
        return actual != i.actual;
    }
};
