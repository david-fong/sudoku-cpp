#include <okiidoku/db/serdes.hpp>

#include <okiidoku_cli_utils/console_setup.hpp>

#include <string>
#include <iostream>  // cout,

/**
ARGUMENTS
1: grid order (default defined in compiled_orders).
2: scramble random key (default: device random number).
*/
int main([[maybe_unused]] const int argc, [[maybe_unused]] char const *const argv[]) {
	okiidoku::util::setup_console();

	// unsigned int user_order; // 1
	// user_order = (argc > 1) ? std::stoi(argv[1]) : OKIIDOKU_DEFAULT_ORDER;

	// std::cout << "\nPARSED ARGUMENTS:"
	// << "\n- ARG 1 [[ grid order ]] : " << user_order
	// << std::endl;

	// TODO.high

	return 0;
}
