#ifndef _CHI_COLLECTION_H
#define _CHI_COLLECTION_H

#include "int.h"


namespace chi {

	template <class T, Info>
	class Iterator {
	protected:
		Info info;

	public:
		//virtual ~Iterator {}

		virtual T* _get() const = 0;
		virtual Iterator<T, Info> _prev() const = 0;
		virtual Iterator<T, Info> _next() const = 0;
		virtual bool valid() const = 0;
		
		T& get() {
			CHI_ASSERT( !this->valid(), "Can't dereference invalid iterator" );
			return *this->_get();
		}
		const T& get() const {
			CHI_ASSERT( !this->valid(), "Can't dereference invalid iterator" );
			return *this->_get();
		}

		Iterator<T> prev() {
			CHI_ASSERT( this->first(), "Iterator can't reach before the first element" );
			return this->_prev();
		}
		const Iterator<T> prev() const {
			CHI_ASSERT( this->first(), "Iterator can't reach before the first element" );
			return this->_prev();
		}

		Iterator<T> next() {
			CHI_ASSERT( this->last(), "Iterator can't reach beyond the last element" );
			return this->_next();
		}
		const Iterator<T> next() const {
			CHI_ASSERT( this->last(), "Iterator can't reach beyond the last element" );
			return this->_next();
		}

		T& operator*()	{ return this->get(); }
		const T& operator*() const	{ return this->get(); }

		T* operator->()	{ return this->_get(); }
		const T* operator->() const	{ return this->_get(); }
	};

	template <class T>
	class Collection {
	public:
		virtual ~Collection() {}

		virtual Size count() const = 0;

		virtual Iterator<T>& _begin() const = 0;
		virtual Iterator<T>& _end() const = 0;

		Iterator<T>& begin()	{ return this->_begin(); }
		const Iterator<T>& begin() const	{ return this->_begin(); }

		Iterator<T>& end()	{ return this->_end(); }
		const Iterator<T>& end() const	{ return this->_end(); }
	};
}

#endif//_CHI_COLLECTION_H
