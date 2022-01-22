#include <iostream>

template <typename T>
class BITTree
{
    private:
    T* tree;
    int size;
    
    public:
    BITTree(T array[], int size)
    {
        this->size=size+1;
        this->tree=new T[this->size];
        for(int i=0;i<this->size;i++)
            tree[i]=0;
        for(int i=0;i<this->size-1;i++)
            update(i,array[i]);
    }

    ~BITTree()
    {
        delete[] tree;
    }
    
    private:
    int getParent(int index)
    {
        return index-(index&(-index));
    }

    int getNext(int index)
    {
        return index+(index&(-index));
    }

    public:
    T prefix_sum(int index)
    {
        if(index<0||index>=this->size-1)
        {
            std::cout<<"Index out of range"<<std::endl;
            return T{};
        }
        index++;
        T sum=T{};
        while(index>0)
        {
            sum+=tree[index];
            index=getParent(index);
        }
        return sum;
    }

    bool update(int index, T val)
    {
        if(index<0||index>=this->size-1)
        {
            std::cout<<"Index out of range"<<std::endl;
            return false;
        }
        index++;
        while(index<this->size)
        {
            tree[index]+=val;
            index=getNext(index);
        }
        return true;
    }

};