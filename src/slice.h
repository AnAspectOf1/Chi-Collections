#ifndef _CHI_SLICE_H
#define _CHI_SLICE_H


namespace qi {

	template <class T>
	class List;

	template <class T>
	class Slice : public virtual List<T> {
		friend List<T>;

	protected:
		List<T>*const list;
		const Size _first;
		const Size _count;

		Slice( List<T>* list, Size count, Size first = 0 ) : list(list), _first(first), _count(count) {}

	public:
		T& _at( Size index ) const override	{ return this->list->_at( this->_first + index ); }

		Size count() const override	{ return this->_count; }
	};
}

#endif//_CHI_SLICE_H
