#include <stdexcept>
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

binary_search_tree::iterator::deque::element::element(const node* data, element* next, element* previous)
	: data{ data }, next{ next }, previous{ previous } {}

binary_search_tree::iterator::deque::deque()
	: size_{}, front_{}, back_{} {}

binary_search_tree::iterator::deque::~deque()
{
	while (size_)
	{
		pop_front();
	}
}

bool binary_search_tree::iterator::deque::operator==(const deque &other) const
{
	if (size_ != other.size_)
	{
		return false;
	}
	const element *current1{ front_ };
	const element *current2{ other.front_ };

	for (size_t i{}; i < size_; ++i)
	{
		if (current1->data != current2->data)
		{
			return false;
		}
		current1 = current1->next;
		current2 = current2->next;
	}
	return true;
}

void binary_search_tree::iterator::deque::push_front(const node *data)
{
	++size_;

	if (!front_)
	{
		back_ = front_ = new element{ data };
	}
	else
	{
		front_ = front_->next = new element{ data, nullptr, front_ };
	}
}

void binary_search_tree::iterator::deque::push_back(const node *data)
{
	if (!back_)
	{
		push_front(data);
	}
	else
	{
		++size_;
		back_ = back_->previous = new element{ data, back_, nullptr };
	}
}

void binary_search_tree::iterator::deque::pop_front()
{
	if (front_)
	{
		--size_;

		if (!size_)
		{
			delete back_;

			back_ = front_ = nullptr;

			return;
		}
		const element *to_be_deleted{ front_ };

		front_ = front_->previous;
		front_->next = nullptr;

		delete to_be_deleted;
	}
}

void binary_search_tree::iterator::deque::pop_back()
{
	if (back_)
	{
		if (size_ == 1)
		{
			pop_front();

			return;
		}
		--size_;

		const element *to_be_deleted{ back_ };

		back_ = back_->next;
		back_->previous = nullptr;

		delete to_be_deleted;
	}
}

const binary_search_tree::iterator::deque::element *binary_search_tree::iterator::deque::front() const
{
	return front_;
}

const binary_search_tree::iterator::deque::element *binary_search_tree::iterator::deque::back() const
{
	return back_;
}

binary_search_tree::iterator::iterator()
	: main_{}, additional_{}, tree_{}, iterator_type_{} {}

binary_search_tree::iterator::iterator
(const binary_search_tree *tree, const iterator_type &iterator_type, const position &position)
	: main_{ new deque{} }, additional_{}, tree_{ tree }, iterator_type_{ iterator_type }
{
	if (position == position::begin)
	{
		main_->push_front(tree_->root_);

		if (iterator_type == iterator_type::in_order && tree->root_)
		{
			while (main_->front()->data->left)
			{
				main_->push_front(main_->front()->data->left);
			}
		}
	}
	if (iterator_type == iterator_type::breadth_first)
	{
		additional_ = new deque{};
	}
}

binary_search_tree::iterator::iterator(const iterator &other)
	: main_{ new deque{} }, additional_{}, tree_{ other.tree_ }, iterator_type_{ other.iterator_type_ }
{
	/*
	while ()
	{

	}
	if ( iterator_type_ == iterator_type::breadth_first )
	{
		additional_ = new deque{};


	}
	*/
}

binary_search_tree::iterator::~iterator()
{
	delete main_;

	if (iterator_type_ == iterator_type::breadth_first)
	{
		delete additional_;
	}
}

bool binary_search_tree::iterator::operator==(const iterator &other) const
{
	return *main_ == *other.main_ && tree_ == other.tree_;
}

bool binary_search_tree::iterator::operator!=(const iterator &other) const
{
	return !operator==(other);
}

const int &binary_search_tree::iterator::operator*() const
{
	if (!main_->front())
	{
		throw std::out_of_range("binary_search_tree iterator operator*: node does not exist");
	}
	return main_->front()->data->data;
}

binary_search_tree::iterator& binary_search_tree::iterator::operator++()
{
	return *this;
}

binary_search_tree::iterator binary_search_tree::iterator::operator++(int)
{
	auto current{ *this };

	operator++();

	return current;
}

binary_search_tree::iterator& binary_search_tree::iterator::operator--()
{
	return *this;
}

binary_search_tree::iterator binary_search_tree::iterator::operator--(int)
{
	auto current{ *this };

	operator--();

	return current;
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

binary_search_tree::iterator binary_search_tree::lnr_begin() const
{
	return { this, iterator_type::in_order, position::begin };
}

binary_search_tree::iterator binary_search_tree::bf_begin() const
{
	return { this, iterator_type::breadth_first, position::begin };
}

binary_search_tree::iterator binary_search_tree::lnr_end() const
{
	return { this, iterator_type::in_order, position::end };
}

binary_search_tree::iterator binary_search_tree::bf_end() const
{
	return { this, iterator_type::breadth_first, position::end };
}

