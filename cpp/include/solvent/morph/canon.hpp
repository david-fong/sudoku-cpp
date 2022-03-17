#ifndef HPP_SOLVENT__MORPH__CANON
#define HPP_SOLVENT__MORPH__CANON

#include "solvent/grid.hpp"
#include "solvent/size.hpp"
#include "solvent_config.hpp"
#include "solvent_export.h"

namespace solvent::morph {

	// contract: the span is a _complete_, valid grid.
	template<Order O>
	SOLVENT_EXPORT void canonicalize(grid_span_t<O>);

	// contract: T fits size<O>::ord2i_t and canonicalize<O> is compiled. also see canonicalize<O>'s contract.
	template<class T>
	requires std::is_integral_v<T>
	SOLVENT_EXPORT void canonicalize(Order O, std::span<T>);
}
#endif