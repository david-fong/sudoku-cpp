// SPDX-FileCopyrightText: 2020 David Fong
// SPDX-License-Identifier: AGPL-3.0-or-later
#ifndef HPP_OKIIDOKU__PRINT_2D__EMOJI
#define HPP_OKIIDOKU__PRINT_2D__EMOJI

#include <okiidoku/detail/export.h>

#include <vector>
#include <array>
#include <string_view>

namespace okiidoku::emoji {

	struct Set final {
		std::string_view name;
		std::vector<std::string_view> entries;
	};

	// TODO.mid constexpr all the things with std::tuple, std::to_array, etc.

	inline constexpr auto top_set_preferences {std::to_array<size_t>({5, 11})};

	// TODO.low annoyingly, when printing on various terminal emulators, some
	// emojis vary in width. I've added spaces where required on gnome terminal,
	// but windows terminal is different.
	inline const std::vector<Set> sets {
		{"fruit",       {"🍉","🍄","🍓","🍅","🌶 ","🍎","🍑","🍊","🥕","🥭","🍍","🍋","🍌","🌽","🥑","🍐","🥝","🍏","🥬","🍈","🫐","🧄","🍆","🍇","🌰"}}, // yeah yeah. vegetables. I know.
		{"reptile",     {"🐸","🦎","🐍","🐢","🐊","🐉","🐲","🦕","🦖"}},
		{"marine",      {"🦦","🦭","🦈","🐬","🐋","🐟","🐠","🦑","🐙","🦀","🦞","🐚"}},
		{"sweets",      {"🍪","🍫","🍬","🍭","🍰","🍮","🍡","🥠","🍦","🍯","🍩"}},
		{"vehicle",     {"🚗","🚑","🚒","🚓","🚈","🚠","🛒","🚀","🛸"}},
		{"plant",       {"🌱","🌲","🌳","🌴","🌵","🌹","🌷","🌸","🌺","🏵 ","🌼","🍀","🍁","🍂","🍃"}},
		{"animal_body", {"🐁","🦔","🐇","🦨","🐈","🐕","🐖","🐄","🐑","🐐","🐎","🐒","🦍","🦥","🦒","🦏","🐘","🐫"}},
		{"animal_head", {"🐰","🐱","🐶","🐔","🐴","🐮","🐷","🦝","🐺","🦁","🐯","🐵","🐻","🐼","🐨","🦄","🦓"}},
		{"bird",        {"🐣","🐤","🐦","🕊 ","🦩","🦅","🦆","🦉","🦚","🦜","🐧","🦢","🐓"}},
		{"insect",      {"🐜","🕷 ","🪰","🐞","🦋","🐝","🐌","🦗"}},
		{"food",        {"🧀","🥨","🥐","🍞","🥪","🍔","🍟","🍕","🌮","🥗","🍚","🍜","🍥","🍤","🍣","🍙","🍘"}},
		{"drink",       {"🫖","🍺","☕","🍵","🧋","🧃","🧂","🧊"}},
		{"body",        {"🧠","👀","👁 ","👂","👃","🦷","👄","💋","👅","✋","👆","👇","👈","👉","👋","👌","👍","👎"}},
		{"clothing",    {"👑","🎀","🧢","👒","🎓","🎩","👓","🤿","💄","🧣","👕","👚","👗","👘","🧤","👛","👜","🎒","🧦","👟","👠","👢"}},
		{"heart",       {"💖️","❤️ ","🧡","💛","💚","💙","💜","🖤","💞","🫀"}},
		{"cartoon",     {"💤","💢","💎","💣","💀","👾","💥","💦","💨","💬","💭","💩"}},
		{"smiley",      {"😀","😍","😎","😛","😐","😗","😮","🤯","😡","😨","🙃"}},
		{"sport",       {"⚽","⚾","🏐","🏀","🏈","🥊","🎾","🏓","🥌","🎳","🎱","🎯","🀄","🃏","🎲","🧵","🧶","🎮","🔫","🏆"}},
		{"occasion",    {"🎂","🎁","🎃","🎄","🎈","🎉","🎊","🎏"}},
		{"element",     {"✨","⭐","🌈","💧","🔥","⚡","☃️","☔","⚓","⛺","⛽","🌊","🌋","🎨","🎪","🏯","🏰","💈","💺","🗻"}},
		{"sign",        {"🚨","🛑","⚠️","⛔","♻️","✅","❌","💯"}},
		{"household",   {"🛋","🪑","🚽","🪠","🧺","🧻","🪣","🧼","🧽","🧯"}},
		{"media",       {"🎬","🎵","🎷","🎺","🎻","🎸","🎹","🥁"}},
		{"office",      {"⏳","🩺","🩹","🧲","🧭","💰","💡","🔐","🔑","🔔","📢","📌","📎","📂","🗑","💾","💽","💻","📺","📷","📼","📨","📬","📦"}},
		{"circle",      {"🔮","🗿","🚬","🔴","🟠️","🟡️","🟢️","🔵","🟣️","🟤️","⚫️","⚪️"}},
	};
}
#endif