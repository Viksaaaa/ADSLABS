#pragma once

#include <sstream>

enum class replacement { allowed, disallowed };

template<typename K, typename V>
class map
{
	enum class node_color { red, black };
	enum class rotation { right, left };

	struct node
	{
		K key;
		V value;
		node *parent;
		node *left;
		node *right;
		node_color color;

		explicit node(K key,
			V value,
			node *parent = nullptr,
			node *left = nullptr,
			node *right = nullptr,
			const node_color &color = node_color::red)
			: key{ std::move(key) }, value{ std::move(value) }, parent{ parent }, left{ left }, right{ right }, color{ color } {}
	};

	node *root_;

	void destroy_tree(node *&current)
	{
		if (current)
		{
			destroy_tree(current->left);
			destroy_tree(current->right);
			delete current;
			current = nullptr;
		}
	}

	std::pair<bool, bool> insert(node *&initial, const K &key, const V &value, const replacement &flag)
	{
		node *to_be_inserted{ new node{key, value} };
		if (!initial)
		{
			initial = to_be_inserted;
			initial->color = node_color::black;
			return { true, false };
		}
		node *parent{ initial };
		while (true)
		{
			if (key == parent->key)
			{
				if (flag == replacement::allowed)
				{
					parent->value = value;
					return { false, true };
				}
				return { false, false };
			}
			const bool compare = key < parent->key;
			if (compare && !parent->left)
			{
				parent->left = to_be_inserted;
				break;
			}
			if (!compare && !parent->right)
			{
				parent->right = to_be_inserted;
				break;
			}
			parent = compare ? parent->left : parent->right;
		}
		to_be_inserted->parent = parent;
		while (true)
		{
			if (!parent)
			{
				to_be_inserted->color = node_color::black;
				return { true, false };
			}
			if (parent->color == node_color::black) { return { true, false }; }
			node *grand_parent{ parent->parent };
			node *uncle{ parent == grand_parent->left ? grand_parent->right : grand_parent->left };
			if (uncle && uncle->color == node_color::red)
			{
				parent->color = uncle->color = node_color::black;
				grand_parent->color = node_color::red;
				to_be_inserted = grand_parent;
				parent = to_be_inserted->parent;
				continue;
			}
			if (to_be_inserted == parent->right && parent == grand_parent->left)
			{
				rotate(parent, rotation::left);
				to_be_inserted = to_be_inserted->left;
				parent = to_be_inserted->parent;
			}
			else if (to_be_inserted == parent->left && parent == grand_parent->right)
			{
				rotate(parent, rotation::right);
				to_be_inserted = to_be_inserted->right;
				parent = to_be_inserted->parent;
			}
			parent->color = node_color::black;
			grand_parent->color = node_color::red;
			if (to_be_inserted == parent->left && parent == grand_parent->left)
			{
				rotate(grand_parent, rotation::right);
			}
			else if (to_be_inserted == parent->right && parent == grand_parent->right)
			{
				rotate(grand_parent, rotation::left);
			}
			return { true, false };
		}
	}

	void rotate(node *&initial, const rotation &side)
	{
		const bool left = side == rotation::left;
		node *rl{ left ? initial->right : initial->left };
		swap(initial, rl);
		if (left) { initial->right = rl->left; }
		else { initial->left = rl->right; }
		if (left && rl->left) { rl->left->parent = initial; }
		else if (!left && rl->right) { rl->right->parent = initial; }
		if (left) { rl->left = initial; }
		else { rl->right = initial; }
		initial->parent = rl;
	}

	void swap(node *&first, node *&second)
	{
		if (!first->parent) { root_ = second; }
		else if (first == first->parent->left) { first->parent->left = second; }
		else { first->parent->right = second; }
		if (second) { second->parent = first->parent; }
	}

	node *find(node *&initial, const K &key)
	{
		node *current{ initial };
		while (current)
		{
			if (current->key == key) { return current; }
			current = current->key < key ? current->right : current->left;
		}
		return nullptr;
	}

public:
	map() : root_{} {}
	~map() { clear(); }

	std::pair<bool, bool> insert(const K &key, const V &value,
		const replacement &flag = replacement::allowed) {
		return insert(root_, key, value, flag);
	}
	void clear() { destroy_tree(root_); }

	node* find(const K &key) { return find(root_, key); }
};
