#ifndef _CHI_PATH_H
#define _CHI_PATH_H

#include "allocator.h"
#include "int.h"
#include "string.h"


namespace chi {
	
	template <class Alloc = StdAllocator<char>>
	class Path : public String<Alloc> {
	public:
		using String<Alloc>::String;

		Path<Alloc> dir() const {

			Size i = this->length();
			do {
				if ( this->at(i) == '/' )
					break;				

				i--;
			}
			while ( i > 0 );

			return this->slice( i + 1 );
		}

		/*template <class T>
		operator String<T>() const {
			return String<T>( *this );
		}*/
	};
}

#endif//_CHI_PATH_H
