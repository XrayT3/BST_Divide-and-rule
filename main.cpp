#include <iostream>
#include <vector>
#include <memory>

using namespace std;

struct node_t
{
    int value = -1;
    shared_ptr<node_t> left = nullptr;
    shared_ptr<node_t> right = nullptr;
    node_t(){}
    node_t(int key){value = key;}
};

int count, depth = 0;

void print_tree(shared_ptr<node_t> &node, int depth){
    if(!node)
        return;
    print_tree(node->left, depth+1);
    for(int i = 0; i < depth; i++)
        cout << " ";
    cout << node->value << endl;
    print_tree(node->right, depth+1);
}

int ret_min_value(shared_ptr<node_t> &node){
    if(!node->left)
        return node->value;
    else
        return ret_min_value(node->left);
}

void get_depth(shared_ptr<node_t> &node, int curr_depth){
    if (node){
        count++;
        get_depth(node->left, curr_depth+1);
        get_depth(node->right, curr_depth+1);
    }
    depth = curr_depth > depth ? curr_depth : depth;
}

void insert(shared_ptr<node_t> &node, int key){
    if(!node){
        node = make_shared<node_t>(key);
        return;
    }
    if(key < node->value){
        if(!node->left)
            node->left = make_shared<node_t>(key);
        else
            insert(node->left, key);
    }
    else if(key > node->value){
        if(!node->right)
            node->right = make_shared<node_t>(key);
        else
            insert(node->right, key);
    }
}

shared_ptr<node_t> remove(shared_ptr<node_t> &node, int key) { 
    if (!node) 
        return node;
    if (key < node->value)
        node->left = remove(node->left, key); 
    else if (key > node->value) 
        node->right = remove(node->right, key); 
    else { 
        // node with only one child or no child
        if (!node->left) 
            return node->right; 
        else if (!node->right) 
            return node->left; 
        // node with two children
        node->value = ret_min_value(node->right); 
        // delete the inorder successor
        node->right = remove(node->right, node->value); 
    } 
    return node; 
}

void rec_insert(shared_ptr<node_t> &node, int start, int end){
    if(!node){
        insert(node, start);
        if(start+1 <= end)
            rec_insert(node->right, start+1, end);
    }
    else{
        if(end > node->value) //right
            rec_insert(node->right, max(start, node->value+1), end);
        if(start < node->value) //left
            rec_insert(node->left, start, min(end, node->value-1));
    }
}

void rec_delete(shared_ptr<node_t> &node, int start, int end){
    if(!node)
        return;
    if(start < node->value)
        rec_delete(node->left, start, min(end, node->value-1));
    if(end > node->value)
        rec_delete(node->right, max(start, node->value+1), end);
    if(start <= node->value && node->value <= end)
        node = remove(node, node->value);
}

int main()
{
    char O;
    int N, A, B;
    shared_ptr<node_t> root = nullptr;
    cin >> N;
    for(int i = 0; i < N; i++){
        cin >> O >> A >> B;
        if(O == 'i')
            rec_insert(root, A, B);
        if(O == 'd')
            rec_delete(root, A, B);
    }

    get_depth(root, -1);
    
    // print_tree(root, 0);
    cout << count << " " << depth << "\r\n";

    return 0;
}