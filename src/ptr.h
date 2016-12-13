#ifndef _CHI_PTR_H
#define _CHI_PTR_H

#include "exception.h"
#include <cstdlib>
#include <cstring>


namespace chi {

	class ConstPtrBase {
	protected:
		void* _ptr;

	public:
		ConstPtrBase( void* ptr = 0 ) : _ptr(ptr) {}

		const void* ptr() const	{ return this->_ptr; }
	};
	class PtrBase : public virtual ConstPtrBase {
	public:
		void* ptr() const	{ return this->_ptr; }
	};

	template <class T>
	class ConstPtr : public virtual ConstPtrBase {
	public:
		const T* ptr() const	{ return (const T*)this->_ptr; }
		const T& operator*() const	{ return *this->ptr(); }
		const T* operator->() const	{ return this->ptr(); }
	};

	template <class T>
	class Ptr : public virtual ConstPtr<T>, public virtual PtrBase {
	public:
		T* ptr()	{ return (T*)this->_ptr; }
		T& operator*()	{ return *this->ptr(); }
		T* operator->()	{ return this->ptr(); }
	};

	template <class T>
	class ConstManPtr : public virtual ConstPtr<T> {
	protected:
		Size size;

		void init( const T& value ) { printf( "init\n" );
			this->_ptr = ::malloc( sizeof(T) );
			if ( this->_ptr == 0 )
				throw AllocException();

			*((T*)this->_ptr) = value;
		}

	public:
		ConstManPtr() : ConstPtrBase(0), size(0) {}

		ConstManPtr( const T& copy ) : size(sizeof(T)) {
			this->init( copy );
		}

		ConstManPtr( const ConstManPtr<T>& copy ) : ConstPtrBase() {
			this->_ptr = const_cast<void*>((const void*)copy.ptr());
		}

		~ConstManPtr()	{ ::free( this->_ptr ); }


		template <class A = T>
		void alloc()	{
			::free( this->_ptr );
			this->_ptr = (A*)::malloc( sizeof(A) );
			if ( this->_ptr == 0 )
				throw AllocException();

			this->size = sizeof(A);
			new (this->_ptr) A();
		}

		const T* ptr() const	{ return (T*)this->_ptr; }


		ConstManPtr<T>& operator=( const ConstManPtr<T>& other ) {
			if ( (const ConstManPtr<T>*)this == &other )	return *this;

			void* new_ptr = ::malloc( this->size );
			if ( new_ptr == 0 )	throw AllocException();
			
			::memcpy( new_ptr, (const void*)other.ptr(), this->size );

			::free( this->_ptr );
			this->_ptr = new_ptr;
			this->size = other.size;

			return *this;
		}

		operator T&() const	{ return *this->_ptr; }
		operator const T*() const	{ return this->_ptr; }
	};

	template <class T>
	class ManPtr : public ConstManPtr<T>, public virtual Ptr<T> {
	public:
		ManPtr() : ConstPtrBase(0) {}

		ManPtr( const ManPtr<T>& copy ) : ConstPtrBase(), ConstManPtr<T>( copy ) {}
		ManPtr( const T& value ) : ConstManPtr<T>( value ) {}

		ManPtr<T>& operator=( const ConstManPtr<T>& other ) {
			return *((ConstManPtr<T>*)this) = other;
		}

		T* ptr()	{ return (T*)this->_ptr; }
		operator T*()	{ return (T*)this->_ptr; }
	};
}

#endif//_CHI_PTR_H
