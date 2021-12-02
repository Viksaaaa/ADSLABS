#include <stdexcept>
#include "binary_search_tree.h"

/*
 * DEQUE BEGIN
 */

binary_search_tree::deque::element::element(const node* data, element* next, element* previous)
	: data{ data }, next{ next }, previous{ previous } {}

binary_search_tree::deque::deque()
	: size_{}, front_{}, back_{} {}

binary_search_tree::deque::~deque()
{
	while (size_)
	{
		pop_front();
	}
}

binary_search_tree::deque &binary_search_tree::deque::operator=(const deque &other)
{
	if (this == &other)
	{
		return *this;
	}
	const auto *current{ other.back() };

	for (size_t i{}; i < other.size(); ++i)
	{
		push_front(current->data);

		current = current->next;
	}
	return *this;
}

bool binary_search_tree::deque::operator==(const deque &other) const
{
	if (size_ != other.size_)
	{
		return false;
	}
	const element *current1{ back_ };
	const element *current2{ other.back_ };

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

void binary_search_tree::deque::push_front(const node *data)
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

void binary_search_tree::deque::push_back(const node *data)
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

void binary_search_tree::deque::pop_front()
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

void binary_search_tree::deque::pop_back()
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

const binary_search_tree::deque::element *binary_search_tree::deque::front() const
{
	return front_;
}

const binary_search_tree::deque::element *binary_search_tree::deque::back() const
{
	return back_;
}

size_t binary_search_tree::deque::size() const
{
	return size_;
}

/*
 * DEQUE END
 */

 /*
  * ITERATOR BEGIN
  */

binary_search_tree::iterator::iterator()
	: iterator_type_{}, index_{}, tree_{}, main_{}, additional_{} {}

binary_search_tree::iterator::iterator
(const binary_search_tree *tree, const iterator_type &iterator_type, const position &position)
	: iterator_type_{ iterator_type }
	, index_{ position == position::begin ? 0 : tree->size_ }
	, tree_{ tree }
	, main_{ new deque{} }
	, additional_{ iterator_type == iterator_type::breadth_first ? new deque{} : nullptr }
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
}

binary_search_tree::iterator::iterator(const iterator &other)
	: iterator_type_{ other.iterator_type_ }
	, index_{ other.index_ }
	, tree_{ other.tree_ }
	, main_{ new deque{} }
	, additional_{ iterator_type_ == iterator_type::breadth_first ? new deque{} : nullptr }
{
	*main_ = *other.main_;

	if (iterator_type_ == iterator_type::breadth_first)
	{
		*additional_ = *other.additional_;
	}
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
	if (iterator_type_ != other.iterator_type_)
	{
		throw std::logic_error("binary_search_tree iterator operator==: can't compare iterators of different types\n");
	}
	if (*tree_ != *other.tree_)
	{
		throw std::logic_error("binary_search_tree iterator operator==: can't compare iterators of different trees\n");
	}
	return *main_ == *other.main_;
}

bool binary_search_tree::iterator::operator!=(const iterator &other) const
{
	bool result = false;

	try
	{
		result = !operator==(other);
	}
	catch (std::logic_error &error)
	{
		throw std::out_of_range(std::string{ "binary_search_tree iterator operator!=: catch error from:\n" }.append(error.what()));
	}
	return result;
}

const int &binary_search_tree::iterator::operator*() const
{
	if (!main_->front())
	{
		throw std::out_of_range("binary_search_tree iterator operator*: node does not exist\n");
	}
	return main_->front()->data->data;
}

binary_search_tree::iterator &binary_search_tree::iterator::operator++()
{
	if (!main_->front())
	{
		throw std::out_of_range("binary_search_tree iterator operator++: node does not exist\n");
	}
	if (iterator_type_ == iterator_type::in_order)
	{
		if (main_->front()->data->right)
		{
			main_->push_front(main_->front()->data->right);

			while (main_->front()->data->left)
			{
				main_->push_front(main_->front()->data->left);
			}
		}
		else
		{
			const node *current{ main_->front()->data };

			main_->pop_front();

			while (main_->front() && main_->front()->data->right == current)
			{
				current = main_->front()->data;

				main_->pop_front();
			}
		}
	}
	else
	{
		const node *parent{ main_->front()->data };

		main_->pop_front();

		if (parent->left)
		{
			main_->push_back(parent->left);
		}
		if (parent->right)
		{
			main_->push_back(parent->right);
		}
		additional_->push_back(parent);
	}
	++index_;

	return *this;
}

binary_search_tree::iterator binary_search_tree::iterator::operator++(int)
{
	auto current{ *this };

	operator++();

	return current;
}

binary_search_tree::iterator &binary_search_tree::iterator::operator--()
{
	if (iterator_type_ == iterator_type::in_order)
	{
		if (main_->front() && main_->front()->data == find_minimum(tree_->root_))
		{
			throw std::out_of_range("binary_search_tree iterator operator--: node does not exist\n");
		}
		if (!main_->front())
		{
			if (!tree_->root_)
			{
				throw std::out_of_range("binary_search_tree iterator operator--: tree is empty\n");
			}
			main_->push_front(tree_->root_);

			while (main_->front()->data->right)
			{
				main_->push_front(main_->front()->data->right);
			}
		}
		else if (main_->front()->data->left)
		{
			main_->push_front(main_->front()->data->left);

			while (main_->front()->data->right)
			{
				main_->push_front(main_->front()->data->right);
			}
		}
		else
		{
			const node *current{ main_->front()->data };

			main_->pop_front();

			while (main_->front() && main_->front()->data->left == current)
			{
				current = main_->front()->data;

				main_->pop_front();
			}
		}
	}
	else
	{
		if (main_->front() && main_->front()->data == tree_->root_)
		{
			throw std::out_of_range("binary_search_tree iterator operator--: node does not exist\n");
		}
		if (!main_->front() && !additional_->front())
		{
			if (!tree_->root_)
			{
				throw std::out_of_range("binary_search_tree iterator operator--: tree is empty\n");
			}
			main_->push_front(tree_->root_);

			while (main_->front())
			{
				operator++();
			}
			main_->push_back(additional_->back()->data);
			additional_->pop_back();
		}
		else if (!main_->front() && additional_->front())
		{
			main_->push_back(additional_->back()->data);
			additional_->pop_back();
		}
		else
		{
			const node *parent{ additional_->back()->data };

			additional_->pop_back();

			if (parent->left || parent->right)
			{
				main_->pop_back();

				if (parent->left && parent->right)
				{
					main_->pop_back();
				}
			}
			main_->push_front(parent);
		}
	}
	--index_;

	return *this;
}

binary_search_tree::iterator binary_search_tree::iterator::operator--(int)
{
	auto current{ *this };

	operator--();

	return current;
}

binary_search_tree::iterator binary_search_tree::iterator::operator+(const size_t &steps)
{
	if (index_ + steps > tree_->size_)
	{
		throw std::out_of_range("binary_search_tree iterator operator+: node does not exist\n");
	}
	for (size_t i{}; i < steps; ++i)
	{
		try
		{
			operator++();
		}
		catch (std::out_of_range &error)
		{
			throw std::out_of_range(std::string{ "binary_search_tree iterator operator+: catch error from:\n" }.append(error.what()));
		}
	}
	return *this;
}

binary_search_tree::iterator binary_search_tree::iterator::operator-(const size_t &steps)
{
	if (index_ < steps)
	{
		throw std::out_of_range("binary_search_tree iterator operator-: node does not exist\n");
	}
	for (size_t i{}; i < steps; ++i)
	{
		try
		{
			operator--();
		}
		catch (std::out_of_range &error)
		{
			throw std::out_of_range(std::string{ "binary_search_tree iterator operator-: catch error from:\n" }.append(error.what()));
		}
	}
	return *this;
}

/*
 * ITERATOR END
 */

 /*
  * TREE BEGIN
  */

binary_search_tree::node::node(const int &data, node *left, node *right)
	: data{ data }, left{ left }, right{ right } {}

bool operator==(const binary_search_tree &tree1, const binary_search_tree &tree2)
{
#ifndef NDEBUG
	return tree1.root_ == tree2.root_ && tree1.size_ == tree2.size_;
#else
	if (tree1.root_ != tree2.root_ || tree1.size_ != tree2.size_)
	{
		return false;
	}
	binary_search_tree::deque deque1{};
	binary_search_tree::deque deque2{};
	const binary_search_tree::node *current1{ tree1.root_ };
	const binary_search_tree::node *current2{ tree2.root_ };

	deque1.push_back(current1);
	deque2.push_back(current2);

	for (size_t i{}; i < tree1.size_; ++i)
	{
		if (
			deque1.front()->data->left != deque2.front()->data->left
			|| deque1.front()->data->left
			&& deque1.front()->data->left->data != deque2.front()->data->left->data
			|| deque1.front()->data->right != deque2.front()->data->right
			|| deque1.front()->data->right
			&& deque1.front()->data->right->data != deque2.front()->data->right->data
			)
		{
			return false;
		}
		if (deque1.front()->data->left)
		{
			deque1.push_back(deque1.front()->data->left);
			deque2.push_back(deque1.front()->data->left);
		}
		if (deque1.front()->data->right)
		{
			deque1.push_back(deque1.front()->data->right);
			deque2.push_back(deque1.front()->data->right);
		}
		deque1.pop_front();
		deque2.pop_front();
	}
	return true;
#endif // NDEBUG
}

bool operator!=(const binary_search_tree &tree1, const binary_search_tree &tree2)
{
	return !operator==(tree1, tree2);
}

void binary_search_tree::insert(node *&current, const int &data)
{
	if (!current)
	{
		++size_;
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

		--size_;
		current = current->left ? current->left : current->right;

		delete to_be_deleted;
	}
}

void binary_search_tree::insert(const int &data)
{
	insert(root_, data);
}

void binary_search_tree::remove(const int &data)
{
	remove(root_, data);
}

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

bool binary_search_tree::contains(const int &data) const
{
	return contains(root_, data);
}

binary_search_tree::binary_search_tree()
	: iterator_type_{ iterator_type::in_order }, size_{}, root_{} {}

binary_search_tree::~binary_search_tree()
{
	destroy_tree(root_);
}

void binary_search_tree::toggle_iterator_type()
{
	iterator_type_ = iterator_type_ == iterator_type::in_order ? iterator_type::breadth_first : iterator_type::in_order;
}

binary_search_tree::iterator binary_search_tree::begin() const
{
	return { this, iterator_type_, position::begin };
}

binary_search_tree::iterator binary_search_tree::end() const
{
	return { this, iterator_type_, position::end };
}

/*
 * TREE END
 */
