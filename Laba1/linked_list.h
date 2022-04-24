#pragma once

#include <string>

template <typename T>
class linked_list
{
    struct node
    {
        T data;
        node *next;
        node *previous;

        explicit node(T data, node *next = nullptr, node *previous = nullptr)
        : data{std::move(data)}, next{next}, previous{previous} {}
    };
    size_t size_;
    node *head_;
    node *tail_;

public:
    linked_list()  : size_{}, head_{}, tail_{}	{}
    ~linked_list() { clear(); }

    linked_list(const linked_list &other) : size_{}
    {
	    clear();
        auto current{other.head_};
        while (current)
        {
	        push_back(current->data);
            current = current->next;
        }
    }

    void push_front(const T &data)
    {
	    head_ = new node{data, head_};
		if (!tail_) { tail_ = head_; }
		++size_;
    }

    void push_back(const T &data)
    {
	    if (!head_) { push_front( data ); }
        else
        {
	        tail_ = tail_->next = new node{data, nullptr, tail_};
			++size_;
        }
    }

    void insert(const size_t &index, const T &data)
    {
	    if (index <= size_)
		{
			if (!index)                 { push_front(data); }
			else if (index == size_)    { push_back(data); }
            else
            {
                bool further = index >= size_ / 2;
                node *current{further ? tail_ : head_};
                if (further)
                {
	                for (size_t i{size_ - 1}; i-- != index;) { current = current->previous; }
                    if (current->previous)
                    {
	                    node *previous{current->previous};
                        current->previous = new node{data, current, current->previous};
                        previous->next = current->previous;
                    }
                    else { current->previous = new node{data, current, current->previous}; }
                }
                else
                {
	                for (size_t i{}; i++ != index;) { current = current->next; }
                    if (current->next)
                    {
	                    node *next{current->next};
                        current->next = new node{data, current->next, current};
                        next->previous = current->next;
                    }
                    else { current->next = new node{data, current->next, current}; }
                }
				++size_;
            }
		}
		else
		{
			throw std::out_of_range
			(
				std::string{"Index ("}.append(std::to_string(index))
				.append(") should be lower than or equal to size (").append(std::to_string(size_)).append(")")
			);
		}
    }

    void set(const size_t &index, const T &data) const
    {
	    if ( index < size_ )
		{
			if (!index)					    { head_->data = data; }
			else if (index == size_ - 1)    { tail_->data = data; }
			else
			{
				bool further = index >= size_ / 2;
                node *current{further ? tail_->previous : head_->next};
                if (further)    { for (size_t i{size_ - 1}; --i != index;) { current = current->previous; } }
                else            { for (size_t i{}; ++i != index;) { current = current->next; } }
				current->data = data;
			}
		}
		else
		{
			throw std::out_of_range
			(
				std::string{"Index ("}.append(std::to_string(index))
				.append(") should be lower than size (").append(std::to_string(size_)).append(")")
			);
		}
    }

    void pop_front()
    {
	    if (size_)
		{
			--size_;
			if (!size_)
			{
				delete head_;
				head_ = tail_ = nullptr;
			}
            else
            {
	            node *new_head{head_->next};
				delete head_;
				head_ = new_head;
				head_->previous = nullptr;
            }
		}
    }

    void pop_back()
    {
	    if (size_)
		{
			if (size_ == 1) { pop_front(); }
            else
            {
	            --size_;
				node *new_tail{tail_->previous};
				delete tail_;
				tail_ = new_tail;
				new_tail->next = nullptr;
            }
		}
    }

    void remove(const size_t &index)
    {
	    if (index < size_)
		{
			if (!index)                     { pop_front(); }
			else if (index == size_ - 1)    { pop_back(); }
            else
            {
                bool further = index >= size_ / 2;
                node *current{further ? tail_ : head_};
                if (further)    { for (size_t i{size_ - 1}; --i != index;) { current = current->previous; } }
                else            { for (size_t i{}; ++i != index;) { current = current->next; } }
                const node *to_be_deleted{further ? current->previous : current->next};
                if (further)
                {
	                current->previous = to_be_deleted->previous;
                    to_be_deleted->previous->next = current;
                }
                else
                {
	                current->next = to_be_deleted->next;
                    to_be_deleted->next->previous = current;
                }
            	delete to_be_deleted;
                --size_;
            }
		}
		else 
		{
			throw std::out_of_range
			(
				std::string{"Index ("}.append(std::to_string(index))
				.append(") should be lower than size (").append(std::to_string(size_)).append(")")
			);
		}
    }

    void clear()
    {
	    while (size_) { pop_front(); }
        head_ = tail_ = nullptr;
    }

    T at(const size_t &index) const
    {
        if (index < size_)
        {
            if (!index)             { return head_->data; }
			if (index == size_ - 1) { return tail_->data; }
            bool further = index >= size_ / 2;
        	node *current{further ? tail_ : head_};
            if (further)    { for (size_t i{size_ - 1}; i-- != index;) { current = current->previous; } }
            else            { for (size_t i{}; i++ != index;) { current = current->next; } }
			return current->data;
        }
    	throw std::out_of_range
    	(
    		std::string{"Index ("}.append(std::to_string(index))
    		.append(") should be lower than size (").append(std::to_string(size_)).append(")")
        );
    }

    void print() const
    {
        node *current{head_};
	    for (size_t i{}; i < size_; ++i)
	    {
		    std::cout << current->data << '\n';
            current = current->next;
	    }
    }

    bool is_empty() const   { return size_ == 0; }
    size_t get_size() const { return size_; }
};
