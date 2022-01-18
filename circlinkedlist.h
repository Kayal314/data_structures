#include <iostream>

template <typename T>
struct Node
{
    public:
    T val;
    Node* next;
    Node()
    {
        this->val=T{};
        this->next=NULL;
    }
    Node(T val)
    {
        this->val=val;
        this->next=NULL; 
    }
};

//Circularly Linked List
// works as a stack
template <typename T>
class LinkedList
{
    private:
    Node<T>* root;
    Node<T>* prev;
    int size;

    public:
    LinkedList()
    {
        this->root=NULL;
        this->prev=NULL;
        this->size=0;
    }

    ~LinkedList()
    {
        if(this->size>0)
        {
            Node<T>* node=this->root;
            for(int i=0;i<this->size;i++)
            {
                Node<T>* to_del=node;
                node=node->next;
                delete to_del;
            }
        }
    }

    int get_size()
    {
        return this->size;
    }

    bool push(T val)
    {
        if(this->size==0)
            this->root=new Node<T>(val);
        else
        {
            Node<T>* node=new Node<T>(val);
            if(this->size==1)
                node->next=this->root;
            else
                node->next=this->root->next;
            this->root->next=node; 
            this->prev=this->root;
            this->root=node;
        }
        this->size++;
        return true;
    }

    bool insert(T val, int index)
    {
        if(index<0||index>this->size)
        {
            std::cout<<"Invalid Operation"<<std::endl;
            return false;
        }
        if(index==this->size)
            return push(val);
        Node<T>* new_node=new Node<T>(val);
        Node<T>* node=this->root;
        for(int i=0;i<index;i++)
            node=node->next;
        if(this->size==1)
            new_node->next=node;
        else
            new_node->next=node->next;
        node->next=new_node;
        this->size++;
        return true;
    }

    T pop()
    {
        if(this->size==0)
        {
            std::cout<<"Invalid Operation"<<std::endl;
            return T{};
        }
        T to_del=this->root->val;
        if(this->size>1)
            this->prev->next=this->root->next;
        delete this->root;
        this->size--;
        return to_del;
    }

    T remove(int index)
    {
        if(index<0||index>=this->size)
        {
            std::cout<<"Invalid Operation"<<std::endl;
            return T{};
        }
        if(index==this->size-1)
            return pop();
        Node<T>* node=this->root->next;
        Node<T>* prev_node=this->root;
        for(int i=0;i<index;i++)
        {
            node=node->next;
            prev_node=prev_node->next;
        }
        prev_node->next=node->next;
        T to_del=node->val;
        delete node;
        this->size--;
        return to_del;
    }


    void display()
    {
        if(this->size==1)
        {
            std::cout<<this->root->val<<std::endl;
            return;
        }
        Node<T>* node=this->root;
        for(int i=0;i<this->size;i++)
        {
            node=node->next;
            std::cout<<node->val<<" ";
        }
        std::cout<<std::endl;    
    }

};