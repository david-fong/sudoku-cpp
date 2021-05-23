#ifndef HPP_SOLVENT_LIB_CANON
#define HPP_SOLVENT_LIB_CANON

#include "../size.hpp"
#include "../grid.hpp"

#include <array>

namespace solvent::lib::canon {

	template<Order O>
	class Canonicalizer final : public solvent::lib::Grid<O> {
	 public:
		using occmask_t = typename size<O>::occmask_t;
		using ord1_t  = typename size<O>::ord1_t;
		using ord2_t  = typename size<O>::ord2_t;
		using ord4_t  = typename size<O>::ord4_t;
		using value_t   = typename size<O>::value_t;

		static constexpr ord1_t O1 = O;
		static constexpr ord2_t O2 = O*O;
		static constexpr ord4_t O4 = O*O*O*O;

	 private:
		const std::array<value_t, O4> buf;

		void handle_relabeling(void);
	};
}

#endif