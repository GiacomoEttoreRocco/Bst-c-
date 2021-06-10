#pragma once

template <class key_type, class value_type, class compare_type = std::less<key_type>>
class Bst {

    compare_type comp;
    class Node;
    class iterator;
    class const_iterator;
    Node* root = nullptr;

public:
    Bst();
    Bst(std::pair<key_type, value_type> data);
    Bst(Bst&& other) noexcept;
    Bst(const Bst& other);
    ~Bst() noexcept;
    
    iterator find(const key_type& x)  noexcept;
    const_iterator find(const key_type& x) const noexcept;

    value_type& operator[](const key_type& x) noexcept;
    value_type& operator[](key_type&& x);

    void erase(const key_type& x);
    void clear();

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    
    template <typename... Types> // variadic templates; packing
    std::pair<iterator, bool> emplace(Types&&... args);
    std::pair<iterator, bool> insert(const std::pair<key_type, value_type>& data);
    std::pair<iterator, bool> insert(std::pair<key_type, value_type>&& data);

    void balance();

    template<typename K, typename V, typename C>
    friend
    std::ostream& operator<<(std::ostream& os, const Bst<K, V, C>& b);

    private:

    Node* helper(Node* sinistro, Node* centrale, Node* destro);
    bool equal(const key_type& n, const key_type& m);
    void copy_aux(Node* n);
    void erase(Node* to_delete);
    void balance_aux(size_t start, size_t end, const std::vector<std::pair<key_type, value_type>>& v);
    void inOrderPrint(Node* n, std::ostream& os) const;

};


template <class key_type, class value_type, class compare_type>
Bst<key_type, value_type, compare_type>::Bst(){
    //std::cout << "Empty constructor" << std::endl;
}

template <class key_type, class value_type, class compare_type>
Bst<key_type, value_type, compare_type>::Bst(std::pair<key_type, value_type> data){
    root = new Node(data);
}

template <class key_type, class value_type, class compare_type>
Bst<key_type, value_type, compare_type>::Bst(Bst&& other) noexcept{
    //std::cout << "Bst move constructor" << std::endl;
    root = other.root;
    other.root = nullptr;
}

template <class key_type, class value_type, class compare_type>
Bst<key_type, value_type, compare_type>::Bst(const Bst& other){
    //std::cout << "Bst deep copy constructor" << std::endl;
    copy_aux(other.root);
}


template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>::copy_aux(Node* n){
    insert(n -> get_data());
    if(n -> get_left() != nullptr){
        //std::cout << "left";
        copy_aux(n -> get_left());
    }
    if(n -> get_right() != nullptr){
        //std::cout << "right";
        copy_aux(n -> get_right());
    }
}

template <class key_type, class value_type, class compare_type>
Bst<key_type, value_type, compare_type>::~Bst() noexcept{
    //std::cout << "Bst destructor" << std::endl;
    clear();
}

template <class key_type, class value_type, class compare_type>
std::pair<typename Bst<key_type, value_type, compare_type>::iterator, bool> Bst<key_type, value_type, compare_type>::insert(const std::pair<key_type, value_type>& data){
    Node* local = root;
    if(root == nullptr){
        root = new Node(data);
        return {iterator(root), true};
    }else{
        while(local != nullptr){

            if(equal(data.first, local -> get_key())){
                return {iterator(local), false};
            }

            if(comp(data.first, local -> get_key())){
                if(local -> get_left() == nullptr){
                    Node* newNode = helper(local -> pred, new Node(data), local);
                    local -> set_left(newNode);
                    return {iterator(newNode), true};

                }else{
                    local = local -> get_left();
                }
            }else{
                if(local -> get_right() == nullptr){
                    Node* newNode = helper(local, new Node(data), local -> succ);
                    local -> set_right(newNode);
                    return {iterator(newNode), true};

                }else{
                    local = local -> get_right();
                }
            }
        }
    }

    return {iterator(nullptr), false};
}

template <class key_type, class value_type, class compare_type>
std::pair<typename Bst<key_type, value_type, compare_type>::iterator, bool> Bst<key_type, value_type, compare_type>::insert(std::pair<key_type, value_type>&& data){
    return insert(data);
}


template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::iterator Bst<key_type, value_type, compare_type>::find(const key_type& x) noexcept {
    Node* local = root;
    while(local != nullptr){
        if(equal(x, local -> get_key())){
            return iterator(local);
        }
        else if(comp(x, local -> get_key())){
            local = local -> get_left();
        }else{
            local = local -> get_right();
        }
    }
    return end();
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::const_iterator Bst<key_type, value_type, compare_type>::find(const key_type& x) const noexcept{
    Node* local = root;
    while(local != nullptr){
        if(equal(x, local -> get_key())){
            return const_iterator(local);
        }
        else if(comp(x, local -> get_key())){
            local = root -> get_left();
        }else{
            local = root -> get_right();
        }
    }
    return cend();
}

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>::erase(const key_type& x){
    iterator itr_to_delete = find(x);
    erase(itr_to_delete.actual);
}

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>::erase(Node* to_delete){
    if(to_delete != nullptr){
        if(to_delete -> is_leaf()){ // se non ha figli, elimina semplicemente il nodo.
            //std::cout << "caso erase su foglia" << std::endl;
            Node* succ = to_delete -> succ;
            Node* pred = to_delete -> pred; 
            if(pred != nullptr){
                pred -> succ = succ;
            }
            if(succ != nullptr){
                succ -> pred = pred;
            }
            if(to_delete == root){ //padre == nullptr
                root = nullptr;
            }else if(to_delete -> is_right()){ // verificichiamo se è un figlio sinistro o destro di suo padre
                to_delete -> parent -> set_right(nullptr);
            }else{
                to_delete -> parent -> set_left(nullptr);
            }

            //std::cout << "----------" << std::endl;
            delete(to_delete);
        }
        else if(to_delete -> get_left() != nullptr && to_delete -> get_right() == nullptr){ //se ha solo il figlio sinistro
            //std::cout << "caso erase figlio solo sx" << std::endl;
            if(to_delete -> parent != nullptr){ // controlla se ha il padre
                if(to_delete -> is_left()){
                    (to_delete -> parent) -> set_left(to_delete -> get_left());
                }else{
                    (to_delete -> parent) -> set_right(to_delete -> get_left());
                }
            }else{ // NON AVENDO IL PADRE ERA LA ROOT
                root = to_delete -> get_left();
                root -> parent = nullptr;
            }

            if(to_delete -> pred != nullptr){
                to_delete -> pred -> succ = to_delete -> succ;
            }
            if(to_delete -> succ != nullptr){
                to_delete -> succ -> pred = to_delete -> pred;
            }

            delete(to_delete);

        }else if(to_delete -> get_left() == nullptr && to_delete -> get_right() != nullptr){ // se ha solo il figlio destro
            //std::cout << "caso erase figlio solo dx" << std::endl;
            if(to_delete -> parent != nullptr){ // controlla se ha il padre
                if(to_delete -> is_right()){
                    (to_delete -> parent) -> set_left(to_delete -> get_right());
                }else{
                    (to_delete -> parent) -> set_right(to_delete -> get_right());
                }
            }else{ // NON AVENDO IL PADRE ERA LA ROOT
                root = to_delete -> get_right();
                root -> parent = nullptr;
            }
            if(to_delete -> pred != nullptr){
                to_delete -> pred -> succ = to_delete -> succ;
            }
            if(to_delete -> succ != nullptr){
                to_delete -> succ -> pred = to_delete -> pred;
            }
            
            delete(to_delete);

        }else if(to_delete -> get_left() != nullptr && to_delete -> get_right() != nullptr){ // ha entrambi i figli, di default ricorsione sul sinistro

            //std::cout << "caso erase 2 figli. Chiamata ricorsiva:" << std::endl;
            Node* pred = to_delete -> pred; 
            std::swap(to_delete -> get_key(), pred -> get_key());  
            std::swap(to_delete -> get_value(), pred -> get_value());
            erase(pred);
            }
        }
    }

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::iterator Bst<key_type, value_type, compare_type>::begin(){
    Node* local = root;
    if(root != nullptr){
        while(local -> get_left() != nullptr){
            local = local -> get_left();
        }
    }
    return iterator(local);
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::const_iterator Bst<key_type, value_type, compare_type>::begin() const {
    Node* local = root;
    if(root != nullptr){
        while(local -> get_left() != nullptr){
                local = local -> get_left();
            }
    }
    return const_iterator(local);
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::const_iterator Bst<key_type, value_type, compare_type>::cbegin() const {
    Node* local = root;
    if(root != nullptr){
        while(local -> get_left() != nullptr){
                local = local -> get_left();
            }
    }
    return const_iterator(local);
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::iterator Bst<key_type, value_type, compare_type>::end(){
    return nullptr;
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::const_iterator Bst<key_type, value_type, compare_type>::end() const{
    return nullptr;
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::const_iterator Bst<key_type, value_type, compare_type>::cend() const{
    return nullptr;
}

/*
void debug_stuff(){
    for(iterator i = begin(); i != end(); ++i){
        std::cout << "valore: " << i -> first << " " ;
        if(i.actual -> pred != nullptr){
            std::cout << "Pred: " << i.actual -> pred -> get_key() << " ";
        }else{
            std::cout << "Pred: nullo ";
        }
        if(i.actual -> succ != nullptr){
            std::cout << "Succ: " << i.actual -> succ -> get_key() << " ";
        }else{
            std::cout << "Succ: nullo ";
        }
        std::cout << std::endl;
    }
}
*/

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>::clear(){
    //deleteTree(root);
    while(root != nullptr){
        erase(root);
    }
}


template <class key_type, class value_type, class compare_type>
template <typename... Types> // variadic templates; packing
std::pair<typename Bst<key_type, value_type, compare_type>::iterator, bool>
Bst<key_type, value_type, compare_type>::emplace(Types&&... args){  
    return insert({std::forward<Types>(args)...}); //black magic 
}

template <class key_type, class value_type, class compare_type>
typename Bst<key_type, value_type, compare_type>::Node* Bst<key_type, value_type, compare_type>::helper(Node* sinistro, Node* centrale, Node* destro){
    if(sinistro != nullptr){
        sinistro -> succ = centrale;
    }

    if(destro != nullptr){
        destro -> pred = centrale;
    }

    centrale -> succ = destro;
    centrale -> pred = sinistro;

    return centrale;
}

template <class key_type, class value_type, class compare_type>
bool Bst<key_type, value_type, compare_type>:: equal(const key_type& n, const key_type& m){
    return (comp(n, m) == comp(m, n));
}

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>:: balance(){
    std::vector<std::pair<key_type, value_type>> temp; //alloc 
    for (iterator i = begin(); i != end(); ++i){
        temp.push_back(i.data());
    }
    clear();
    balance_aux(0, temp.size(), temp);
}

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>:: balance_aux(size_t start, size_t end, const std::vector<std::pair<key_type, value_type>>& v){
    if(start - end <= 0){
        return;
    }else{
        size_t half = start + ((end - start)/2);
        insert(v[half]);
        balance_aux(start, half, v);
        balance_aux(half + 1, end,  v);
    }
}

template <class key_type, class value_type, class compare_type>
value_type& Bst<key_type, value_type, compare_type>:: operator[](const key_type& x) noexcept {
    return find(x) -> second;
}

template <class key_type, class value_type, class compare_type>
value_type& Bst<key_type, value_type, compare_type>::operator[](key_type&& x){
    return find(x) -> second;
}

template <class key_type, class value_type, class compare_type>
void Bst<key_type, value_type, compare_type>::inOrderPrint(Node* n, std::ostream& os) const{
    if(n == nullptr){
        return;
    }else{
        inOrderPrint(n -> get_left(), os);
        os << *n;
        inOrderPrint(n -> get_right(), os);
    }
}

template <class key_type, class value_type, class compare_type>
std::ostream& operator<<(std::ostream& os, const Bst<key_type, value_type, compare_type>& b) {
    b.inOrderPrint(b.root, os);
    return os;
}