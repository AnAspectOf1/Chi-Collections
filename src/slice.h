#ifndef _CHI_SLICE_H
#define _CHI_SLICE_H


namespace chi {

	template <class T>
	class List;

	template <class T>
	class Slice : public virtual List<T> {
		friend List<T>;

	protected:
		List<T>* list;
		Size _first;
		Size _count;

		Slice( List<T>* list, Size count, Size first = 0 ) : list(list), _first(first), _count(count) {}

	public:
		T& at( Size index )	{ return this->list->at( this->_first + index ); }
		const T& at( Size index ) const	{ return this->list->at( this->_first + index ); }

		Size count() const	{ return this->_count; }
	};
}

#endif//_CHI_SLICE_H
