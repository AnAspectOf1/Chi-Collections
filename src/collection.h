#ifndef _CHI_COLLECTION_H
#define _CHI_COLLECTION_H

#include "int.h"


namespace chi {

	class Collection {
	public:
		virtual Size count() const = 0;
	};
}

#endif//_CHI_COLLECTION_H
