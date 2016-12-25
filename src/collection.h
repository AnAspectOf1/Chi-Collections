#ifndef _CHI_COLLECTION_H
#define _CHI_COLLECTION_H

#include "int.h"
#include "ptr.h"


namespace qi {

	template <class T>
	class Iterator {

	public:
		virtual ~Iterator() {}

		virtual T* _get() const = 0;
		virtual SPtr<Iterator<T>> _prev() const = 0;
		virtual SPtr<Iterator<T>> _next() const = 0;
		virtual bool valid() const = 0;
		
		T& get() {
			CHI_ASSERT( !this->valid(), "Can't dereference invalid iterator" );
			return *this->_get();
		}
		const T& get() const {
			CHI_ASSERT( !this->valid(), "Can't dereference invalid iterator" );
			return *this->_get();
		}

		SPtr<Iterator<T>> prev() {
			CHI_ASSERT( this->first(), "Iterator can't reach before the first element" );
			return this->_prev();
		}
		CSPtr<Iterator<T>> prev() const {
			CHI_ASSERT( this->first(), "Iterator can't reach before the first element" );
			return this->_prev();
		}

		SPtr<Iterator<T>> next() {
			CHI_ASSERT( this->last(), "Iterator can't reach beyond the last element" );
			return this->_next();
		}
		CSPtr<Iterator<T>> next() const {
			CHI_ASSERT( this->last(), "Iterator can't reach beyond the last element" );
			return this->_next();
		}

		T& operator*()	{ return this->get(); }
		const T& operator*() const	{ return this->get(); }

		T* operator->()	{ return this->_get(); }
		const T* operator->() const	{ return this->_get(); }

		SPtr<Iterator<T>> operator++()	{ return this->next(); }
		CSPtr<Iterator<T>> operator++() const	{ return this->next(); }
		SPtr<Iterator<T>> operator--()	{ return this->prev(); }
		CSPtr<Iterator<T>> operator--() const	{ return this->prev(); }
	};

	template <class T>
	class Collection {
	public:
		virtual ~Collection() {}

		virtual SPtr<Iterator<T>> _begin() const = 0;
		virtual SPtr<Iterator<T>> _end() const = 0;

		Iterator<T>& begin()	{ return this->_begin(); }
		const Iterator<T>& begin() const	{ return this->_begin(); }

		Iterator<T>& end()	{ return this->_end(); }
		const Iterator<T>& end() const	{ return this->_end(); }
	};
}

#endif//_CHI_COLLECTION_H
