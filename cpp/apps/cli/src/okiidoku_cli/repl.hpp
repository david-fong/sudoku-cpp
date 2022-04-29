#ifndef HPP_OKIIDOKU_CLI__REPL
#define HPP_OKIIDOKU_CLI__REPL

#include <okiidoku_cli/config.hpp>
#include <okiidoku_cli/enum.hpp>
#include <okiidoku/shared_rng.hpp>

#include <map>
#include <string_view>
#include <array>
#include <memory> // unique_ptr


namespace okiidoku::cli {

	namespace Command {
		enum class E {
			help,
			quit,
			config_order,
			config_print_level,
			config_auto_canonicalize,
			gen_single,
			gen_multiple,
		};
		const std::map<std::string_view, Command::E> enum_str_to_enum {
			{ "help",         E::help },
			{ "quit",         E::quit },
			{ "order",        E::config_order },
			{ "verbosity",    E::config_print_level },
			{ "canonicalize", E::config_auto_canonicalize },
			{ "",             E::gen_single },
			{ "gen",          E::gen_multiple },
		};
		constexpr std::string_view helpMessage {"\nCOMMAND MENU:"
		"\n- help                  print this help menu"
		"\n- quit                  cleanly exit this program"
		"\n"
		"\n- verbosity [<level>]   get/set verbosity level"
		"\n- order [<order>]       get/set order (sqrt of grid length)"
		"\n- canonicalize [<y/n>]  get/set canonicalization"
		"\n"
		"\n- {enter}               generate a single solution"
		"\n- gen <n>               generate <n> solutions"
		};
	}

	// Returns zero on error.
	inline unsigned get_terminal_num_cols() {
		char const*const env_var = std::getenv("COLUMNS");
		return (env_var != NULL) ? static_cast<unsigned>(std::stoul(env_var)) : 0u;
	}


	/** */
	class Repl final {
	public:
		explicit Repl(Order O, SharedRng& rng);
		void start();
		bool run_command(std::string_view cmd_line);

	private:
		SharedRng& shared_rng_;
		Config config_;

		void gen_single();

		void gen_multiple(unsigned long long stop_after);
		void gen_multiple(std::string_view);
	};
}
#endif