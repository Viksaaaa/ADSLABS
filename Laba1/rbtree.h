#pragma once

#include <sstream>
#include "linked_list.h"

enum class replacement { allowed, disallowed };

template<typename K, typename V>
class map
{
	enum class node_color	{ red, black };
	enum class rotation		{ right, left };
	
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
		: key{std::move(key)}, value{std::move(value)}, parent{parent}, left{left}, right{right}, color{color} {}
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
		node *to_be_inserted{new node{key, value}};
		if (!initial)
		{
			initial = to_be_inserted;
			initial->color = node_color::black;
			return {true, false};
		}
		node *parent{initial};
		while (true)
		{
			if (key == parent->key)
			{
				if (flag == replacement::allowed)
				{
					parent->value = value;
					return {false, true};
				}
				return {false, false};
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
				return {true, false};
			}
			if (parent->color == node_color::black) { return {true, false}; }
			node *grand_parent{parent->parent};
			node *uncle{parent == grand_parent->left ? grand_parent->right : grand_parent->left};
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
			return {true, false};
		}
 	}

	bool remove(node *&initial, const K &key)
	{
		node *result{find(initial, key)};
		if (!result) { return false; }
		if (result->left && result->right)
		{
			node *predecessor{result->left};
			while(predecessor->right) { predecessor = predecessor->right; }
			result->key = predecessor->key;
			result->value = predecessor->value;
			result = predecessor;
		}
		node *child{!result->right ? result->left : result->right};
		if (result->color == node_color::black)
		{
			result->color = child ? child->color : node_color::black;
			while (true)
			{
				node *parent{result->parent};
				if (!parent) { break; }
				bool left_child = result == parent->left;
				node *sibling{left_child ? parent->right : parent->left};
				if (sibling && sibling->color == node_color::red)
				{
					parent->color = node_color::red;
					sibling->color = node_color::black;
					if (left_child) { rotate(parent, rotation::left); }
					else			{ rotate(parent, rotation::right); }
				}
				left_child = result == parent->left;
				const bool parent_black = parent->color == node_color::black;
				const bool sibling_black = !sibling || sibling->color == node_color::black;
				const bool sibling_left_black = !sibling->left || sibling->left->color == node_color::black;
				const bool sibling_right_black = !sibling->right || sibling->right->color == node_color::black;
				if (parent_black && sibling_black && sibling_left_black && sibling_right_black)
				{
					sibling->color = node_color::red;
					result = parent;
					continue;
				}
				if (!parent_black && sibling_black && sibling_left_black && sibling_right_black)
				{
					sibling->color = node_color::red;
					parent->color = node_color::black;
					break;
				}
				if (left_child && sibling_black && !sibling_left_black && sibling_right_black)
				{
					sibling->color = node_color::red;
					sibling->left->color = node_color::black;
					rotate(sibling, rotation::right);
				}
				else if (!left_child && sibling_black && sibling_left_black && !sibling_right_black)
				{
					sibling->color = node_color::red;
					sibling->right->color = node_color::black;
					rotate(sibling, rotation::left);
				}
				sibling = result == parent->left ? parent->right : parent->left;
				sibling->color = parent->color;
				parent->color = node_color::black;
				if (left_child)
				{
					sibling->right->color = node_color::black;
					rotate(parent, rotation::left);
				}
				else
				{
					sibling->left->color = node_color::black;
					rotate(parent, rotation::right);
				}
				break;
			}
		}
		swap(result, child);
		if (!result->parent && child) { child->color = node_color::black; }
		delete result;
		return true;
	}

	void rotate(node *&initial, const rotation &side)
	{
		const bool left = side == rotation::left;
		node *rl{left ? initial->right : initial->left};
		swap(initial, rl);
		if (left)									{ initial->right = rl->left; }
		else										{ initial->left = rl->right; }
		if (left && rl->left)						{ rl->left->parent = initial; }
		else if (!left && rl->right)				{ rl->right->parent = initial; }
		if (left)									{ rl->left = initial; }
		else										{ rl->right = initial; }
		initial->parent = rl;
	}
	
	void swap(node *&first, node *&second)
	{
		if (!first->parent)						{ root_ = second; }
		else if (first == first->parent->left)	{ first->parent->left = second; }
		else									{ first->parent->right = second; }
		if (second)								{ second->parent = first->parent; }
	}

	void get_keys(node *current, linked_list<K> &keys)
	{
		if (!current) { return; }
		get_keys(current->left, keys);
		keys.push_back(current->key);
		get_keys(current->right, keys);
	}

	linked_list<K> get_keys(node *&initial)
    {
		linked_list<K> result{};
		get_keys(initial,result);
		return result;
    }

	void get_values(node *current, linked_list<V> &values)
	{
		if (!current) { return; }
		get_values(current->left, values);
		values.push_back(current->value);
		get_values(current->right, values);
	}

	linked_list<V> get_values(node *&initial)
    {
		linked_list<V> result{};
		get_values(initial,result);
		return result;
    }

	node *find(node *&initial, const K &key)
	{
		node *current{initial};
		while (current)
		{
			if (current->key == key)		{ return current; }
			current = current->key < key ? current->right : current->left;
		}
		return nullptr;
	}

	void print(node *const &initial, std::string &result) const
	{
		if (!initial) { return; }
		print(initial->left, result);
		std::stringstream current{};
		current << "Key:\t" << initial->key << "\nValue:\t" << initial->value << "\n\n";
		const std::string string{ current.str() };
		std::cout << string;
		result.append(string);
		print(initial->right, result);
	}

public:
	map() : root_{}																{}
	~map()																		{ clear(); }

	std::pair<bool, bool> insert(const K &key, const V &value,
								 const replacement &flag=replacement::allowed)	{ return insert(root_, key, value, flag); }
	bool remove(const K &key)													{ return remove(root_, key); }
	void clear()															    { destroy_tree(root_); }

	linked_list<K> get_keys()													{ return get_keys(root_); }
	linked_list<V> get_values()													{ return get_values(root_); }

	const node* find(const K &key)												{ return find(root_, key); }
	std::string print() const
	{
		std::string result{};
		print(root_, result);
		return result;
	}
};
