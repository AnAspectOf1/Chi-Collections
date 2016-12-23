#ifndef _CHI_MAP_H
#define _CHI_MAP_H

#include "allocator.h"
#include "array.h"
#include "string.h"


namespace chi {

	template <class V, class K = String<StdAllocator<char>>>
	struct MapEntry {
		K key;
		V value;

		MapEntry() {}
		MapEntry( const K& key, const V& value ) : key(key), value(value) {}
	};

	template <class V, class K = String<
StdAllocator<char>>>
	class MapBase : public virtual ArrayBase<MapEntry<V,K>> {
	public:
		typedef K KeyType;
		typedef V ValueType;

		void add( const K& key, const V& value ) {
			this->append( MapEntry<V,K>( key, value ) );
		}

		Size findKey( const K& key ) const {
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

		V* operator[]( const K& key ) {
			Size foundAt = this->findKey( key );
			if ( foundAt == -1 )
				return 0;

			return &this->at( foundAt ).value;
		}
		const V* operator[]( const K& key ) const {
			Size foundAt = this->findKey( key );
			if ( foundAt == (Size)-1 )
				return 0;

			return &this->at( foundAt ).value;
		}
	};

	template <class V, class K = String<>, class Alloc = StdAllocator<MapEntry<V,K>>>
	class Map : public Array<MapEntry<V,K>, Alloc>, public virtual MapBase<V,K> {
	public:
		Map( Size count = 0 ) : Array<MapEntry<V,K>>( count ) {}
		Map( const Map<V,K,Alloc>& other ) : Array<MapEntry<V,K>>( other ) {}

		template <class C>
		Size findKey( const C& key ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i).key == key )
					return i;
			}

			return -1;
		}
		
		template <class C>
		Size findValue( const C& value ) const {
			for ( Size i = 0; i < this->count(); i++ ) {
				if ( this->at(i).value == value )
					return i;
			}

			return -1;
		}
	};
}

#endif//_CHI_MAP_H
