#ifndef _CHI_LIST_H
#define _CHI_LIST_H

#include "collection.h"


namespace chi {

	template <class T>
	class List : public virtual Collection {
	public:
		virtual T& at( Size index ) = 0;
		virtual const T& at( Size index ) const = 0;

		Size size() const	{ return this->count() * sizeof(T); }

		T& operator[]( Size index )	{ return this->at( index ); }
		const T& operator[]( Size index ) const	{ return this->at( index ); }
	};
}

#endif//_CHI_LIST_H
