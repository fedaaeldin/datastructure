#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

class BinaryTree
{
private:
    int data {};
    BinaryTree* left {};
    BinaryTree* right {};

public:
    BinaryTree(int data): data(data)
    {
    }

    void print_inorder() {
        if(left)
            left->print_inorder();
        cout << data << " ";
        if(right)
            right->print_inorder();
    }

    int tree_max()
    {
        int res = data;
        if (left)
        {
            res = max(res, left->tree_max());
        }
        if (right)
        {
            res = max(res, right->tree_max());
        }
        return res;
    }

    int tree_height()
    {
        int height = 0;
        if (left)
        {
            height = left->tree_height() + 1;
        }
        if (right)
        {
            height = max(right->tree_height() + 1, height);

        }
        return height;
    }

    bool is_exists(int find)
    {
        bool res = find == data;
        if (!res && left)
        {
            res = left->is_exists(find);
        }
        if (!res && right)
        {
            res = right->is_exists(find);
        }
        return res;
    }

    int count()
    {
        int count = 1;
        if (left)
        {
            count += left->count();
        }
        if (right)
        {
            count += right->count();

        }
        return count;
    }

    int count_leafs()
    {
        int count = !left && !right;
        if (left)
        {
            count += left->count_leafs();
        }

        if (right)
        {
            count += right->count_leafs();

        }
        return count;
    }

    void add(vector<int> values, vector<char> directions)
    {
        BinaryTree* current = this;
        for (int i = 0; i < values.size(); ++i )
        {
            if (directions[i] == 'L')
            {
                if (!current->left)
                {
                    current->left = new BinaryTree(values[i]);
                }
                current = current->left;
            }
            if (directions[i] == 'R')
            {
                if (!current->right)
                {
                    current->right = new BinaryTree(values[i]);
                }
                current = current->right;
            }
        }
    }

    bool is_prefect(int h = -1)
    {
        if (h == -1)
        {
            h = tree_height();
        }

        if (!left && !right)
        {
            return h == 0; //if leaf
        }

        if (!left && right || left && !right)
        {
            return false; // one child only
        }

        return left->is_prefect(h - 1) && right->is_prefect(h - 1);
    }

    bool is_prefect_using_formula()
    {
        int h = tree_height();
        int n = count();
        return pow(2, h  + 1) - 1 == n;
    }
};

int main()
{	BinaryTree tree(1);
    tree.add( { 2, 4, 7 }, { 'L', 'L', 'L' });
    cout << tree.is_exists(4) << endl;

    return 0;
}
