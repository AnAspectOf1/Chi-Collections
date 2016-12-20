#ifndef _CHI_LIST_H
#define _CHI_LIST_H

#include "collection.h"
#include "op.h"
#include "slice.h"


namespace chi {

	template <class T>
	class ListIterator;

	template <class T>
	class List : public Collection<T> {
	public:
		virtual T& _at( Size index ) = 0;
		virtual const T& _at( Size index ) const = 0;


		T& at( Size index ) {
			CHI_ASSERT( index >= this->count(), "Index out of bounds" );
			return this->_at( index );
		}
		const T& at( Size index ) const {
			CHI_ASSERT( index >= this->count(), "Index out of bounds" );
			return this->_at( index );
		}

		bool contains( const T& element ) const	{
			return this->find( element ) < this->count();
		}

		void copy( const T* other, Size count, Size index = 0 ) {
			CHI_ASSERT( index + count > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = index; i < count; i++, j++ ) {
				this->at(j) = other[i];
			}
		}
		void copy( const Collection<T>& other, Size index = 0 ) {
			Size j = 0;
			for ( auto i = other.begin(); i.valid(), i = i.next() ) {
				CHI_ASSERT( j >= this->count(), "Reached list boundaries while copying from collection" );

				this->at(j) = *i;
				j++;
			}
		}
		void copy( const List<T>& other, Size index = 0 ) {
			CHI_ASSERT( index + other.count() > this->count(), "Can't copy more into array than it has allocated" );

			for ( Size i = 0, j = index; i < other.count(); i++, j++ ) {
				this->at(j) = other[i];
			}
		}

		bool equals( const List<T>& other ) const {
			static_assert( op::implementsNotEqual<T>(), "List::equal requires the element type to implement the not-equal '!=' operator" );

			if ( this->count() != other.count() )
				return false;

			for ( Size i = 0; i < other.count(); i++ ) {
				if ( this->at(i) != other.at(i) )
					return false;
			}

			return true;
		}

		// Returns the index of the element, otherwise returns count().
		Size find( const T& element ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i) == element )
					return i;
			}

			return this->count();
		}

		T& first()	{ return this->at(0); }
		const T& first() const	{ return this->at(0); }

		T& last()	{ return this->at( this->count() - 1 ); }
		const T& last() const	{ return this->at( this->count() - 1 ); }

		Size size() const	{ return this->count() * sizeof(T); }

		Slice<T> slice( Size count, Size first = 0 ) {
			CHI_ASSERT( first + count > this->count(), "Can not slice from %d with length %d, this is outside of the lists' count %d", (int)first, (int)count, (int)this->count() );

			return Slice<T>( this, count, first );
		}


		T& operator[]( Size index )	{ return this->at( index ); }
		const T& operator[]( Size index ) const	{ return this->at( index ); }

		bool operator==( const List<T>& other ) const {
			return this->equals( other );
		}
		bool operator!=( const List<T>& other ) const {
			return !this->equals( other );
		}
	};

	struct ListIteratorInfo {
		List<T>* list;
		Size index;
	};

	template <class T>
	class ListIterator : public Iterator<T, ListIteratorInfo> {
		friend List<T>;

	protected:
		ListIterator( List<T>* list, Size index ) : info({list, index}) {}

	public:
		bool valid() const {
			return this->info.index < this->info.list->count();
		}

		T* _get() const override {
			return &this->info.list[ this->info.index ];
		}

		Iterator<T, ListIteratorInfo> _prev() const {
			return ListIterator( this->info.list, this->info.index - 1 );
		}
		Iterator<T, ListIteratorInfo> _next() const {
			return ListIterator( this->info.list, this->info.index + 1 );
		}
	};
}

#endif//_CHI_LIST_H
