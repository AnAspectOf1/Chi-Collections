#ifndef _CHI_OP_H
#define _CHI_OP_H

#include <type_traits>


namespace chi {
namespace op {

	template <class T, class O> \
	constexpr auto _equalType() -> decltype( T() == O() ) { \
		return T() == O(); \
	}

	template <class T, class O> \
	constexpr bool implementsEqual() { \
		return std::is_same<bool, decltype( _equalType<T, O>() )>::value; \
	}
} }

#endif//_CHI_OP_H
