#include <iostream>

/**
 * U = data type of the actual object or element to be stored in the heap
 * V = data type of the key of the object to be stored in the heap
 */
template <typename U, typename V>
class MaxHeap
{
    private:
    U* element_arr;
    V* key_arr;
    int capacity;
    int last_index;

    public:
    MaxHeap(int max_elems)
    {
        element_arr=new U[max_elems];
        key_arr=new V[max_elems];
        capacity=max_elems;
        last_index=-1;
    }

    bool insert(U element, V key)
    {
        if(last_index==capacity-1)
            return false;
        element_arr[++last_index]=element;
        key_arr[last_index]=key;
        move_up(last_index);
        return true;
    }

    U extract_max()
    {
        if(last_index==-1)
        {
            std::cout<<"Heap is empty"<<std::endl;
            return U{};
        }
        U max=element_arr[0];
        element_arr[0]=element_arr[last_index];
        key_arr[0]=key_arr[last_index];
        last_index--;
        move_down(0);
        return max;
    }

    U peek_max()
    {
        if(last_index==-1)
        {
            std::cout<<"Heap is empty"<<std::endl;
            return U{};
        }
        return element_arr[0];
    }

    int get_size()
    {
        return last_index+1;
    }


    private:
    void move_up(int index)
    {
        int parent_index=(index-1)/2;
        if(parent_index<0)
            return;
        if(key_arr[parent_index]<key_arr[index])
        {
            swap(parent_index,index);
            move_up(parent_index);
        }
    }

    void move_down(int index)
    {
        int child_index;
        if(2*index+1>last_index)
            return;
        else if(2*index+1==last_index)
            child_index=2*index+1;
        else
            child_index=key_arr[2*index+1]>key_arr[2*index+2]?2*index+1:2*index+2;
        if(key_arr[child_index]>key_arr[index])
        {
            swap(child_index, index);
            move_down(child_index);
        }
    }

    void swap(int n, int m)
    {
        //swap keys
        V temp_key=key_arr[n];
        key_arr[n]=key_arr[m];
        key_arr[m]=temp_key;

        //swap actual elements
        U temp_obj=element_arr[n];
        element_arr[n]=element_arr[m];
        element_arr[m]=temp_obj;
    }
};