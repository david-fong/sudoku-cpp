#ifndef HPP_SOLVENT_LIB_GRID
#define HPP_SOLVENT_LIB_GRID

#include "../util/ansi.hpp"
#include "./size.hpp"

namespace solvent::lib {
	/**
	 */
	template<Order O>
	class Grid {
	 static_assert((1 < O) && (O <= MAX_REASONABLE_ORDER));
	 public:
		using occmask_t = typename size<O>::occmask_t;
		using ord1_t  = typename size<O>::ord1_t;
		using ord2_t  = typename size<O>::ord2_t;
		using ord4_t  = typename size<O>::ord4_t;
		using value_t = typename size<O>::value_t;

		static constexpr ord1_t O1 = O;
		static constexpr ord2_t O2 = O*O;
		static constexpr ord4_t O4 = O*O*O*O;

	// ========================
	 public:
		// Inline functions:
		[[gnu::const]] static constexpr ord2_t get_row(const ord4_t index) noexcept { return index / O2; }
		[[gnu::const]] static constexpr ord2_t get_col(const ord4_t index) noexcept { return index % O2; }
		[[gnu::const]] static constexpr ord2_t get_blk(const ord4_t index) noexcept { return get_blk(get_row(index), get_col(index)); }
		[[gnu::const]] static constexpr ord2_t get_blk(const ord2_t row, const ord2_t col) noexcept {
			return ((row / O1) * O1) + (col / O1);
		}
	};
}

#endif