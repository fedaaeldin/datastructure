#include <iostream>
#include <cassert>
#include <vector>
using namespace std;

class Stack
{
private:
    int size{};
    int top{};
    int* array{};

public:
    Stack(int size) :
        size(size), top(-1)
    {
        array = new int[size];
    }

    ~Stack()
    {
        delete[] array;
    }

    void push(int x)
    {
        assert(!isFull());
        array[++top] = x;
    }

    int pop()
    {
        assert(!isEmpty());
        return array[top--];
    }

    int peek()
    {
        assert(!isEmpty());
        return array[top];
    }

    int isFull()
    {
        return top == size - 1;
    }

    int isEmpty()
    {
        return top == -1;
    }

    void display()
    {
        for (int i = top; i >= 0; i--)
            cout << array[i] << " ";
        cout << "\n";
    }

    void reverse_display()
    {
        for (int i = 0; i <= top; i++)
            cout << array[i] << " ";
        cout << "\n";
    }

    void insert_bottom(int value)
    {
        if (isEmpty())
        {
            push(value);
        } else
        {
            int cur = pop();
            insert_bottom(value);
            push(cur);
        }
    }

    void reverse()
    {
        if (isEmpty())
        {
            return;
        }
        int cur = pop();

        reverse();

        insert_bottom(cur);
    }
};

string reverse_sub_words(string line)
{
    int len = line.length();
    string output = {};
    Stack stk(len);
    for (int i = 0; i < len; i++)
    {
        if (line[i] == ' ')
        {
            while (!stk.isEmpty())
            {
                output += stk.pop();
            }
            output += ' ';
        }
        else
        {
            stk.push(line[i]);
        }
    }
    while (!stk.isEmpty())
    {
        output += stk.pop();
    }
    return output;
}

int reverse_number(int num)
{
    if (num == 0) return 0;
    Stack stk(25);
    while (num)
    {
        stk.push(num % 10), num /= 10;
    }
    int tens = 1;
    while (! stk.isEmpty())
    {
        cout << num << endl;
        num = stk.pop() * tens + num, tens *= 10;
    }
    return num;
}

char match_parentheses(char p)
{
    if (p == ')') return '(';
    if (p == ']') return '[';
    return '{';
}

bool valid_parentheses(string input)
{
    Stack stk(input.size());
    for (char ch : input)
    {
        if (ch == '(' || ch == '{' || ch == '[')
        {
            stk.push(ch);
        } else if (stk.isEmpty() || stk.pop() != match_parentheses(ch))
        {
            return false;
        }
    }
    return stk.isEmpty();
}

string remove_adjacent(string input)
{
    Stack stk(input.size());
    for (char ch: input)
    {
        if (! stk.isEmpty() && stk.peek() == ch)
        {
            stk.pop();
        } else
        {
            stk.push(ch);
        }
    }
    string output = "";
    while (! stk.isEmpty())
    {
        output = ( (char) stk.pop()) + output;
    }
    return output;
}

bool haveDifferentSigns(int a, int b) {
    return (a ^ b) < 0; // XOR the two numbers and check if the result is negative
}

void asteroid_collision(vector<int> nums)
{
    int size = nums.size();
    if (size <= 1)
    {
        return;
    }

    Stack stk(size);
    int i = 0;
    while (i < size)
    {
        if (! stk.isEmpty() && haveDifferentSigns(stk.peek(), nums[i]))
        {
            if (abs(stk.peek()) > abs(nums[i]))
            {
                ++i;
            } else if (abs(stk.peek()) < nums[i])
            {
                stk.pop();
            } else
            {
                ++i;
                stk.pop();
            }
        } else
        {
            stk.push(nums[i++]);
        }
    }

    stk.reverse_display();
}





int main()
{
    Stack stk(10);
    stk.push(1);
    stk.push(2);
    stk.push(3);
    // stk.reverse();

    stk.display();

    return 0;
}
