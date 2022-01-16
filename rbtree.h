#include <iostream>
#include <queue>

enum Color{red, black};

template <typename T>
struct Node
{
    public:
    T val;
    Node<T>* left;
    Node<T>* right;
    Node<T>* parent;
    Color color;
    Node()
    {
        this->val=T{};
        this->left=NULL;
        this->right=NULL;
        this->parent=NULL;
        this->color=black;
    }
    Node(T val, Node<T>* sentinel, Color color)
    {
        this->val=val;
        this->left=sentinel;
        this->right=sentinel;
        this->parent=sentinel;
        this->color=color;
    }
};

template <typename T>
class RBTree
{
    private:
    Node<T>* root;
    Node<T>* sentinel;

    public:
    RBTree()
    {
        this->sentinel=new Node<T>();
        this->root=sentinel;
    }

    bool present(T val)
    {
        if(search(this->root,val)==this->sentinel)
            return false;
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
        if(this->root==this->sentinel)
            return;
        std::queue<Node<T>*> q;
        q.push(this->root);
        while(!q.empty())
        {
            Node<T>* node=q.front();
            std::cout<<node->val<<" ";
            q.pop();

            if(node->left!=this->sentinel)
                q.push(node->left);
            if(node->right!=this->sentinel)
                q.push(node->right);
        }  
        std::cout<<std::endl;
    }

    T successor(T val)
    {
        Node<T>* x=search(this->root,val);
        if(x==this->sentinel)
        {
            std::cout<<"The value "<<val<<" does not exist in this red-black tree"<<std::endl;
            return T{};
        }
        if(x->right!=this->sentinel)
            return minimum(x->right)->val;
        Node<T>* y=x->parent;
        while(y!=this->sentinel&&x==y->right)
        {
            x=y;
            y=y->parent;
        }
        return y->val;

    }

    T predecessor(T val)
    {
        Node<T>* x=search(this->root,val);
        if(x==this->sentinel)
        {
            std::cout<<"The value "<<val<<" does not exist in this red-black tree"<<std::endl;
            return T{};
        }
        if(x->left!=this->sentinel)
            return maximum(x->left)->val;
        Node<T>* y=x->parent;
        while(y!=this->sentinel&&x==y->left)
        {
            x=y;
            y=y->parent;
        }
        return y->val;
    }

    void insert_val(T val)
    {
        Node<T>* z=new Node<T>(val, this->sentinel, red);
        Node<T>* y=this->sentinel;
        Node<T>* x=this->root;
        while(x!=this->sentinel)
        {
            y=x;
            if(z->val<x->val)
                x=x->left;
            else
                x=x->right;
        }
        z->parent=y;
        if(y==this->sentinel)
            this->root=z;
        else if(z->val<y->val)
            y->left=z;
        else
            y->right=z;
        insert_fixup(z);
    }

    void delete_val(T val)
    {
        Node<T>* z=search(this->root,val);
        Node<T>* y=z;
        Color y_original_color=y->color;
        Node<T>* x;
        if(z->left==this->sentinel)
        {
            x=z->right;
            transplant(z,z->right);
        }
        else if(z->right==this->sentinel)
        {
            x=z->left;
            transplant(z,z->left);
        }
        else
        {
            y=minimum(z->right);
            y_original_color=y->color;
            x=y->right;
            if(y->parent==z)
                x->parent=y;
            else
            {
                transplant(y,y->right);
                y->right=z->right;
                z->right->parent=y;
            }
            transplant(z,y);
            y->left=z->left;
            y->left->parent=y;
            y->color=z->color;
        }
        if(y_original_color==black)
            delete_fixup(x);
        delete z;
    }

    
    private:
    void inorder(Node<T>* x)
    {
        if(x!=this->sentinel)
        {
            inorder(x->left);
            std::cout<<x->val<<" ";
            inorder(x->right);
        }
    }

    void preorder(Node<T>* x)
    {
        if(x!=this->sentinel)
        {
            std::cout<<x->val<<" ";
            preorder(x->left);
            preorder(x->right);
        }
    }

    void postorder(Node<T>* x)
    {
        if(x!=this->sentinel)
        {
            postorder(x->left);
            postorder(x->right);
            std::cout<<x->val<<" ";
        }
    }

    // Assume x->right != this->sentinel
    void left_rotate(Node<T>* x)
    {
        Node<T>* y=x->right;
        x->right=y->left;
        if(y->left!=NULL)
            y->left->parent=x;
        y->parent=x->parent;
        if(x->parent=this->sentinel)
            this->root=y;
        else if(x==x->parent->left)
            x->parent->left=y;
        else
            x->parent->right=y;
        y->left=x;
        x->parent=y;
    }
    // Assume y->left != this->sentinel
    void right_rotate(Node<T>* y)
    {
        Node<T>* x=y->left;
        y->left=x->right;
        if(x->right!=NULL)
            x->right->parent=y;
        x->parent=y->parent;
        if(y->parent==this->sentinel)
            this->root=x;
        else if(y==y->parent->right)
            y->parent->right=x;
        else
            y->parent->left=x;
        x->right=y;
        y->parent=x;
    }

    void insert_fixup(Node<T>* z)
    {
        while(z->parent->color==red)
        {
            if (z->parent==z->parent->parent->left)
            {
                Node<T>* y=z->parent->parent->right;
                if(y->color==red)
                {
                    z->parent->color=black;
                    y->color=black;
                    z->parent->parent->color=red;
                }
                else 
                {
                    if(z==z->parent->right)
                    {
                        z=z->parent;
                        left_rotate(z);
                    }
                    z->parent->color=black;
                    z->parent->parent->color=red;
                    right_rotate(z->parent->parent);
                }

            }
            else
            {
                Node<T>* y=z->parent->parent->left;
                if(y->color==red)
                {
                    z->parent->color=black;
                    y->color=black;
                    z->parent->parent->color=red;
                }
                else
                {
                    if(z==z->parent->left)
                    {
                        z=z->parent;
                        right_rotate(z);
                    }
                    z->parent->color=black;
                    z->parent->parent->color=red;
                    left_rotate(z->parent->parent);
                }
            }
            
        }
        this->root->color=black;
    }

    Node<T>* minimum(Node<T>* x)
    {
        while(x->left!=this->sentinel)
            x=x->left;
        return x;
    }

    Node<T>* maximum(Node<T>* x)
    {
        while(x->right!=this->sentinel)
            x=x->right;
        return x;
    }

    Node<T>* search(Node<T>* x, T val)
    {
        if(x==this->sentinel||val==x->val)
            return x;
        if(val<x->val)
            return search(x->left,val);
        return search(x->right, val);
    }

    void transplant(Node<T>* u, Node<T>* v)
    {
        if(u->parent==this->sentinel)
            this->root=v;
        else if(u==u->parent->left)
            u->parent->left=v;  
        else
            u->parent->right=v;
        v->parent=u->parent;
    }

    void delete_fixup(Node<T>* x)
    {
        while(x!=this->sentinel&&x->color==black)
        {
            if(x==x->parent->left)
            {
                Node<T>* w=x->parent->right;
                if(w->color==red)
                {
                    w->color=black;
                    x->parent->color=red;
                    left_rotate(x->parent);
                    w=x->parent->right;
                }
                if(w->left->color==black&&w->right->color==black)
                {
                    w->color=red;
                    x=x->parent;
                }
                else
                {
                    if(w->right->color==black)
                    {
                        w->left->color=black;
                        w->color=red;
                        right_rotate(w);
                        w=x->parent->right;
                    }
                    w->color=x->parent->color;
                    x->parent->color=black;
                    w->right->color=black;
                    left_rotate(x->parent);
                    x=this->root;
                }
            }
            else
            {
                Node<T>* w=x->parent->left;
                if(w->color==red)
                {
                    w->color=black;
                    x->parent->color=red;
                    right_rotate(x->parent);
                    w=x->parent->left;
                }
                if(w->right->color==black&&w->left->color==black)
                {
                    w->color=red;
                    x=x->parent;
                }
                else
                {
                    if(w->left->color==black)
                    {
                        w->right->color=black;
                        w->color=red;
                        left_rotate(w);
                        w=x->parent->left;
                    }
                    w->color=x->parent->color;
                    x->parent->color=black;
                    w->left->color=black;
                    right_rotate(x->parent);
                    x=this->root;
                }
            }
            x->color=black;
        }
    }
};
