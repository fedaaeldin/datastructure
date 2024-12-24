#include <assert.h>
#include <iostream>

using namespace std;

class Vector
{
private:
    int *arr = nullptr;
    int size = 0;
    int capacity = {};
public:
    Vector(int size) : size(size)
    {
        if (size < 0) size = 0;
        capacity = size * 2;
        arr = new int[capacity] {}; // initially all created with zeros
    }
    ~Vector()
    {
        delete arr;
        arr = nullptr;
    }

    int get(int idx)
    {
        assert(idx >= 0 && idx < size);
        return arr[idx];
    }

    int set (int idx, int val)
    {
        assert(idx >= 0 && idx < size);
        arr[idx] = val;
    }

    void print()
    {
        for (int i = 0; i < size; i++)
        {
            cout << arr[i] << " ";
        }

        cout << endl;
    }

    int find(int val)
    {
        for (int i = 0; i < size; i++)
        {
            if (val == arr[i]) return i;
        }
        return -1;
    }

    int get_front()
    {
        return arr[0];
    }

    int get_back()
    {
        return arr[size - 1];
    }

    void push_back(int val)
    {
        if (size == capacity)
        {
            expand_capacity();
        }
        arr[size++] = val;
    }

    void insert(int idx, int val)
    {
        assert(idx >= 0 && idx < size);
        if (size == capacity)
        {
           expand_capacity();
        }

        for (int i = size - 1; i >= idx; i--)
        {
            arr[i + 1] = arr[i];
        }

        arr[idx] = val;
        ++size;
    }

    void rotate_right()
    {
        int last = arr[size - 1];
        for (int i = size -1; i > 0; --i)
        {
            arr[i] = arr[i - 1];
        }
        arr[0] = last;
    }

    void rotate_left()
    {
        int first = arr[0];
        for (int i = 1; i < size; i++)
        {
            arr[i - 1] = arr[i];
        }
        arr[size - 1] = first;
    }

    void rotate_left(int times)
    {
        while (times--)
        {
            rotate_left();
        }
    }

    void rotate_right(int times)
    {
        times %= size;
        while(times--)
        {
            rotate_right();
        }
    }

    int pop(int idx)
    {
        assert(idx >= 0 && idx < size);
        int val = arr[idx];
        for (int i = idx + 1; i < size - 1; i++)
        {
            arr[i - 1] = arr[i];
        }
        --size;

        return val;
    }

    int find_transposition(int val)
    {
        for (int i = 0; i < size; i++)
        {
            if (val == arr[i])
            {
                if (i == 0) return 0;

                // swap
                int tmp = arr[i - 1];
                arr[i - 1] = arr[i];
                arr[i] = tmp;

                return i - 1;
            }
        }
        return -1;
    }

    void expand_capacity()
    {
        // New array to hold the new element
        capacity *= 2;
        cout << "Expanding the capacity to: " << capacity << endl;
        int *newArr = new int[capacity];

        // Copy the data
        for (int i = 0; i < size; i++)
        {
            newArr[i] = arr[i];
        }

        // change the pointers
        swap(arr, newArr);

        // Delete local pointer
        delete[] newArr;
    }
};
int main()
{
    Vector v(3);
    for (int i = 0; i < 3; i++)
    {
        v.set(i, i + 1);
    }
    v.push_back(50);
    v.push_back(30);
    v.push_back(30);
    v.push_back(30);
    v.insert(3, 4);
    v.rotate_right(4);
    v.rotate_left();
    cout << v.pop(2) << endl;
    cout << v.pop(5) << endl;
    v.print();



    return 0;
}
