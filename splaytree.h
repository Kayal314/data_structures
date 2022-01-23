#include <iostream>
#include <queue>

template <typename T>
struct Node
{
    public:
    T val;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Node()
    {
        this->val=T{};
        this->left=NULL;
        this->right=NULL;
        this->parent=NULL;
    }
    Node(T val)
    {
        this->val=val;
        this->left=NULL;
        this->right=NULL;
        this->parent=NULL;
    }
};

template <typename T>
class SplayTree
{
    private:
    Node<T>* root;
    unsigned long size;

    public:
    SplayTree()
    {
        this->root=NULL;
        this->size=0;
    }

    SplayTree(Node<T>* root)
    {
        this->root=root;
        this->size=0;
    }

    ~SplayTree()
    {
        if(this->root)
            release_mem(root);
    }

    bool is_empty()
    {
        return root==NULL;
    }

    unsigned long get_size()
    {
        return size;
    }

    bool present(T val)
    {
        return search(this->root,val)!=NULL;
    }

    bool insert(T val)
    {
        Node<T>* z=new Node<T>(val);
        Node<T>* y=NULL;
        Node<T>* x=this->root;
        while(x!=NULL)
        {
            y=x;
            if(z->val<x->val)
                x=x->left;
            else
                x=x->right;
        }
        z->parent=y;
        if(y==NULL)
            this->root=z;
        else if(z->val<y->val)
            y->left=z;
        else
            y->right=z;
        splay(z);
        size++;
        return true;
    }

    // elements of tree s are smaller than those of tree t
    SplayTree<T>* join(SplayTree<T>* s, SplayTree<T>* t)
    {
        Node<T>* max_elem = s->root;
        while(max_elem->right)
            max_elem=max_elem->right;
        s->splay(max_elem);
        max_elem->right=t->root;
        return new SplayTree<T>(max_elem);
    }

    bool remove(T val)
    {
        Node<T>* z = search(this->root,val);
        if(z==NULL)
            return false;
        splay(z);
        if (z->left==NULL) 
            transplant(z, z->right);
        else if (z->right==NULL) 
            transplant(z, z->left);
        else 
        {
            Node<T>* y = minimum(z->right);
            if (y->parent != z) 
            {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        }
        delete z;
        size--;
        return true;
    }


     void inorder_traversal()
    {
        std::cout<<"Inorder Traversal"<<std::endl;
        inorder(this->root);
        std::cout<<std::endl;
    }

    void preorder_traversal()
    {
        std::cout<<"Preorder Traversal"<<std::endl;
        preorder(this->root);
        std::cout<<std::endl;
    }

    void postorder_traversal()
    {
        std::cout<<"Postorder Traversal"<<std::endl;
        postorder(this->root);
        std::cout<<std::endl;
    }

    void levelorder_traversal()
    {
        std::cout<<"Levelorder Traversal"<<std::endl;
        if(this->root==NULL)
            return;
        std::queue<Node<T>*> q;
        q.push(this->root);
        while(!q.empty())
        {
            Node<T>* node=q.front();
            std::cout<<node->val<<" ";
            q.pop();

            if(node->left!=NULL)
                q.push(node->left);
            if(node->right!=NULL)
                q.push(node->right);
        }  
        std::cout<<std::endl;
    }

    T successor(T val)
    {
        Node<T>* x=search(this->root,val);
        if(x==NULL)
        {
            std::cout<<"The value "<<val<<" does not exist in this splay tree"<<std::endl;
            return T{};
        }
        if(x->right!=NULL)
            return minimum(x->right)->val;
        Node<T>* y=x->parent;
        while(y!=NULL&&x==y->right)
        {
            x=y;
            y=y->parent;
        }
        if(y==NULL)
        {
            std::cout<<"No successor of "<<val<<" exists in this splay tree"<<std::endl;
            return T{};
        }
        return y->val;
    }

    T predecessor(T val)
    {
        Node<T>* x=search(this->root,val);
        if(x==NULL)
        {
            std::cout<<"The value "<<val<<" does not exist in this splay tree"<<std::endl;
            return T{};
        }
        if(x->left!=NULL)
            return maximum(x->left)->val;
        Node<T>* y=x->parent;
        while(y!=NULL&&x==y->left)
        {
            x=y;
            y=y->parent;
        }
        if(y==NULL)
        {
            std::cout<<"No predecessor of "<<val<<" exists in this splay tree"<<std::endl;
            return T{};
        }
        return y->val;
    }

    private:
    void release_mem(Node<T>* node)
    {
        if(node->left!=NULL)
            release_mem(node->left);
        if(node->right!=NULL)
            release_mem(node->right);
        delete node;
    }

    void left_rotate(Node<T>* x)
    {
        Node<T>* y=x->right;
        if(y!=NULL)
        {
            x->right=y->left;
            if(y->left!=NULL)
                y->left->parent=x;
            y->parent=x->parent;
        }
        if(x->parent==NULL)
            this->root=y;
        else if(x==x->parent->left)
            x->parent->left=y;
        else
            x->parent->right=y;
        if(y!=NULL)
            y->left=x;
        x->parent=y;
    }

    void right_rotate(Node<T>* y)
    {
        Node<T>* x=y->left;
        if(x!=NULL)
        {
            y->left=x->right;
            if(x->right!=NULL)
                x->right->parent=y;
            x->parent=y->parent;
        }
        if(y->parent==NULL)
            this->root=x;
        else if(y==y->parent->right)
            y->parent->right=x;
        else
            y->parent->left=x;
        if(x!=NULL)
            x->right=y;
        y->parent=x;
    }

    // if x->parent == this->root
    void zig(Node<T>* x)
    {
        if(x==x->parent->left)
            right_rotate(x->parent);
        else
            left_rotate(x->parent);
        this->root=x;      
    }

    // if x->parent != this->root and x and its parent are both left children
    void zig_zig_left(Node<T>* x)
    {
        right_rotate(x->parent->parent);
        right_rotate(x->parent);
    }

    // if x->parent != this->root and x and its parent are both right children
    void zig_zig_right(Node<T>* x)
    {
        left_rotate(x->parent->parent);
        left_rotate(x->parent);
    }

    // if x->parent != this->root and x is the right child and x->parent is the left child
    void zig_zag_rl(Node<T>* x)
    {
        left_rotate(x->parent);
        right_rotate(x->parent);
    }

    // if x->parent != this->root and x is the left child and x->parent is the right child
    void zig_zag_lr(Node<T>* x)
    {
        right_rotate(x->parent);
        left_rotate(x->parent);
    }

    void splay(Node<T>* x)
    {
        while(x!=this->root)
        {
            if(x->parent==this->root)
                zig(x);
            else
            {
                Node<T>* p=x->parent;
                if(x==x->parent->left&&p==p->parent->left)
                    zig_zig_left(x);
                else if(x==x->parent->left&&p==p->parent->right)
                    zig_zag_lr(x);
                else if(x==x->parent->right&&p==p->parent->left)
                    zig_zag_rl(x);
                else
                    zig_zig_right(x);
            }
        }
        this->root=x;
    }

    void transplant(Node<T>* x, Node<T>* y)
    {
        if (x->parent==NULL) 
            this->root = y;
        else if (x == x->parent->left) 
            x->parent->left = y;
        else 
            x->parent->right = y;
        if (y!=NULL) 
            y->parent = x->parent;
    }

     Node<T>* search(Node<T>* x, T val)
    {
        if(x==NULL||val==x->val)
            return x;
        if(val<x->val)
            return search(x->left,val);
        return search(x->right, val);
    }

    void inorder(Node<T>* x)
    {
        if(x!=NULL)
        {
            inorder(x->left);
            std::cout<<x->val<<" ";
            inorder(x->right);
        }
    }

    void preorder(Node<T>* x)
    {
        if(x!=NULL)
        {
            std::cout<<x->val<<" ";
            preorder(x->left);
            preorder(x->right);
        }
    }

    void postorder(Node<T>* x)
    {
        if(x!=NULL)
        {
            postorder(x->left);
            postorder(x->right);
            std::cout<<x->val<<" ";
        }
    }

     Node<T>* minimum(Node<T>* x)
    {
        while(x->left!=NULL)
            x=x->left;
        return x;
    }

    Node<T>* maximum(Node<T>* x)
    {
        while(x->right!=NULL)
            x=x->right;
        return x;
    }
};