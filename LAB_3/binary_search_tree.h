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

	class iterator
	{
		const iterator_type iterator_type_;
		size_t index_;
		const binary_search_tree *tree_;
		deque *main_;

	public:
		iterator();
		iterator(const binary_search_tree *, const iterator_type &);
		iterator(const iterator &);
		~iterator();

		bool has_next() const;
		int next();
	};
	
	size_t size_;
	node *root_;

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

	iterator create_dft_iterator() const;
	iterator create_bft_iterator() const;
};

#endif
