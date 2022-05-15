#ifndef HPP_OKIIDOKU__MORPH__TIES
#define HPP_OKIIDOKU__MORPH__TIES

#include <okiidoku/ints.hpp>
#include <okiidoku/detail/order_templates.hpp>

#include <range/v3/view/iota.hpp>

#include <algorithm>
#include <array>
#include <type_traits>
#include <cassert>

namespace okiidoku::mono {

	template<Order O, unsigned O1_OR_O2>
	requires (is_order_compiled(O) && ((O1_OR_O2 == 1) || (O1_OR_O2 == 2)))
	struct TieLinks final {
		using T = Ints<O>;
		static constexpr size_t size_ {(O1_OR_O2 == 1) ? T::O1 : T::O2};
		using link_t = std::conditional_t<(O1_OR_O2 == 1), typename T::o1i_t, typename T::o2i_smol_t>;
		using links_t = std::array<link_t, size_>;

		class Range final {
		public:
			link_t begin_;
			link_t end_;
			Range(const link_t begin, const link_t end) noexcept: begin_{begin}, end_{end} {}
			[[nodiscard]] link_t size() const noexcept { return end_ - begin_; }
			auto begin() const noexcept { return ranges::views::iota(begin_, end_).begin(); }
			auto end()   const noexcept { return ranges::views::iota(begin_, end_).end(); }
		};

		class Iterator final {
		public:
			using iterator_category = std::input_iterator_tag;
			using difference_type = std::ptrdiff_t;
			using value_type = Range;
			using reference = value_type&;
			using pointer = value_type*;
		private:
			const links_t& links_;
			link_t i_;
		public:
			Iterator(const links_t& links, link_t i = 0) noexcept: links_(links), i_{i} {}

			Range operator*()  const noexcept { return Range{i_, links_[i_]}; }
			Range operator->() const noexcept { return Range{i_, links_[i_]}; }
			Iterator& operator++() noexcept { i_ = links_[i_]; return *this; }
			Iterator operator++(int) noexcept { Iterator tmp = *this; ++(*this); return tmp; }
			[[nodiscard, gnu::const]] friend bool operator==(const Iterator& a, const Iterator& b) noexcept { return (&a.links_ == &b.links_) && (a.i_ == b.i_); }
			[[nodiscard, gnu::const]] friend bool operator!=(const Iterator& a, const Iterator& b) noexcept { return (&a.links_ != &b.links_) || (a.i_ != b.i_); }
		};
	private:
		links_t links_ {0};
	public:
		TieLinks() noexcept: links_{[]{ links_t _{0}; _[0] = size_; return _; }()} {}
		[[nodiscard, gnu::const]] friend bool operator==(const TieLinks&, const TieLinks&) noexcept = default;
		Iterator begin() const noexcept { return Iterator(links_); }
		Iterator end()   const noexcept { return Iterator(links_, size_); }

		[[nodiscard, gnu::pure]] bool has_unresolved() const noexcept { return std::ranges::any_of(links_, [](const auto& e){ return e == 0; }); }
		[[nodiscard, gnu::pure]] bool is_completely_unresolved() const noexcept { return links_[0] == size_; }

		template<class IsEq>
		// requires (std::regular_invocable<IsEq, link_t, link_t>)
		void update(const IsEq is_eq) {
			for (const auto tie : *this) {
				assert(tie.begin_ < tie.end_);
				auto cursor {tie.begin_};
				for (auto i {static_cast<link_t>(cursor+1)}; i < tie.end_; ++i) {
					assert(cursor < i);
					if (!std::invoke(is_eq, static_cast<link_t>(i-1), i)) [[likely]] {
						links_[cursor] = i;
						cursor = i;
				}	}
				links_[cursor] = tie.end_;
			}
		}
	};
}
#endif