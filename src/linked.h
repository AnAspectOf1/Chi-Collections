#ifndef _CHI_LINKED_H
#define _CHI_LINKED_H

#include "exception.h"
#include "int.h"
#include "list.h"
#include <new>

// The Linker class represents a doubly-linked list.

namespace chi {

	template <class T>
	class LinkedList;

	template <class T>
	class Link {
		friend LinkedList<T>;

	protected:
		Link<T>* _prev;
		Link<T>* _next;
		T element;

	public:
		Link() {}
		Link( const T& copy ) : element( copy ) {}

		Link<T>* prev()	{ return this->_prev; }
		const Link<T>* prev() const	{ return this->_prev; }

		Link<T>* next()	{ return this->_next; }
		const Link<T>* next() const	{ return this->_next; }


		Link<T>& operator=( const T& element ) {
			this->element = element;
		}

		T& operator*()	{ return this->element; }
		const T& operator*() const	{ return this->element; }
	};

	template <class T>
	struct LinkerInfo {
		Size ref_count;
		Link<T>* head;
		SSize first;
		Size last;
	};

	template <class T>
	class LinkedList : public virtual List<T> {
		LinkerInfo<T>* info;

	protected:
		Link<T>* linkAt( SSize index ) const {
			if ( index == 0 ) {
				CHI_ASSERT( this->info->head == 0, "No head element available" );
				return this->info->head;
			}
			else if ( index > 0 )
				return linkAtForward( index );
			else// if ( index < 0 )
				return linkAtBackward( -index );
		}

		Link<T>* linkAtBackward( Size index ) const {
			Link<T>* e = this->info->head;
			for ( Size i = 0; i < index; i++ ) {
				CHI_ASSERT( e == 0, "Element at %d non-existant. Last one found at %d", (int)index, (int)i );				

				e = e->_prev;
			}
			return e;
		}

		Link<T>* linkAtForward( Size index ) const {
			Link<T>* e = this->info->head;
			for ( Size i = 0; i < index; i++ ) {
				CHI_ASSERT( e == 0, "Element at %d non-existant. Last one found at %d", (int)index, (int)i );
				printf("last: %d\n", this->info->last);

				e = e->_next;
			}
			return e;
		}

		void release() {
			CHI_ASSERT( this->info == 0, "Double free of SharedLinker" );
			printf("Linker release %p %d %d %p\n", this->info, this->info->ref_count, this->count(), this->info->head);
			this->info->ref_count--;
			if ( this->info->ref_count == 0 ) {
				Link<T>* l = this->firstLink();
				while ( l != this->info->head ) {
					Link<T>* next = l->_next;
					delete l;
					l = next;
#ifndef NDEBUG
					l->_prev = 0;
					this->info->first++;
#endif
				}

				l = this->lastLink();
				while ( l != this->info->head ) {
					Link<T>* prev = l->_prev;
					delete l;
					l = prev;
#ifndef NDEBUG
					printf( "%p ->_next = 0;\n", l );
					l->_next = 0;
					this->info->last--;
#endif
				}

				delete this->info->head;
#ifndef NDEBUG
				this->info->head = 0;
#endif

				printf( "Releasing linkedlist %p els\n", this->info );
				delete this->info;
#ifndef NDEBUG
				this->info = 0;
#endif
			}
		}

	public:
		LinkedList() {
			this->info = new (std::nothrow) LinkerInfo<T>;
			if ( this->info == 0 )	throw AllocException();

			this->info->head = 0;
			this->info->first = 0;
			this->info->last = 0;
			this->info->ref_count = 1;
		}

		LinkedList( const LinkedList& copy ) : info( copy.info ) {
			this->info->ref_count++;
		}

		~LinkedList()	{ this->release(); }

		T& at( SSize index )	{ return ((Link<T>*)this->linkAt( index ))->element; }
		const T& at( SSize index ) const	{ return ((Link<T>*)this->linkAt( index ))->element; }
		T& at( Size index ) override { return this->at( (SSize)index ); }
		const T& at( Size index ) const override { return this->at( (SSize)index ); }

		
		void append( const T& element ) {
			Link<T>* new_link = new(std::nothrow) Link<T>( element );
			if ( new_link == 0 )	throw AllocException();
			Link<T>* last_link = this->lastLink();

			if ( last_link != 0 ) {
				new_link->_prev = last_link;
				last_link->_next = new_link;
				this->info->last++;
			}
			else
				this->info->head = last_link;
		}

		/*void prepend( const T& element ) {
			Link<T>* new_link = new(std::nothrow) Link<T>( element );
			if ( new_link == 0 )	throw AllocException();

			if ( this->_prev == 0 )
				this->_prev = new_link;
			else {
				new_link->_next = this;
				new_link->_prev = this->_prev;
				this->_prev->_next = new_link;
				this->_prev = new_link;
			}
		}*/

		Size count() const override	{ return this->info->last - this->info->first + ( this->info->head != 0 ? 1 : 0 ); }

		Link<T>* firstLink() {
			return linkAtBackward( this->info->first );
		}
		const Link<T>* firstLink() const {
			return linkAtBackward( this->info->first );
		}

		Link<T>* lastLink() {
			return linkAtForward( this->info->last );
		}
		const Link<T>* lastLink() const {
			return linkAtForward( this->info->last );
		}

		LinkedList& operator=( const LinkedList& other ) {
			if ( this == &other )	return *this;

			this->release();
			this->info = other.info;
			this->info->ref_count++;

			return *this;
		}
	};

}

#endif//_CHI_LINKED_H
