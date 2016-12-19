#ifndef _CHI_PTR_H
#define _CHI_PTR_H

#include "exception.h"
#include <cstdlib>
#include <cstring>


namespace chi {

	template <class T>
	class CPtr {
	public:
		virtual ~CPtr() {}

		virtual const T* ptr() const = 0;

		const T& operator*() const	{
					CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
					return *this->ptr();
		}

		const T* operator->() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return this->ptr();
		}

		operator const T*() const	{
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return this->ptr();
		}

		template <class D = T>
		const D& val() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *(const D*)this->ptr();
		}

		bool operator==( const T* other ) const	{ return this->ptr() == other; }
		bool operator!=( const T* other ) const	{ return this->ptr() != other; }
	};

	template <class T>
	class Ptr : public virtual CPtr<T> {
	public:

		virtual T* ptr() const = 0;
	
		T& operator*() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *this->ptr();
		}

		T* operator->() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return this->ptr();
		}

		operator T*() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return (T*)this->ptr();
		}

		template <class D = T>
		D& val() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *(D*)this->ptr();
		}
	};

	template <class T>
	class CSPtr : public virtual CPtr<T> {
	protected:
		T* _ptr;

		void release() {
			CHI_ASSERT( this->count == (Size*)-1, "Double release (C)SPtr" );

			if ( this->_ptr != 0 ) {
				(*this->count)--;
				if ( *this->count == 0 ) {
					delete ((T*)this->_ptr);
					delete this->count;
#ifndef NDEBUG
					this->_ptr = (T*)-1;
					this->count = (Size*)-1;
#endif
				}
			}
		}

	public:
		Size* count;

		CSPtr() : _ptr(0), count(0) {}

		// Constructor for joining a shared pointer
		CSPtr( const CSPtr<T>& copy ) : _ptr(copy._ptr), count(copy.count) {
			if ( this->_ptr != 0 )
				(*this->count)++;
		}

		~CSPtr()	{ this->release(); }

		CSPtr<T>& operator=( const CSPtr<T>& other ) {
			if ( this == &other )	return *this;

			this->release();

			this->_ptr = other._ptr;
			this->count = other.count;
			if ( this->_ptr != 0 )
				(*this->count)++;
		
			return *this;
		}

		template <class D>
		void alloc() {
			CHI_ASSERT( this->_ptr != 0, "Pointer already allocated" );

			this->_ptr = new (std::nothrow) D();
			if ( this->_ptr == 0 )	throw AllocException();
			this->count = new (std::nothrow) Size(1);
			if ( this->count == 0 )	{
				delete this->_ptr;
				throw AllocException();
			}
		}

		template <class D>
		void alloc( const D& value ) {
			CHI_ASSERT( this->_ptr != 0, "Pointer already allocated" );

			this->_ptr = new (std::nothrow) D( value );
			if ( this->_ptr == 0 )	throw AllocException();
			this->count = new (std::nothrow) Size(1);
			if ( this->count == 0 )		{
				delete this->_ptr;
				throw AllocException();
			}
		}

		bool allocated() const	{ return this->_ptr != 0; }

		const T* ptr() const	{ return this->_ptr; }
	};

	template <class T>
	class SPtr : public CSPtr<T>, public virtual Ptr<T> {
	public:
		SPtr() : CSPtr<T>() {}
		SPtr( const CSPtr<T>& other ) : CSPtr<T>( other ) {}
		SPtr( const SPtr<T>& copy ) : CSPtr<T>( copy ) {}

		T* ptr() const	{ return this->_ptr; }

		/*template <class D>
		void alloc()	{ CSPtr<T>::alloc<D>(); }
		template <class D>
		void alloc( const D& value)	{ CSPtr<T>::alloc<D>( value ); }

		template <class D>
		static SPtr<T> allocNew() {
			SPtr<T> pointer;
			pointer.alloc<D>();
			return pointer;
		}

		template <class D>
		static SPtr<T> allocNew( const D& value ) {
			SPtr<T> pointer;
			pointer.alloc( value );
			return pointer;
		} */

		SPtr<T>& operator=( const CSPtr<T>& other ) {
			CSPtr<T>::operator=( other );
			return *this;
		}
	};
}

#endif//_CHI_PTR_H
