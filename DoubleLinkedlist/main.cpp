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
	Node* prev{};

	Node(int data) : data(data)
	{
	}

	~Node()
	{
		cout << "Destroy value: " << data << " at address " << this << "\n";
	}
};

class DoublyLinkedList
{
private:
	Node* head{};
	Node* tail{};
	Node* prev{};
	int length = 0;

public:
	// Below 2 deletes prevent copy and assign to avoid this mistake
	DoublyLinkedList()
	{
	}

	DoublyLinkedList(const DoublyLinkedList&) = delete;
	DoublyLinkedList& operator=(const DoublyLinkedList& another) = delete;

	~DoublyLinkedList()
	{
		while (head)
		{
			Node* current = head->next;
			delete head;
			head = current;
		}
	}


	void print()
	{
		for (Node* cur = head; cur; cur = cur->next)
			cout << cur->data << " ";
		cout << "\n";
	}

	// These 2 simple functions just to not forget changing the vector and length
	void delete_node(Node* node)
	{
		--length;
		delete node;
	}

	void add_node(Node* node)
	{
		++length;
	}

	void insert_end(int value)
	{
		Node* item = new Node(value);
		add_node(item);

		if (!head)
			head = tail = item;
		else
		{
			link(tail, item);
			tail = item;
		}
	}

	void link(Node* first, Node* second)
	{
		if (first)
		{
			first->next = second;
		}
		if (second)
		{
			second->prev = first;
		}
	}

	void insert_front(int value)
	{
		Node* item = new Node(value);
		add_node(item);

		if (!head) head = tail = item;
		else
		{
			link(item, head);
			head = item;
		}
	}

	void delete_front()
	{
		if (!head)
		{
			return;
		}
		Node* cur = head->next;
		delete_node(head);
		head = cur;
		if (head)
		{
			head->prev = nullptr;
		} else if (!length)
		{
			head = nullptr;
		}
	}

	void delete_end()
	{
		if (!head)
		{
			return;
		}
		Node* cur = tail->prev;
		delete_node(tail);
		tail = cur;
		if (tail)
		{
			head->next = nullptr;
		} else if (!length)
		{
			head = nullptr;
		}
	}

	Node* delete_and_link(Node* cur)
	{
		Node* ret = cur->prev;
		link(cur->prev, cur->next);
		delete_node(cur);
		return ret;
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
			for (Node *cur = head; cur; cur = cur->next)
			{
				if (cur->data == k)
				{
					cur = delete_and_link(cur);
					if (! cur->next)
					{
						tail = cur;
					}
					break;
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
			for (Node *cur = head; cur; cur = cur->next)
			{
				if (cur->data >= value)
				{
					embed_after(cur->prev, value);
					break;
				}
			}
		}
	}

	void embed_after(Node* node_before, int value)
	{
		Node* middle = new Node(value);
		++length;
		Node* after = node_before->next;
		link(node_before, middle);
		link(middle, after);
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

	void insert_alternate(DoublyLinkedList& another)
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

	void insert_alternate2(DoublyLinkedList& list)
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

	void reverse_part(Node* start, int n)
	{
		if (length <= 1) return;
		bool is_head = start == head;
		Node* prv = start;
		start = start->next;
		while (n-- && start)
		{
			Node* next = start->next;
			start->next = prv;

			prv = start;
			start = next;
		}
		if (is_head)
		{
			head = prv;
		}
	}

	void delete_all_with_key(int key)
	{
		Node* cur = head;
		while (cur)
		{
			if (cur->data == key)
			{
				if (cur == head)
				{
					delete_front();
					cur = head;
				} else if (cur == tail)
				{
					Node* to_delete = cur;
					cur->prev->next = nullptr;
					delete to_delete;
					cur = nullptr;
				}
				else
				{
					Node* to_delete = cur;
					cur->prev->next = cur->next;
					cur->next->prev = cur->prev;
					cur = to_delete->next;
					delete to_delete;
				}
			} else
			{
				cur = cur->next;
			}
		}
	}

	void delete_even_positions()
	{
		Node* cur = head;
		while (cur && cur->next)
		{
			Node* to_delete = cur->next;
			link(cur, cur->next->next);
			if (to_delete == tail)
			{
				tail = cur;
			}
			cur = cur->next;
			delete to_delete;
		}
	}

	void delete_odd_positions()
	{
		delete_front();
		delete_even_positions();
	}

	bool is_plaindrome()
	{
		Node* front = head;
		Node* back = tail;
		while (front != back && front->next != back)
		{
			if (front->data == back->data)
			{
				front = front->next;
				back = back->prev;
			} else
			{
				return false;
			}
		}
		return true;
	}

	int middle()
	{
		Node* front = head;
		Node* back = tail;
		while (front != back && front->next != back)
		{
			front = front->next;
			back = back->prev;
		}
		return back->data;
	}

	int middle2()
	{
		Node* slow = head;
		Node* fast = head;
		while (fast && fast->next)
		{
			slow = slow->next;
			fast = fast->next->next;
		}
		return slow->data;
	}

	void swap_kth(int k)
	{
		if (k > length)
		{
			return;
		}

		int kth_back = length - k + 1;
		if (k == kth_back)
		{
			return;
		}

		if (k > kth_back)
		{
			swap(k, kth_back);
		}

		Node* first = get_nth(k);
		Node* last = get_nth(kth_back);

		Node* first_prv = first->prev;
		Node* first_next = first->next;

		Node* last_prv = last->prev;
		Node* last_next = last->next;

		if (k + 1 == kth_back)
		{
			link(first_prv, last);
			link(last, first);
			link(first, last_next);

		} else
		{
			link(first_prv, last);
			link(last, first_next);
			link(last_prv, first);
			link(first, last_next);
		}

		if (k == 1)
		{
			swap(head, tail);
		}

	}
	void reverse()
	{
		if (length <= 1)
		{
			return;
		}
		Node* first = head, *second = head->next;
		while (second)
		{
			Node* first_ = second, *second_ = second->next;
			link(second, first);
			first = first_, second = second_;
		}
		swap(head, tail);
		head->prev = tail->next = nullptr;
	}

	void merge_two_sorted_lists(DoublyLinkedList& other)
	{
		if (!other.head)
		{
			return;
		}
		if (head)
		{
			Node* cur1 = head;
			Node* cur2 = other.head;
			Node* last = { };
			head = nullptr;
			while (cur1 && cur2)
			{
				Node* next { };
				if (cur1->data <= cur2->data)
				{
					next = cur1;
					cur1 = cur1->next;
				} else
				{
					next = cur2;
					cur2 = cur2->next;
				}
				link(last, next);
				last = next;
				if (!head) // first step
				{
					head = last;
				}
			}
			if (cur2)
			{
				tail = other.tail;
				link(last, cur2);
			} else if (cur1)
			{
				link(last, cur1);
			}
		} else
		{
			head = other.head;
			tail = other.tail;
		}
		length += other.length;
	}
};



int main()
{
	DoublyLinkedList list;
	list.insert_end(1);
	list.insert_end(3);
	list.insert_end(5);
	list.insert_end(7);
	list.insert_end(9);

	DoublyLinkedList list2;
	list2.insert_end(2);
	list2.insert_end(4);
	list2.insert_end(6);
	list2.insert_end(8);
	list2.insert_end(10);


	list.merge_two_sorted_lists(list2);
	list.print();
	return 0;
}

