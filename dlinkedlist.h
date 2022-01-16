#include <iostream>

template <typename T>
struct Node
{
    public:
    T val;
    Node<T>* next;
    Node<T>* prev;

    Node()
    {
        this->val=T{};
        this->next=NULL;
        this->prev=NULL;
    }
    Node(T val)
    {
        this->val=val;
        this->next=NULL;
        this->prev=NULL;
    }
};

//doubly linked list
template <typename T>
class LinkedList
{
    private:
    Node<T>* head;
    Node<T>* foot;
    int size;

    public:
    LinkedList()
    {
        this->head=NULL;
        this->foot=NULL;
        this->size=0;
    }

    int get_size()
    {
        return this->size;
    }

    bool present(T val)
    {
        Node<T>* node=this->foot;
        while(node!=NULL)
        {
            if(node->val==val)
                return true;
            node=node->prev;
        }
        return false;
    }

    bool insert_at_end(T val)
    {
        if(this->size==0)
        {
            this->head=new Node<T>(val);
            this->foot=this->head;
        }
        else
        {
            Node<T>* node=new Node<T>(val);
            node->next=this->head;
            this->head->prev=node;
            this->head=node;
        }
        this->size++;
        return true;
    }

    bool insert_at_beginning(T val)
    {
        if(this->size==0)
        {
            this->foot=new Node<T>(val);
            this->head=this->foot;
        }
        else
        {
            Node<T>* node=new Node<T>(val);
            node->prev=this->foot;
            this->foot->next=node;
            this->foot=node;
        }
        this->size++;
        return true;
    }

    bool insert(T val, int index)
    {
        if(index>this->size||index<0)
            return false;
        if(index==0)
            return insert_at_beginning(val);
        else if(index==this->size)
            return insert_at_end(val);
        else
        {
            Node<T>* new_node=new Node<T>(val);
            Node<T>* node=this->foot;
            for(int i=0;i<index;i++)
                node=node->prev;
            Node<T>* next_node=node->next;
            node->next=new_node;
            new_node->prev=node;
            new_node->next=next_node;
            next_node->prev=new_node;
        }
        this->size++; 
        return true;  
    }

    T delete_at(int index)
    {
        if(index==this->size-1)
            return delete_last();
        if(index==0)
            return delete_first();
        if(index<0||index>=this->size)
        {
            std::cout<<"Invalid Index"<<std::endl;
            return T{};
        }
        Node<T>* node=this->foot;
        for(int i=0;i<index;i++)
            node=node->prev;
        node->prev->next=node->next;
        node->next->prev=node->prev;
        T to_delete=node->val;
        delete node;
        this->size--;
        return to_delete;
    }

    T delete_first()
    {
        T to_delete=this->foot->val;
        this->foot=this->foot->prev;
        delete this->foot->next;
        this->foot->next=NULL;
        this->size--;
        return to_delete;
    }

    T delete_last()
    {
        T to_delete=this->head->val;
        this->head=this->head->next;
        delete this->head->prev;
        this->head->prev=NULL;
        this->size--;
        return to_delete;
    }

    void display()
    {
        Node<T>* node=this->foot;
        while(node!=NULL)
        {
            std::cout<<node->val<<" ";
            node=node->prev;
        }
        std::cout<<std::endl;
    }
};