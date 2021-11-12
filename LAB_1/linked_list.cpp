#include "pch.h"
#include <string>
#include "linked_list.h"

linked_list::node::node( const int &data, linked_list::node *next )
: data{ data }, next{ next } {}

linked_list::linked_list()
: size_{}, head_{}, tail_{} {}

linked_list::~linked_list()
{
    while ( size_ != 0 )
    {
        pop_front();
    }
}

bool operator==( const linked_list &list1, const linked_list &list2 )
{
    if ( list1.size_ != list2.size_ )
    {
        return false;
    }
    if ( !list1.size_ )
    {
        return list1.head_ == list2.head_
               && list1.tail_ == list2.tail_
               && list1.head_ == nullptr
               && list1.tail_ == nullptr;
    }
    const linked_list::node *current1{ list1.head_ };
    const linked_list::node *current2{ list2.head_ };

    for ( size_t i{}; i < list1.size_; ++i )
    {
        if ( current1->data != current2->data )
        {
            return false;
        }
        current1 = current1->next;
        current2 = current2->next;
    }
    return true;
}

std::ostream &operator<<( std::ostream &o_stream, const linked_list &list )
{
    if ( list.size_ > 0 )
    {
        linked_list::node *current{ list.head_ };

        for ( size_t i{}; current != nullptr; current = current->next )
        {
            o_stream << i++
                     << ".\t"
                     << current->data
                     << ( current->next == nullptr ? "" : "\n" );
        }
    }
    return o_stream;
}

void linked_list::push_front( const int &data )
{
    head_ = new node{ data, head_ };

    if ( tail_ == nullptr )
    {
        tail_ = head_;
    }
    ++size_;
}

void linked_list::push_back( const int &data )
{
    if ( head_ == nullptr )
    {
        push_front( data );

        return;
    }
    tail_ = tail_->next = new node{ data };
    ++size_;
}

void linked_list::insert( const size_t &position, const int &data )
{
    if ( position < size_ )
    {
        if ( position == 0 )
        {
            push_front( data );

            return;
        }
        if ( position == size_ - 1 )
        {
            tail_->next = new node{ tail_->data };
            tail_->data = data;
            tail_ = tail_->next;
            ++size_;

            return;
        }
        node *current{ head_ };

        for ( size_t i{}; ++i != position; )
        {
            current = current->next;
        }
        current->next = new node{ data, current->next };
        ++size_;
    }
}

void linked_list::insert( const size_t &position, const linked_list &list )
{
    if ( position <= size_ && !( list == linked_list{} ) )
    {
        node *current{ list.head_ };
        node *new_head{ nullptr };
        node *new_tail{ nullptr };

        while ( current != nullptr )
        {
			const auto new_node{ new node{ current->data } };

            if (  new_tail == nullptr )
            {
                new_head = new_tail = new_node;
            }
            else
            {
                new_tail = new_tail->next = new_node;
            }
            current = current->next;
        }
        if ( position == 0 )
        {
            new_tail->next = head_;
            head_ = new_head;
        }
        else if ( position < size_ )
        {
            current = head_;

            for ( size_t i{}; ++i != position; )
            {
                current = current->next;
            }
            new_tail->next = current->next;
            current->next = new_head;
        }
        else
        {
            tail_->next = new_head;
            tail_ = new_tail;
        }
        size_ += list.size_;
    }
}

void linked_list::set( const size_t &position, const int &data ) const
{
    if ( position < size_ )
    {
        if ( position == 0 )
        {
            head_->data = data;

            return;
        }
        if ( position == size_ - 1 )
        {
            tail_->data = data;

            return;
        }
        node *current{ head_->next };

        for ( size_t i{ 1 }; i++ != position; )
        {
            current = current->next;
        }
        current->data = data;
    }
}

void linked_list::pop_front()
{
    if ( size_ != 0 )
    {
        --size_;

        if ( size_ == 0 )
        {
            delete head_;

            head_ = tail_ = nullptr;

            return;
        }
        node *new_head{ head_->next };

        delete head_;

        head_ = new_head;
    }
}

void linked_list::pop_back()
{
    if ( size_ != 0 )
    {
        if ( size_ == 1 )
        {
            pop_front();

            return;
        }
        --size_;

        node *new_tail{ head_ };

        while ( new_tail->next != tail_ )
        {
            new_tail = new_tail->next;
        }
        delete tail_;

        new_tail->next = nullptr;

        tail_ = new_tail;
    }
}

void linked_list::remove( const size_t &position )
{
    if ( position < size_ )
    {
        if ( position == 0 )
        {
            pop_front();

            return;
        }
        if ( position == size_ - 1 )
        {
            pop_back();

            return;
        }
        node *current{ head_ };

        for ( size_t i{}; ++i != position; )
        {
            current = current->next;
        }
        const node *to_be_deleted{ current->next };

        --size_;

        current->next = to_be_deleted->next;

        delete to_be_deleted;
    }
}

void linked_list::clear()
{
    while ( size_ != 0 )
    {
        pop_front();
    }
}

int linked_list::at( const size_t &position ) const
{
    if ( position >= size_ )
    {
        throw std::out_of_range
              (
                  std::string{ "Index (" }
                  .append( std::to_string( position ) )
                  .append( ") should be lower than size (" )
                  .append( std::to_string( size_ ) )
                  .append( ")" )
              );
    }
    if ( position == size_ - 1 )
    {
        return tail_->data;
    }
    const node *current{ head_ };

    for ( size_t i{}; i++ != position; )
    {
        current = current->next;
    }
    return current->data;
}

bool linked_list::is_empty() const
{
    return size_ == 0;
}

size_t linked_list::get_size() const
{
    return size_;
}
