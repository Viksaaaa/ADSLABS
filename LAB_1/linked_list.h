#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <ostream>

class linked_list
{
    struct node
    {
        int data;
        node *next;

        explicit node( const int &data = 0, node *next = nullptr );
    };
    size_t size_;
    node *head_;
    node *tail_;

public:
    linked_list();
    ~linked_list();

    friend bool operator==( const linked_list &, const linked_list & );

    friend std::ostream &operator<<( std::ostream &, const linked_list & );

    void push_front( const int & );
    void push_back( const int & );
    void insert( const size_t &, const int & );
    void insert( const size_t &, const linked_list & );

    void set( const size_t &, const int & ) const;

    void pop_front();
    void pop_back();
    void remove( const size_t & );
    void clear();

    int at( const size_t & ) const;

    bool is_empty() const;
    size_t get_size() const;
};

#endif //LINKED_LIST_H
