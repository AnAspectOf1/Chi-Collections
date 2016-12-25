#ifndef _CHI_MAP_H
#define _CHI_MAP_H

#include "allocator.h"
#include "array.h"
#include "linked.h"
#include "string.h"


namespace qi {

	template <class V, class K = String<StdAllocator<char>>>
	struct MapEntry {
		K key;
		V value;

		MapEntry() {}
		MapEntry( const K& key, const V& value ) : key(key), value(value) {}
	};

	template <class V, class K = String<
StdAllocator<char>>>
	class Map : public virtual List<MapEntry<V,K>> {
	public:
		typedef K KeyType;
		typedef V ValueType;

		void add( const K& key, const V& value ) {
			CHI_ASSERT( this->findKey( key ) != (Size)-1, "Key already exists in map" );

			this->append( MapEntry<V,K>( key, value ) );
		}



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

		V* operator[]( const K& key ) {
			Size foundAt = this->findKey( key );
			if ( foundAt == (Size)-1 )
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
	class ArrayMap : public Array<MapEntry<V,K>, Alloc>, public virtual Map<V,K> {
	public:
		ArrayMap( Size count = 0 ) : Array<MapEntry<V,K>,Alloc>( count ) {}
		ArrayMap( const ArrayMap<V,K,Alloc>& other ) : Array<MapEntry<V,K>,Alloc>( other ) {}
		ArrayMap( const List<MapEntry<V,K>>& other ) : Array<MapEntry<V,K>,Alloc>( other ) {}
		ArrayMap( const Collection<MapEntry<V,K>>& other ) : Array<MapEntry<V,K>,Alloc>( other ) {}
	};

	template <class V, class K = String<>>
	class LinkedMap : public LinkedList<MapEntry<V,K>>, public virtual Map<V,K> {
	public:
		LinkedMap() : LinkedList<MapEntry<V,K>>() {}
		LinkedMap( const LinkedMap<V,K>& copy ) : LinkedList<MapEntry<V,K>>( copy ) {}
		LinkedMap( const List<MapEntry<V,K>>& other ) : LinkedList<MapEntry<V,K>>( other ) {}
		LinkedMap( const Collection<MapEntry<V,K>>& other ) : LinkedList<MapEntry<V,K>>( other ) {}
	};
}

#endif//_CHI_MAP_H
