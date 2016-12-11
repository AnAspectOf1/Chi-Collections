#ifndef _CHI_LINKED_H
#define _CHI_LINKED_H

#include "collection.h"
#include "exception.h"
#include "int.h"
#include <new>

// The Linked class represents a doubly-linked list.

namespace chi {

	template <class T>
	class Link;
	class Linked;

	class LinkBase {
		friend Linked;

	protected:
		LinkBase() : prev(0), next(0) {}

	public:		
		LinkBase* prev;
		LinkBase* next;

		template <class T>
		void append( const T& element ) {
			Link<T>* new_link = new(std::nothrow) Link<T>( element );
			if ( new_link == 0 )	throw AllocException();

			if ( this->next == 0 )
				this->next = new_link;
			else {
				new_link->prev = this;
				this->next->prev = new_link;
				this->next = new_link;
			}
		}

		template <class T>
		void prepend( const T& element ) {
			Link<T>* new_link = new(std::nothrow) Link<T>( element );
			if ( new_link == 0 )	throw AllocException();

			if ( this->prev == 0 )
				this->prev = new_link;
			else {
				new_link->next = this;
				new_link->prev = this->prev;
				this->prev->next = new_link;
				this->prev = new_link;
			}
		}
	};

	template <class T>
	class Link : public LinkBase {
		friend Linked;

	protected:
		T element;

	public:
		Link() {}
		Link( const T& copy ) : element( copy ) {}

		Link<T>& operator=( const T& element ) {
			this->element = element;
		}

		T& operator*()	{ return this->element; }
	};

	class Linked : public Collection {
		LinkBase* head;
		SSize _first;
		SSize _last;

	protected:
		LinkBase* linkAt( SSize index ) const {
			if ( index == 0 ) {
				CHI_ASSERT( head == 0, "No head element available." );
				return head;
			}
			else if ( index > 0 )
				return linkAtForward( index );
			else// if ( index < 0 )
				return linkAtBackward( -index );
		}

		LinkBase* linkAtBackward( Size index ) const {
			LinkBase* e = head;
			for ( Size i = 0; i < index; i++ ) {
				CHI_ASSERT( e->prev == 0, "Element at %d non-existant. Last one found at %d.", (int)index, (int)i );				

				e = e->prev;
			}
			return e;
		}

		LinkBase* linkAtForward( Size index ) const {
			LinkBase* e = head;
			for ( Size i = 0; i < index; i++ ) {
				CHI_ASSERT( e->next == 0, "Element at %d non-existant. Last one found at %d.", (int)index, (int)i );

				e = e->next;
			}
			return e;
		}

		LinkBase* lastLink() {
			LinkBase* e = head;
			
			while ( e->next != 0 ) {
				e = e->next;
			}
			return e;
		}

	public:
		Linked() : head(0), _first(0), _last(0) {}
	
		template <class T>
		T& at( SSize index )	{ return ((Link<T>*)linkAt( index )).element; }
		template <class T>
		const T& at( SSize index ) const	{ return ((Link<T>*)this->linkAt( index )).element; }

		template <class T>
		void append( const T& element ) {
			
			if ( this->head != 0 ) {
				LinkBase* last_link = lastLink();

				Link<T>* new_link = new(std::nothrow) Link<T>( element );
				if ( new_link == 0 )	throw AllocException();
	
				last_link->next = new_link;
				_last++;
			}
			else {
				Link<T>* new_link = new(std::nothrow) Link<T>( element );
				if ( new_link == 0 )	throw AllocException();

				this->head = new_link;
			}
		}

		Size count() const	{ return _last - _first + ( this->head != 0 ? 1 : 0 ); }

		LinkBase* first() {
			return linkAtBackward( _first );
		}

		LinkBase* last() {
			return linkAtBackward( _last );
		}
	};

}

#endif//_CHI_LINKED_H
