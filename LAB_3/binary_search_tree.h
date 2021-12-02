#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

class binary_search_tree
{
	struct node
	{
		int data;
		node *left;
		node *right;

		explicit node(const int &, node *left = nullptr, node *right = nullptr);
	};

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

		deque &operator=(const deque &);

		bool operator==(const deque &) const;

		void push_front(const node *);
		void push_back(const node *);
		void pop_front();
		void pop_back();

		const element *front() const;
		const element *back() const;
		size_t size() const;
	};

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

	class iterator
	{
		const iterator_type iterator_type_;
		size_t index_;
		const binary_search_tree *tree_;
		deque *main_;
		deque *additional_;

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

		iterator operator+(const size_t &);
		iterator operator-(const size_t &);
	};

	iterator_type iterator_type_;
	size_t size_;
	node *root_;

	friend bool operator==(const binary_search_tree &, const binary_search_tree &);
	friend bool operator!=(const binary_search_tree &, const binary_search_tree &);

	void insert(node *&, const int &);
	void remove(node *&, const int &);
	static void destroy_tree(node *&);

	static bool contains(node *, const int &);

	static node *find_minimum(node *);

public:
	binary_search_tree();
	~binary_search_tree();

	void insert(const int &);
	void remove(const int &);

	bool contains(const int &) const;

	void toggle_iterator_type();
	iterator begin() const;
	iterator end() const;
};

#endif
