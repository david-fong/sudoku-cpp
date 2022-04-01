#include <ookiidoku_cli/repl.hpp>
#include <ookiidoku/rng.hpp>

#include <ookiidoku_util/console_setup.hpp>

#include <string>
#include <iostream>  // cout,
#include <random>    // random_device,

/**
ARGUMENTS
1: grid order (default defined in ookiidoku_config).
2: scramble random key (default: device random number).
*/
int main(const int argc, char const *const argv[]) {
	ookiidoku::util::setup_console();

	unsigned int user_order; // 1
	std::uint_fast64_t srand_key; // 2

	user_order = (argc > 1) ? std::stoi(argv[1]) : M_OOKIIDOKU_DEFAULT_ORDER;
	if (argc > 2 && !std::string(argv[2]).empty()) {
		srand_key = std::stoi(argv[2]);
	} else {
		srand_key = std::random_device()();
	}

	std::cout << "\nPARSED ARGUMENTS:"
	<< "\n- ARG 1 [[ grid order ]] : " << user_order
	<< "\n- ARG 2 [[ srand key  ]] : " << srand_key
	<< std::endl;

	// Scramble the random number generators:
	ookiidoku::seed_rng(srand_key);

	ookiidoku::cli::Repl repl {user_order};
	repl.start();

	// End of program:
	std::cout << "\nbye bye!\n" << std::endl;
	// std::cout << "\ntotal: " << ookiidoku::gen::::total;
	// std::cout << "\ntrue_: " << ookiidoku::gen::::true_ << std::endl;
	return 0;
}