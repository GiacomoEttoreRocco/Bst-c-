#pragma once

template <class key_type, class value_type, class compare_type>
class Bst<key_type, value_type, compare_type>::Node {
    
    Node* left = nullptr;
    Node* right = nullptr;
    std::pair<key_type, value_type> data;

    public:

    Node* succ = nullptr;
    Node* pred = nullptr;
    Node* parent = nullptr;

    Node(const std::pair<key_type, value_type>& c){
        data = c;
        //std::puts("Base pair constructor node");
    }
    
    Node(const Node& n) = delete;

    Node& operator=(const Node& n) = delete;

    ~Node() noexcept {
        //std::puts("Destructor of Node");
    }

    Node(Node&& node) noexcept 
    {
        //std::puts("Move constructor node");
        data = std::move(node.data);
        left = std::move(node.left);
        right = std::move(node.right);
        parent = std::move(node.parent);
        succ = std::move(node.succ); 
        pred = std::move(node.pred); 

        node.data = nullptr;  //la move non mette a nullptr si rischiano double free
        node.left = nullptr;
        node.right = nullptr;
        node.parent = nullptr;
        node.succ = nullptr;
        node.pred = nullptr;

    }

    Node& operator=(Node &&node) noexcept
    {
        //std::puts("Move operator node");
        data = std::move(node.data);
        left = std::move(node.left);
        right = std::move(node.right);
        parent = std::move(node.parent);
        succ = std::move(node.succ); 
        pred = std::move(node.pred); 

        node.data = nullptr;
        node.left = nullptr;
        node.right = nullptr;
        node.parent = nullptr;
        node.succ = nullptr;
        node.pred = nullptr;

        return *this;
    }
    
    bool is_right() const noexcept // noexcept: non lancia eccezioni
    {
        return (parent && (parent -> right == this));
    }

    bool is_left() const noexcept
    {
        return (parent && (parent -> left == this));
    }

    bool is_root() const noexcept
    {
        return !is_left() && !is_right();
    }

    bool is_leaf(){
        return left == nullptr && right == nullptr;
    }

    Node* get_left() noexcept
    {
        return left;
    }

    Node* get_right() noexcept
    {
        return right;
    }

    Node* get_parent() const noexcept
    {
        return parent;
    }
    std::pair<key_type, value_type>& get_data(){
        return data;
    }
    key_type& get_key(){
        return data.first;
    }
    value_type& get_value(){
        return data.second;
    }
    void set_left(Node* n){
        left = n;
        if(n != nullptr){
            left -> parent = this;
        }
    }
    void set_right(Node* n){
        right = n;
        if(n != nullptr){
            right -> parent = this;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, Node& n){
        os << "key: " << n.data.first << " " << "value: " << n.data.second << " " << std::endl;
        return os;
    }
};
