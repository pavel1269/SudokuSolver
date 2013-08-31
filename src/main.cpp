///
/// @Author: Pavel Grim, pavelgrim (at) gmail (dot) com
/// @Project: Sudoku solver
/// @File: main.cpp
/// @Created: 6. 4. 2007
///
#include <iostream>
#include <sstream>
#include <string>
#include "type.h"
#include "grid.h"
#include "solver.h"

/// Default maximum number of solutions to output.
static int _default_write_num = 10;
/// Default number of numbers which will be tried to fill in after using
/// all algorithms but still unable to find any solution.
static uint8 _default_experiments = 2;

/// Prints help for this program.
static void PrintHelp()
{
	std::cout << "Sudoku solver - Solves a sudoku." << std::endl;
	std::cout << "Enter 81 numbers as the sudoku."
		" Represent empty fields with 0." << std::endl;

	std::cout << "SudokuSolver";
	std::cout << " [write number = " << (int)_default_write_num;
	std::cout << " [experiment = " << (int)_default_experiments;
	std::cout << "]]" << std::endl << std::endl;

	std::cout << " - write number     "
		"Maximum number of solutions to output."
		" Number below 0 for all." << std::endl;
	std::cout << " - experiment       "
		"Number of numbers which will be tried to fill in after using"
		" all algorithms but still unable to find any solution." << std::endl;

	std::cout << std::endl;
	return;
}

/// Converts string to number.
/// @tparam T Number type.
/// @param str String to convert.
/// @return Converted number.
template<class T>
static inline T ConvertToNumber(const char * str)
{
	std::istringstream stream(str);
	T number;
	stream >> number;
	return number;
}

/// Main function.
/// @param argc Argument count.
/// @param argv Arguments.
/// @return Program exit code.
int main(int argc, char ** argv)
{
	// Print help.
	if (argc == 2 && 
		(std::strcmp(argv[1], "-h") == 0
		|| std::strcmp(argv[1], "--help") == 0))
	{
		PrintHelp();
		return 0;
	}
	
	// Load grid.
	Grid grid;
	if (!grid.LoadGrid(std::cin)) {
		std::cerr << "Could not load entering." << std::endl;
		return 0;
	}
	if (!Solver::IsValidGrid(grid)) {
		std::cerr << "Input sudoku is not valid." << std::endl;
		return 0;
	}

	// Try to solve the sudoku.
	int write_number = _default_write_num;
	uint8 experiments = _default_experiments;

	if (argc > 1) {
		write_number = ConvertToNumber<int>(argv[1]);
	}
	if (argc > 2) {
		experiments = ConvertToNumber<uint8>(argv[2]);
	}

	bool solved = Solver::Solve(grid, std::cout, write_number, experiments);
	if (!solved) {
		std::cerr << "Could not solve the sudoku." << std::endl;
	}

	return 0;
}

/// EOF
