#ifndef _CHI_MAP_H
#define _CHI_MAP_H

#include "array.h"
#include "string.h"


namespace chi {

	template <class V, class K>
	struct MapEntry {
		K key;
		V value;

		MapEntry( const K& key, const V& value ) : key(key), value(value) {}
	};

	template <class V, class K = String<>, class KB = K>
	class MapBase : public virtual ArrayBase<MapEntry<V,K>> {
	public:
		void add( const K& key, const V& value ) {
			this->append( MapEntry<V,K>( key, value ) );
		}

		Size findKey( const KB& key ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i).key == key )
					return i;
			}

			return -1;
		}

		Size findValue( const V& value ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i).value == value )
					return i;
			}

			return -1;
		}

		V* operator[]( const KB& key ) {
			Size foundAt = this->findKey( key );
			if ( foundAt == -1 )
				return 0;

			return &this->at( foundAt ).value;
		}
		const V* operator[]( const KB& key ) const {
			Size foundAt = this->findKey( key );
			if ( foundAt == -1 )
				return 0;

			return &this->at( foundAt ).value;
		}
	};

	template <class V, class K = String<>, class KB = K, class Alloc = StdAllocator<MapEntry<V,K> >
	class Map : public Array<MapEntry<V,K>>, public virtual MapBase<V,K,KB> {
	public:
		Map( Size count = 0 ) : Array<MapEntry<V,K>>( count ) {}
		Map( const Map<V,K,KB,Alloc>& other ) : Array<MapEntry<V,K>>( other ) {}
	};
}

#endif//_CHI_MAP_H
