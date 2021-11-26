#include "binary_search_tree.h"

binary_search_tree::node::node(const int &data, node *left, node *right)
	: data{ data }, left{ left }, right{ right } {}

void binary_search_tree::destroy_tree(node *&current)
{
	if (current)
	{
		destroy_tree(current->left);
		destroy_tree(current->right);

		delete current;
	}
	current = nullptr;
}

bool binary_search_tree::contains(node *current, const int &data)
{
	return current && (current->data == data || (current->data < data ? contains(current->right, data) : contains(current->left, data)));
}

binary_search_tree::node *binary_search_tree::find_minimum(node *current)
{
	return current ? current->left ? find_minimum(current->left) : current : nullptr;
}

void binary_search_tree::insert(node *&current, const int &data)
{
	if (!current)
	{
		current = new node{ data };
	}
	else if (current->data < data)
	{
		insert(current->right, data);
	}
	else if (current->data > data)
	{
		insert(current->left, data);
	}
}

void binary_search_tree::remove(node *&current, const int &data)
{
	if (!current)
	{
		return;
	}
	if (current->data < data)
	{
		remove(current->right, data);
	}
	else if (current->data > data)
	{
		remove(current->left, data);
	}
	else if (current->left && current->right)
	{
		current->data = find_minimum(current->right)->data;

		remove(current->right, current->data);
	}
	else
	{
		const node *to_be_deleted{ current };

		current = current->left ? current->left : current->right;

		delete to_be_deleted;
	}
}

binary_search_tree::binary_search_tree()
	: root_{} {}

binary_search_tree::~binary_search_tree()
{
	destroy_tree(root_);
}

void binary_search_tree::insert(const int &data)
{
	insert(root_, data);
}

void binary_search_tree::remove(const int &data)
{
	remove(root_, data);
}

bool binary_search_tree::contains(const int &data) const
{
	return contains(root_, data);
}
