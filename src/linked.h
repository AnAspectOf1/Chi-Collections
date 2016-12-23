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
	class LinkIterator;
	template <class T>
	class LinkedList;

	template <class T>
	class Link {
		friend LinkIterator<T>;
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
	class LinkIterator : public Iterator<T> {
		friend LinkedList<T>;

	protected:
		Link<T>* link;

		LinkIterator( Link<T>* link ) : link(link) {}

		bool valid() const {
			return this->link != 0;
		}

		T* _get() const override {
			return &this->link->element;
		}

		SPtr<Iterator<T>> _prev() const {
			return SPtr<Iterator<T>>::allocNew( LinkIterator( const_cast<Link<T>*>(this->link) ) );
		}
		SPtr<Iterator<T>> _next() const {
			return SPtr<Iterator<T>>::allocNew( LinkIterator( const_cast<Link<T>*>(this->link) ) );
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

		void _init( Link<T>** head, const LinkedList<T>& copy ) {
			Link<T>* link = copy._head;
			if ( link == 0 )
				*head = 0;

			else {
				*head = new (std::nothrow) Link<T>( **link);
				if ( *head == 0 )	throw AllocException();
				Link<T>* our_link = *head;

				while ( (link = link->_next) != 0 ) {
					our_link->_next = new (std::nothrow) Link<T>( **link );
					if ( our_link->_next == 0 )	throw AllocException();
					our_link = our_link->_next;
				}
			}
		}

		Link<T>& _lastLink() const {
			CHI_ASSERT( this->_head == 0, "This linked list has no links" );

			Link<T>* link = this->_head;

			while ( link->_next != 0 ) {
				link = link->_next;
			}

			return *link;
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
		LinkedList() : _head(0) {}
		LinkedList( const LinkedList& copy ) {
			this->_init( &this->_head, copy );
		}
		~LinkedList() {
			if ( this->_head != 0 )
				this->freeLink( this->_head );
		}

		T& _at( Size index ) const override	{ return **this->_linkAt( index ); }

		SPtr<Iterator<T>> _begin() const override	{ return SPtr<Iterator<T>>::allocNew( LinkIterator<T>( const_cast<Link<T>*>(&this->firstLink()) ) ); }

		SPtr<Iterator<T>> _end() const override	{ return SPtr<Iterator<T>>::allocNew( LinkIterator<T>( const_cast<Link<T>*>(&this->firstLink()) ) ); }

		void append( const T& value ) {
			if ( this->_head == 0 ) {
				this->_head = new (std::nothrow) Link<T>( value );
				if ( this->_head == 0 )	throw AllocException();
			}
			else {
				Link<T>& link = this->lastLink();
				link._next = new (std::nothrow) Link<T>( value );
				if ( link._next == 0 )	throw AllocException();
			}
		}

		// The count function counts the number of links manually
		Size count() const override {
			Size count = 0;
			Link<T>* link = this->_head;

			while ( link != 0 ) {
				count++;

				link = link->next();
			}

			return count;
		}

		T& head() {
			CHI_ASSERT( this->_head == 0, "This linked list has no links" );
			return **this->_head;
		}
		const T& head() const {
			CHI_ASSERT( this->_head == 0, "This linked list has no links" );
			return **this->_head;
		}

		Link<T>& firstLink() {
			CHI_ASSERT( this->_head == 0, "This linked list has no links" );
			return *this->_head; 
		}
		const Link<T>& firstLink() const {
			CHI_ASSERT( this->_head == 0, "This linked list has no links" );
			return *this->_head; 
		}

		Link<T>& lastLink()	{ return this->_lastLink(); }
		const Link<T>& lastLink() const	{ return this->_lastLink(); }
		
		Link<T>& linkAt( Size index )	{ return *this->_linkAt( index ); }
		const Link<T>& linkAt( Size index ) const	{ return *this->_linkAt( index ); }


		LinkedList<T>& operator=( const LinkedList<T>& other ) {
			if ( this == &other )	return *this;

			Link<T>* new_head;
			this->_init( &new_head, other );

			this->freeLink( this->_head );
			
			this->_head = new_head;

			return *this;
		}

		LinkedList<T>& operator+=( const T& element ) {
			this->append( element );
			return *this;
		}
	};
}

#endif//_CHI_LINKED_H
