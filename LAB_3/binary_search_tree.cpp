#include <stdexcept>
#include "binary_search_tree.h"

/*
 * DEQUE BEGIN
 */

binary_search_tree::deque::element::element(const node* data, element* next, element* previous)
: data{data}, next{next}, previous{previous} {}

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
		push_front( current->data );

		current = current->next;
	}
	return *this;
}

void binary_search_tree::deque::push_front( const node *data )
{
	++size_;

	if(!front_)
	{
		back_ = front_ = new element{ data };
	}
	else
	{
		front_ = front_->next = new element{ data, nullptr, front_ };
	}
}

void binary_search_tree::deque::push_back( const node *data )
{
	if (!back_)
	{
		push_front( data );
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
: iterator_type_{}, index_{}, tree_{}, main_{} {}

binary_search_tree::iterator::iterator
(const binary_search_tree *tree, const iterator_type &iterator_type)
: iterator_type_{ iterator_type }
, index_{}
, tree_{ tree }
, main_{ new deque{} }
{
	if (tree_->root_)
	{
		main_->push_front(tree_->root_);

		if ( iterator_type == iterator_type::in_order)
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
{
	*main_ = *other.main_;
}

binary_search_tree::iterator::~iterator()
{
	delete main_;
}

bool binary_search_tree::iterator::has_next() const
{
	return index_ != tree_->size_;
}

int binary_search_tree::iterator::next()
{
	if (index_ == tree_->size_)
	{
		throw std::out_of_range("binary_search_tree iterator next: node doesn't exist");
	}
	const int element = main_->front()->data->data;

	if (iterator_type_ == iterator_type::in_order)
	{
		if (main_->front()->data->right)
		{
			main_->push_front(main_->front()->data->right);

			while( main_->front()->data->left )
			{
				main_->push_front( main_->front()->data->left );
			}
		}
		else
		{
			const node *current{ main_->front()->data };

			main_->pop_front();

			while( main_->front() && main_->front()->data->right == current)
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

		if ( parent->left )
		{
			main_->push_back( parent->left );
		}
		if ( parent->right )
		{
			main_->push_back( parent->right );
		}
	}
	++index_;

	return element;
}

/*
 * ITERATOR END
 */

/*
 * TREE BEGIN
 */

binary_search_tree::node::node(const int &data, node *left, node *right)
: data{data}, left{left}, right{right} {}

void binary_search_tree::insert(node *&current, const int &data)
{
	if (!current)
	{
		++size_;
		current = new node{data};
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

		remove(current->right,current->data);
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
: size_{}, root_{} {}

binary_search_tree::~binary_search_tree()
{
	destroy_tree(root_);
}

binary_search_tree::iterator binary_search_tree::create_dft_iterator() const
{
	return { this, iterator_type::in_order };
}

binary_search_tree::iterator binary_search_tree::create_bft_iterator() const
{
	return { this, iterator_type::breadth_first };
}

/*
 * TREE END
 */
