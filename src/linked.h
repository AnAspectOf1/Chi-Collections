#ifndef _CHI_LINKED_H
#define _CHI_LINKED_H

#include "collection.h"
#include "exception.h"
#include "int.h"
#include "list.h"
#include <new>

// TODO: Create a LinkedRefList (or some other name) that makes use of RefLink which should be able to maintain a derived instance of T.

namespace chi {

	template <class T>
	class LinkedList;

	template <class T>
	class Link {
		friend LinkedList<T>;

	protected:
		Link<T>* _next;

		Link() : _next(0) {}
		Link( const T& value ) : _next(0), element(value) {}

	public:
		T element;

		Link<T>* next()	{ return this->_next; }
		const Link<T>* next() const	{ return this->_next; }

		T& operator*()	{ return this->element; }
		const T& operator*() const	{ return this->element; }
	};

	template <class T>
	class LinkIterator : public Iterator<T, Link<T>*> {
		friend LinkedList<T>;

	protected:
		LinkIterator( Link<T>* link ) : info(link) {}

		bool valid() const {
			return this->info != 0;
		}

		T* _get() const override {
			return &this->info->element;
		}

		Iterator<T, Link<T>*> _prev() const {
			return LinkIterator( this->info );
		}
		Iterator<T, Link<T>*> _next() const {
			return LinkIterator( this->info );
		}
	};

	/*template <class T>
	class RefLink {
	protected:
		RefLinkBase* _next;
		T* e;

		RefLinkBase() : _next(0) {}

	public:
		template <class E>
		E& element()	{ return *(E*)this->e; }
		template <class E>
		const E& element() const	{ return *(E*)this->e; }

		RefLinkBase* next()	{ return this->_next; }
		const RefLinkBase* next() const	{ return this->_next; }

		T& operator*()	{ return *this->e; }
		const T& operator*() const	{ return *this->e; }
	};*/

	// The LinkedList class is a list that allocates a new element on the heap every time an element is added.
	// The is an efficient class for lists that need to have single elements added to it very often, instead of using an array which needs to be reallocated every now and then copied over.
	template <class T>
	class LinkedList : public List<T> {
	protected:
		Link<T>* _head;
		Size _count;

		void _init( Link<T>** head, Size* count, const LinkedList<T>& copy ) {
			Link<T>* link = copy._head;
			if ( link == 0 ) {
				*head = 0;
				*count = 0;
			}
			else {
				*head = new (std::nothrow) Link<T>( **link);
				if ( *head == 0 )	throw AllocException();
				*count = 1;
				Link<T>* our_link = *head;

				while ( (link = link->_next) != 0 ) {
					our_link->_next = new (std::nothrow) Link<T>( **link );
					if ( our_link->_next == 0 )	throw AllocException();
					(*count)++;
					our_link = our_link->_next;
				}
			}
		}

		Link<T>* _linkAt( Size index ) const {
			Link<T>* link = this->_head;
			for ( Size i = 0; i < index; i++ ) {
				link = link->_next;
			}
			return link;
		}

		void freeLink( Link<T>* link ) {
			if ( link != 0 ) {
				this->freeLink( link->_next );
#ifndef NDEBUG
				link->_next = (Link<T>*)-1;
#endif
				delete link;
			}
		}

	public:
		LinkedList() : _head(0), _count(0) {}
		LinkedList( const LinkedList& copy ) {
			this->_init( &this->_head, &this->_count, copy );
		}
		~LinkedList() {
			if ( this->_head != 0 )
				this->freeLink( this->_head );
		}

		void append( const T& value ) {
			if ( this->_head == 0 ) {
				this->_head = new (std::nothrow) Link<T>( value );
				if ( this->_head == 0 )	throw AllocException();
				this->_count = 1;
			}
			else {
				Link<T>& link = this->lastLink();
				link._next = new (std::nothrow) Link<T>( value );
				if ( link._next == 0 )	throw AllocException();
				this->_count++;
			}
		}

		T& _at( Size index ) override	{ return *this->linkAt( index ); }
		const T& _at( Size index ) const override	{ return *this->linkAt( index ); }

		Size count() const override	{ return this->_count; }

		T& head()	{ return **this->_head; }
		const T& head() const	{ return **this->_head; }

		Link<T>& lastLink()	{ return this->linkAt( this->_count - 1 ); }
		const Link<T>& lastLink() const	{ return this->linkAt( this->_count - 1 ); }
		
		Link<T>& linkAt( Size index )	{ return *this->_linkAt( index ); }
		const Link<T>& linkAt( Size index ) const	{ return *this->_linkAt( index ); }


		LinkedList<T>& operator=( const LinkedList<T>& other ) {
			if ( this == &other )	return *this;

			Link<T>* new_head;
			Size new_count;
			this->_init( &new_head, &new_count, other );

			this->freeLink( this->_head );
			
			this->_head = new_head;
			this->_count = new_count;

			return *this;
		}

		LinkedList<T>& operator+=( const T& element ) {
			this->append( element );
			return *this;
		}
	};
}

#endif//_CHI_LINKED_H
