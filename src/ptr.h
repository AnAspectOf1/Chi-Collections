#ifndef _CHI_PTR_H
#define _CHI_PTR_H

#include "exception.h"
#include "int.h"
#include <cstdlib>
#include <cstring>
#include <new>


namespace chi {

	template <class T>
	class Ptr {
	public:
		virtual ~Ptr() {}

		virtual T* ptr() = 0;
		virtual const T* ptr() const = 0;
	
		T& operator*() {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *this->ptr();
		}
		const T& operator*() const	{
					CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
					return *this->ptr();
		}

		T* operator->()	{ return this->ptr(); }
		const T* operator->() const	{ return this->ptr(); }
		operator T*()	{ return (T*)this->ptr(); }
		operator const T*() const	{ return this->ptr(); }

		template <class D = T>
		D& val() {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *(D*)this->ptr();
		}
		template <class D = T>
		const D& val() const {
			CHI_ASSERT( this->ptr() == 0, "Can't dereference the pointer, it isn't allocated." );
			return *(const D*)this->ptr();
		}
	};

	template <class T>
	class UPtr : public Ptr<T> {
	private:
		UPtr<T>& operator=( const UPtr<T>& otheronst ) {}
		UPtr( const UPtr<T>& copy ) {}

	protected:
		bool is_owner;

		void free() {
			delete this->_ptr;
		}

		void init( const T& value ) {
			this->_ptr = ::malloc( sizeof(T) );
			if ( this->_ptr == 0 )
				throw AllocException();

			*((T*)this->_ptr) = value;
		}

	public:
		UPtr() : is_owner(false) {}

		UPtr( const T& copy ) : is_owner(true) {
			this->init( copy );
		}
		
		UPtr( UPtr<T>&& other ) : is_owner(true) {
			this->_ptr = const_cast<void*>((const void*)other.ptr());
			other.is_owner = false;
		}

		~UPtr()	{ if (is_owner) this->free(); }


		template <class A = T>
		void alloc()	{
			this->release();

			this->_ptr = new (std::nothrow) A;
			if ( this->_ptr == 0 )
				throw AllocException();
			this->is_owner = true;
		}

		template <class I>
		void assign( const I& value ) {
			this->release();

			this->_ptr = new (std::nothrow) I( value );
			if ( this->_ptr == 0 )
				throw AllocException();
			this->is_owner = true;
		}

		void take( UPtr<T>& other ) {
			this->release();

			other.is_owner = false;
			this->_ptr = const_cast<void*>((const void*)other.ptr());
			this->is_owner = true;
		}

		T* ptr()	{ return this->_ptr; }
		const T* ptr() const	{ return this->_ptr; }

		void release() {
			if (this->is_owner) {
				this->free();
				this->is_owner = false;
			}
		}

		UPtr<T>& operator=( UPtr<T>&& other ) {
			if ( this == &other )	return *this;

			this->release();

			other.is_owner = false;
			this->_ptr = other.ptr();
			this.is_owner = true;

			return *this;
		}
	};

	template <class T>
	class SPtr : public Ptr<T> {
	protected:
		T* _ptr;
		Size* count;

		void release() {
			CHI_ASSERT( this->count == (Size*)-1, "Double release SPtr" );

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
		SPtr() : _ptr(0), count(0) {}

		// Constructor for joining a shared pointer
		SPtr( const SPtr<T>& copy ) {
			this->_ptr = copy._ptr;
			this->count = copy.count;
			(*this->count)++;
		}

		~SPtr()	{ this->release(); }

		SPtr<T>& operator=( const SPtr<T>& other ) {
			CHI_ASSERT( this->_ptr != 0, "Assigning a pointer that is already assigned" );
			if ( this == &other )	return *this;

			this->_ptr = other._ptr;
			this->count = other.count;
			(*this->count)++;
		
			return *this;
		}

		template <class D>
		void alloc() {
			CHI_ASSERT( this->_ptr != 0, "Pointer already allocated" );

			this->_ptr = new (std::nothrow) D();
			if ( this->_ptr == 0 )	throw AllocException();
			this->count = new (std::nothrow) Size(1);
			if ( this->count == 0 )	throw AllocException();
		}

		template <class D>
		void alloc( const D& value ) {
			CHI_ASSERT( this->_ptr != 0, "Pointer already allocated" );

			this->_ptr = new (std::nothrow) D(value);
			if ( this->_ptr == 0 )	throw AllocException();
			this->count = new (std::nothrow) Size(1);
			if ( this->count == 0 )	throw AllocException();
		}

		T* ptr()	{ return this->_ptr; }
		const T* ptr() const	{ return this->_ptr; }
	};
}

#endif//_CHI_PTR_H
