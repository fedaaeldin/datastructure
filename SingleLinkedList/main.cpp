#include <iostream>
#include <cassert>
#include <climits>

#include <vector>		// for debug
#include <algorithm>
#include <sstream>
using namespace std;

struct Node
{
	int data{};
	Node* next{};

	Node(int data) : data(data)
	{
	}

	~Node()
	{
		cout << "Destroy value: " << data << " at address " << this << "\n";
	}
};

class LinkedList
{
private:
	Node* head{};
	Node* tail{};
	int length = 0; // let's maintain how many nodes

	vector<Node*> debug_data; // add/remove nodes you use

	void debug_add_node(Node* node)
	{
		debug_data.push_back(node);
	}

	void debug_remove_node(Node* node)
	{
		auto it = std::find(debug_data.begin(), debug_data.end(), node);
		if (it == debug_data.end())
			cout << "Node does not exist\n";
		else
			debug_data.erase(it);
	}

public:
	// Below 2 deletes prevent copy and assign to avoid this mistake
	LinkedList()
	{
	}

	LinkedList(const LinkedList&) = delete;
	LinkedList& operator=(const LinkedList& another) = delete;

	~LinkedList()
	{
		while (head)
		{
			Node* current = head->next;
			delete head;
			head = current;
		}
	}

	void debug_print_address()
	{
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur << "," << cur->data << "\t";
		cout << "\n";
	}

	void debug_print_node(Node* node, bool is_seperate = false)
	{
		if (is_seperate)
			cout << "Sep: ";
		if (node == nullptr)
		{
			cout << "nullptr\n";
			return;
		}
		cout << node->data << " ";
		if (node->next == nullptr)
			cout << "X ";
		else
			cout << node->next->data << " ";

		if (node == head)
			cout << "head\n";
		else if (node == tail)
			cout << "tail\n";
		else
			cout << "\n";
	}

	void debug_print_list(string msg = "")
	{
		if (msg != "")
			cout << msg << "\n";
		for (int i = 0; i < (int)debug_data.size(); ++i)
			debug_print_node(debug_data[i]);
		cout << "************\n" << flush;
	}

	string debug_to_string()
	{
		if (length == 0)
			return "";
		ostringstream oss;
		for (Node* cur = head; cur; cur = cur->next)
		{
			oss << cur->data;
			if (cur->next)
				oss << " ";
		}
		return oss.str();
	}

	void debug_verify_data_integrity()
	{
		if (length == 0)
		{
			assert(head == nullptr);
			assert(tail == nullptr);
		}
		else
		{
			assert(head != nullptr);
			assert(tail != nullptr);
			if (length == 1)
				assert(head == tail);
			else
				assert(head != tail);
			assert(!tail->next);
		}
		int len = 0;
		for (Node* cur = head; cur; cur = cur->next, len++)
			assert(len < 10000); // Consider infinite cycle?
		assert(length == len);
		assert(length == (int)debug_data.size());
	}

	////////////////////////////////////////////////////////////

	void print()
	{
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur->data << " ";
		cout << "\n";
	}

	// These 2 simple functions just to not forget changing the vector and length
	void delete_node(Node* node)
	{
		debug_remove_node(node);
		--length;
		delete node;
	}

	void add_node(Node* node)
	{
		debug_add_node(node);
		++length;
	}

	void insert_end(int value)
	{
		Node* item = new Node(value);
		add_node(item);

		if (!head)
			head = tail = item;
		else
			tail->next = item, tail = item;
	}

	void insert_front(int value)
	{
		Node* item = new Node(value);

		if (!head) head = tail = item;
		else
		{
			item->next = head;
			head = item;
		}
		++length;
	}

	void delete_front()
	{
		assert(length);
		Node* cur = head->next;
		delete_node(head);
		head = cur;
		if (!head) tail = nullptr;
	}

	void delete_last()
	{
		if (length <= 1)
		{
			delete_front();
			return;
		}
		Node* previous = get_nth(length - 1);
		delete_node(tail);
		tail = previous;
		tail->next = nullptr;
	}

	void delete_nth(int n)
	{
		if (n < 1 || n > length)
		{
			cout << "no such nodes exists" << endl;
		}
		else if (n == 1)
		{
			delete_front();
		}
		else
		{
			Node* before_nth = get_nth(n - 1);
			Node* nth = before_nth->next;
			bool is_tail = nth == tail;

			// Connect before with after
			before_nth->next = nth->next;
			if (is_tail)
			{
				tail = before_nth;
			}
			delete_node(nth);
		}
	}

	Node* get_nth(int n)
	{
		assert(n > 0);
		int cnt = 0;
		for (Node* cur = head; cur; cur = cur->next)
		{
			if (++cnt == n) return cur;
		}
		return nullptr;
	}

	Node* get_nth_back(int n)
	{
		if (length < n) return nullptr;
		return get_nth(length - n + 1);
	}

	void delete_next_node(Node* node)
	{
		assert(node);
		Node* to_delete = node->next;
		bool is_tail = to_delete == tail;
		node->next = node->next->next;
		delete_node(to_delete);
		if (is_tail) tail = node;
	}

	void delete_with_key(int k)
	{
		if (!length)
		{
			cout << "Empty list" << endl;
		}
		else if (head->data == k)
		{
			delete_front();
		}
		else
		{
			for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
			{
				if (cur->data == k)
				{
					delete_next_node(prv);
					return;
				}
			}
			cout << "Value not found" << endl;
		}
	}

	void swap_consecutive()
	{
		if (length < 2) return;
		for (Node* cur = head; cur; cur = cur->next)
		{
			if (cur->next)
			{
				swap(cur->data, cur->next->data);
				cur = cur->next;
			}
		}
	}

	void reverse()
	{
		if (length <= 1) return;
		Node* prv = head;
		tail = head;
		head = head->next;
		while (head)
		{
			Node* next = head->next;
			head->next = prv;

			prv = head;
			head = next;
		}
		head = prv;
		tail->next = nullptr;
	}

	void delete_even()
	{
		if (length <= 1) return;
		for (Node* cur = head; cur; cur = cur->next)
		{
			if (cur->next)
			{
				Node* to_delte = cur->next;
				cur->next = cur->next->next;
				delete_node(to_delte);
			}
		}
	}

	void insert_sorted(int value)
	{
		if (!length || value <= head->data)
		{
			insert_front(value);
		}
		else if (tail->data < value)
		{
			insert_end(value);
		}
		else
		{
			for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
			{
				if (cur->data >= value)
				{
				}
			}
		}
	}

	void embed_after(Node* node, int value)
	{
		Node* item = new Node(value);
		++length;
		item->next = node;
		node->next = item;
	}

	int max(Node* cur = nullptr, bool is_first_call = true)
	{
		if (is_first_call)
		{
			// Trigger the recursion to follow the standard naming convention
			return this->max(this->head, false);
		}
		if (cur == nullptr)
		{
			return INT_MIN;
		}
		return std::max(cur->data, this->max(cur->next, false));
	}

	void remove_last_occurrence(int val)
	{
		Node* last_found = nullptr;
		Node* last_prv = nullptr;
		for (Node *cur = head, *prv = nullptr; cur; prv = cur, cur = cur->next)
		{
			if (cur->data == val)
			{
				last_prv = prv;
				last_found = cur;
			}
		}
		if (last_found)
		{
			if (last_found == head)
			{
				delete_front();
			}
			else
			{
				delete_next_node(last_prv);
			}
		}
	}

	void swap_head_tail()
	{
		Node* before_last = get_nth(length - 1);
		before_last->next = head;
		tail->next = head->next;;
		head->next = nullptr;
		swap(head, tail);
	}

	void left_rotate(int k)
	{
		k %= length; // remove cycles.
		assert(k > 0);
		Node* node = get_nth(k);
		tail->next = head;
		head = node->next;
		node->next = nullptr;
		tail = node;
	}

	void remove_duplicate()
	{
		for (Node* cur = head; cur; cur = cur->next)
		{
			for (Node *cur2 = cur->next, *prv = cur; cur2;)
			{
				if (cur->data == cur2->data)
				{
					delete_next_node(prv);
					cur2 = prv->next;
				}
				else
				{
					prv = cur2, cur2 = cur2->next;
				}
			}
		}
	}

	Node* move_to_end(Node* cur, Node* prv)
	{
		Node* next = cur->next;
		tail->next = cur;
		if (prv)
		{
			prv->next = next;
		}
		else
		{
			head = next;
		}

		tail = cur;
		tail->next = nullptr;
		return next;
	}

	void move_key_occurrence_to_end(int key)
	{
		if (length <= 1) return;
		int len = length;
		for (Node *cur = head, *prv = nullptr; len--;)
		{
			if (cur->data == key)
			{
				cur = move_to_end(cur, prv);
			}
			else
			{
				prv = cur, cur = cur->next;
			}
		}
	}

	void arrange_odd_even()
	{
		// Handle edge cases
		if (length <= 2)
		{
			return;
		}

		// Initialize pointers for odd and even nodes
		Node* even_head = head->next;
		Node* odd = head;
		Node* even = even_head;

		// Process nodes in pairs to maintain correct ordering
		while (even && even->next)
		{
			// Save next odd node
			Node* next_odd = even->next;

			// Connect even node to skip the odd node
			even->next = next_odd->next;

			// Connect odd node to next odd position
			odd->next = next_odd;

			// Advance pointers
			odd = next_odd;
			even = even->next;
		}

		// Connect end of odd list to start of even list
		odd->next = even_head;

		// Update tail to last even node
		tail = even ? even : odd;

		// Ensure list is properly terminated
		if (even)
		{
			even->next = nullptr;
		}
	}

	void insert_after(Node* src, Node* target)
	{
		assert(src && target);
		target->next = src->next;
		src->next = target;
		++length;
	}

	void insert_alternate(LinkedList& another)
	{
		if (!another.length)
		{
			return;
		}
		if (!length)
		{
			length = another.length;
			head = another.head;
			tail = another.tail;
		}
		else
		{
			Node* cur2 = another.head;
			for (Node* cur1 = head; cur1 && cur2;)
			{
				Node* cur2_next_tmp = cur2->next;
				insert_after(cur1, cur2);
				another.length--;
				cur2 = cur2_next_tmp;
				if (cur1 == tail)
				{
					tail = another.tail;
					cur1->next->next = cur2;
					length += another.length;
					break;
				}
				cur1 = cur1->next->next;
			}
		}
	}

	void insert_alternate2(LinkedList& list)
	{
		Node* cur1 = head;
		Node* cur2 = list.head;
		while (cur1 && cur2)
		{
			Node* cur1_next = cur1->next;
			Node* cur2_next = cur2->next;
			cur1->next = cur2;
			cur2->next = cur1_next;
			cur1 = cur1_next;
			cur2 = cur2_next;
		}
		tail = list.tail;
	}

	void remove_all_repeated()
	{
		for (Node *cur = head, *prv = nullptr; cur;)
		{
			if (cur->next && cur->data == cur->next->data)
			{
				int cur_data = cur->data;
				while (cur && cur_data == cur->next->data)
				{
					delete_next_node(cur);
					cur = cur->next;
				}
				if (!prv)
				{
					delete_front();
				} else
				{
					delete_next_node(prv);
				}
			} else
			{
				prv = cur; cur = cur->next;
			}
		}
	}

	void add_number(LinkedList& another)
	{
		Node* my_cur = head;
		Node* his_cur = another.head;
		int carry = 0, my_value, his_value;
		while (my_cur || his_cur)
		{
			my_value = 0, his_value = 0;
			if (my_cur)
			{
				my_value = my_cur->data;
			}
			if (his_cur)
			{
				his_value = his_cur->data;
				his_cur = his_cur->next;
			}
			my_value += his_value + carry;
			carry = my_value / 10;
			my_value = my_value % 10;
			if (my_cur)
			{
				my_cur->data = my_value;
				my_cur = my_cur->next;
			}
			else
			{
				insert_end(my_value);
			}
		}
		if (carry)
		{
			insert_end(carry);
		}
	}

	pair<Node*, pair<Node*, Node*>> reverse_subchain(Node* cur_head, int k)
	{
		Node* cur_tail = cur_head;
		Node* prv = cur_head;
		cur_head = cur_head->next;
		for (int s = 0; s < k-1 && cur_head; s++ )
		{
			Node* next = cur_head->next;
			cur_head->next = prv;

			prv = cur_head;
			cur_head = next;
		}
		return make_pair(prv, make_pair(cur_tail, cur_head));
	}

	void reverse_chain(int k)
	{
		Node* last_tail = nullptr;
		Node* next_chain_head = head;
		head = nullptr;

		while (next_chain_head)
		{
			auto p = reverse_subchain(next_chain_head, k);
			Node* chain_head = p.first;
			Node* chain_tail = p.second.first;
			next_chain_head = p.second.second;
			tail = chain_tail;
			if (!head)
			{
				head = chain_head;
			} else
			{
				last_tail->next = chain_head;
			}
			last_tail = chain_tail;
		}
		tail->next = nullptr;
	}

	////////////////////////////////////////////////////////////
};

void test1()
{
	cout << "\n\ntest1\n";
	LinkedList list;

	list.insert_end(1);
	list.insert_end(2);
	list.insert_end(3);
	// some actions
	list.print();

	string expected = "1 2 3";
	string result = list.debug_to_string();
	if (expected != result)
	{
		cout << "no match:\nExpected: " <<
			expected << "\nResult  : " << result << "\n";
		assert(false);
	}
	list.debug_print_list("********");
}

void test2()
{
	cout << "\n\ntest2\n";
	LinkedList list;

	list.insert_end(1);
	list.insert_end(2);
	list.insert_end(3);
	list.insert_end(4);
	// some actions
	list.print();

	string expected = "1 2 3 4";
	string result = list.debug_to_string();
	if (expected != result)
	{
		cout << "no match:\nExpected: " << expected << "\nResult  : " << result << "\n";
		assert(false);
	}
	list.debug_print_list("********");
}


int main()
{
	// test1();
	// test2();
	//test3();

	// must see it, otherwise RTE
	// cout << "\n\nNO RTE\n";

	LinkedList list;
	list.insert_end(1);
	list.insert_end(2);
	list.insert_end(3);
	list.insert_end(4);
	list.insert_end(5);
	list.reverse_chain(3);

	list.print();

	return 0;
}

