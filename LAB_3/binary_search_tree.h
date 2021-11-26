#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <iterator>

enum class iterator_type
{
	in_order,
	breadth_first
};

enum class position
{
	begin,
	end
};

class binary_search_tree
{
	struct node
	{
		int data;
		node *left;
		node *right;

		explicit node(const int &, node *left = nullptr, node *right = nullptr);
	};

	class iterator
		: public std::iterator<std::bidirectional_iterator_tag, int>
	{
		class deque
		{
			struct element
			{
				const node *data;
				element *next;
				element *previous;

				explicit element(const node *, element *next = nullptr, element *previous = nullptr);
			};

			size_t size_;
			element *front_;
			element *back_;

		public:
			deque();
			~deque();

			bool operator==(const deque &) const;

			void push_front(const node *);
			void push_back(const node *);
			void pop_front();
			void pop_back();

			const element *front() const;
			const element *back() const;
		};

		deque *main_;
		deque *additional_;
		const binary_search_tree *tree_;
		const iterator_type iterator_type_;

	public:
		iterator();
		iterator(const binary_search_tree *, const iterator_type &, const position &);
		iterator(const iterator &);
		~iterator();

		bool operator==(const iterator &) const;
		bool operator!=(const iterator &) const;

		const int &operator*() const;

		iterator &operator++();
		iterator operator++(int);
		iterator &operator--();
		iterator operator--(int);
	};

	node *root_;

	static void insert(node *&, const int &);
	static void remove(node *&, const int &);
	static void destroy_tree(node *&);

	static bool contains(node *, const int &);

	static node *find_minimum(node *);

public:
	binary_search_tree();
	~binary_search_tree();

	void insert(const int &);
	void remove(const int &);

	bool contains(const int &) const;

	iterator lnr_begin() const;
	iterator bf_begin() const;
	iterator lnr_end() const;
	iterator bf_end() const;
};

#endif
