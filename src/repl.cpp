#include "repl.hpp"

#include <iostream>     // cout, endl
#include <iomanip>      // setw,


namespace Sudoku {

// Mechanism to statically toggle printing alignment:
// (#undef-ed before the end of this namespace)
#define STATW_I << std::setw(this->solver.STATS_WIDTH)
#define STATW_D << std::setw(this->solver.STATS_WIDTH + 4)


template <Order O, bool CBT, GiveupMethod GUM>
Repl<O,CBT,GUM>::Repl(std::ostream& os):
    solver  (os),
    os      (os)
{
    // Print help menu and then start the REPL (read-execute-print-loop):
    std::cout << HELP_MESSAGE << std::endl;
    std::string command;
    do {
        std::cout << REPL_PROMPT;
        std::getline(std::cin, command);
    } while (runCommand(command));
};


template <Order O, bool CBT, GiveupMethod GUM>
bool Repl<O,CBT,GUM>::runCommand(std::string const& cmdLine) {
    size_t tokenPos;
    // Very simple parsing: Assumes no leading spaces, and does not
    // trim leading or trailing spaces from the arguments substring.
    const std::string cmdName = cmdLine.substr(0, tokenPos = cmdLine.find(" "));
    const std::string cmdArgs = cmdLine.substr(tokenPos + 1, std::string::npos);
    const auto it = COMMAND_MAP.find(cmdName);
    if (it == COMMAND_MAP.end()) {
        // No command name was matched
        std::cout << "command \"" << cmdLine << "\" not found. enter \"help\" for the help menu." << std::endl;
        return true;
    }
    switch (it->second) {
        case CMD_HELP:
            std::cout << HELP_MESSAGE << std::endl;
            break;
        case CMD_QUIT:
            return false;
        case CMD_SOLVE: {
            if (solver.loadPuzzleFromString(cmdArgs)) {
                // TODO: give better output if solver gives up. Maybe move to its own function.
                SolverExitStatus exitStatus;
                solver.template generateSolution<true>(exitStatus);
                solver.print();
                break;
            }
            std::ifstream puzzleFile(cmdArgs);
            if (!puzzleFile.good()) {
                std::cout << "the specified file could not be opened for reading." << std::endl;
            } else {
                solvePuzzlesFromFile(puzzleFile);
            }
            break; }
        case CMD_RUN_SINGLE:
            runSingle();
            break;
        case CMD_CONTINUE_PREV:
            runSingle(true);
            break;
        case CMD_RUN_MULTIPLE:
            try {
                runMultiple(std::stoul(cmdArgs));
            } catch (std::invalid_argument const& ia) {
                std::cout << "could not convert " << cmdArgs << " to an integer." << std::endl;
            }
            break;
        case CMD_SET_GENPATH:
            solver.setGenPath(static_cast<GenPath>((solver.getGenPath() + 1) % (GenPath_MAX + 1)));
            std::cout << "generator path is now set to: "
                << GenPath_Names[solver.getGenPath()] << std::endl;
            break;
    }
    return true;
}


template <Order O, bool CBT, GiveupMethod GUM>
void Repl<O,CBT,GUM>::solvePuzzlesFromFile(std::ifstream& puzzlesFile) {
    // Put the string outside the loop since the space allocation
    // for proper input, should be all the same.
    std::string puzzleString;
    puzzleString.reserve(solver.area + 1);
    while (std::getline(puzzlesFile, puzzleString)) {
        // Read a puzzle from the file:
        if (!solver.loadPuzzleFromString(puzzleString)) {
            std::cout << "Could not use the input to read in a puzzle." << std::endl;
            // Just go try something else:
            continue;
        }
        SolverExitStatus exitStatus;
        solver.template generateSolution<true>(exitStatus);

        // TODO Write the solution to an output file.
        solver.print();
    }
}


template <Order O, bool CBT, GiveupMethod GUM>
void Repl<O,CBT,GUM>::runSingle(const bool contPrev) {
    solver.printMessageBar("START " + std::to_string(solver.getTotalGenCount()));

    // Generate a new solution:
    SolverExitStatus exitStatus;
    const clock_t    clockStart = std::clock();
    const opcount_t numSolveOps = solver.generateSolution(exitStatus, contPrev);
    const clock_t   clockFinish = std::clock();
    const double  processorTime = ((double)(clockFinish - clockStart)) / CLOCKS_PER_SEC;

    os << "num operations: " STATW_I << numSolveOps << '\n';
    os << "processor time: " STATW_D << processorTime << " seconds" << '\n';
    if (!solver.isPretty) solver.printMessageBar("", '-');
    solver.print();
    solver.printMessageBar((numSolveOps == 0) ? "ABORT" : "DONE");
}


template <Order O, bool CBT, GiveupMethod GUM>
void Repl<O,CBT,GUM>::runMultiple(const trials_t trialsToRun) {
    constexpr unsigned DEFAULT_COLS     = ((unsigned[]){0,64,32,24,16,4,1})[solver.order];
    constexpr unsigned LINES_PER_FLUSH  = ((unsigned[]){0, 0, 0, 0, 0,1,1})[solver.order];
    const unsigned COLS = (solver.isPretty ? (GET_TERM_COLS(DEFAULT_COLS)-7) : DEFAULT_COLS) / solver.STATS_WIDTH;
    const unsigned BAR_WIDTH  = solver.STATS_WIDTH * COLS + (solver.isPretty ? 7 : 0);

    // The last bin is for trials that do not succeed.
    std::array<trials_t, TRIALS_NUM_BINS+1> binHitCounts = {0,};
    std::array<double,   TRIALS_NUM_BINS+1> binGcvTotals = {0,};

    solver.printMessageBar("START x" + std::to_string(trialsToRun), BAR_WIDTH);
    clock_t clockStart = std::clock();

    for (trials_t numTotalTrials = 0; numTotalTrials < trialsToRun;) {
        if (numTotalTrials % COLS == 0) {
            const unsigned pctDone = 100.0 * numTotalTrials / trialsToRun;
            std::cout << "| " << std::setw(2) << pctDone << "% |";
        }
        SolverExitStatus exitStatus;
        const opcount_t numOperations = solver.generateSolution(exitStatus);
        numTotalTrials++;

        const opcount_t giveupCondVar
            = (GUM == OPERATIONS) ? numOperations
            : (GUM == BACKTRACKS) ? solver.getMaxBacktrackCount()
            : ~0;
        const unsigned binNum = TRIALS_NUM_BINS * (giveupCondVar) / solver.GIVEUP_THRESHOLD;
        binHitCounts[binNum]++;
        binGcvTotals[binNum] += giveupCondVar;

        if (exitStatus != SUCCESS) {
            // TODO: pretty print this as the number with color?
            os STATW_I << "---";
        } else {
            os STATW_I << numOperations;
        }
        if (numTotalTrials % COLS == 0) {
            if constexpr (LINES_PER_FLUSH) {
            if (solver.isPretty && (numTotalTrials % (LINES_PER_FLUSH * COLS) == 0)) {
                // Runs are slower. Flush buffer more frequently.
                os << std::endl;
            } else { os << '\n'; }
            } else { os << '\n'; }
        }
    }
    if (trialsToRun % COLS != 0) { os << '\n'; } // Last newline.

    // Print stats:
    const double processorSeconds = ((double)(std::clock() - clockStart) / CLOCKS_PER_SEC);
    solver.printMessageBar("", BAR_WIDTH, '-');
    os << "trials aborted: " STATW_I << binHitCounts[TRIALS_NUM_BINS] << '\n';
    os << "processor time: " STATW_D << processorSeconds << " seconds (including I/O)" << '\n';
    if (processorSeconds > 10.0) {
        // Emit a beep sound if the trials took longer than ten processor seconds:
        std::cout << '\a' << std::flush;
    }

    // Print bins (work distribution):
    solver.printMessageBar("", BAR_WIDTH, '-');
    printTrialsWorkDistribution(trialsToRun, binHitCounts, binGcvTotals);
    solver.printMessageBar("DONE x" + std::to_string(trialsToRun), BAR_WIDTH);
    os << std::flush;
}


template <Order O, bool CBT, GiveupMethod GUM>
void Repl<O,CBT,GUM>::printTrialsWorkDistribution(
    const trials_t trialsToRun,
    std::array<trials_t, TRIALS_NUM_BINS+1> const& opsBinHitCounts,
    std::array<double,   TRIALS_NUM_BINS+1> const& opsBinValTotals
) {
    // TODO: do a preliminary loop that initializes accumulator arrays.
    for (unsigned int i = 0; i < opsBinHitCounts.size(); i++) {
        ;
    }

    os << "|  bin bot  |   hits   |  throughput  |\n";
    os << "+-----------+----------+--------------+";
    opcount_t successfulTrialsAccum = 0;
    double  successfulSolveOpsAccum = 0.0;
    for (unsigned int i = 0; i < opsBinHitCounts.size(); i++) {
        successfulTrialsAccum   += opsBinHitCounts[i];
        successfulSolveOpsAccum += opsBinValTotals[i];
        const double binBottom  = (double)(i) * (double)solver.GIVEUP_THRESHOLD / TRIALS_NUM_BINS;
        const double throughput = (i == TRIALS_NUM_BINS) ? 0.0 : successfulTrialsAccum /
            (successfulSolveOpsAccum + ((trialsToRun - successfulTrialsAccum) * binBottom));
        if constexpr (solver.order < 4) {
            os << "\n|" << std::setw(9) << (int)(binBottom);
        } else {
            os << "\n|" << std::setw(8) << (int)(binBottom / 1'000.0) << 'K';
        }
        os << "  |" << std::setw(8)  << opsBinHitCounts[i];
        os << "  |" << std::setw(12);
        if (i == TRIALS_NUM_BINS) { os << "unknown";
        } else { os << std::scientific << throughput << std::fixed; }
        os << "  |";
    }
    os << " <- current threshold (giveups)\n";
    os << "+-----------+----------+--------------+\n";
    os << " * Throughput here is \"average successes per operation\". Tightening the"
        "\n   threshold induces more giveups, but also reduces the operational cost"
        "\n   giveups incur. Mathematically speaking, operations are proportional"
        "\n   to time, except operations are machine independent unlike time.\n";
}

#undef STATW_I
#undef STATW_D

} // End of Sudoku namespace.
