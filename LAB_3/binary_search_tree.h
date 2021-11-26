#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

class binary_search_tree
{
	struct node
	{
		int data;
		node *left;
		node *right;

		explicit node(const int &data, node *left = nullptr, node *right = nullptr);
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
};



#endif