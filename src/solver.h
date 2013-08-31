///
/// @Author: Pavel Grim, pavelgrim (at) gmail (dot) com
/// @Project: Sudoku solver
/// @File: solver.h
/// @Created: 6. 4. 2007
///
#ifndef SOLVER_H
#define SOLVER_H

#include <ostream>
#include "type.h"
#include "grid.h"

/// Class with logic for filling an empty grid spots.
class Solver
{
	/// Structure for the grid position.
	struct Coords
	{
		uint8 row; /// Row number in a grid.
		uint8 col; /// Column number in a grid.
	
		/// Constructor.
		Coords() { }

		/// Constructor.
		/// @param col Column.
		/// @param row Row.
		Coords(uint8 row, uint8 col) : row(row), col(col) { }
	};

	/// Private constructor.
	Solver() { }

public:
	/// Tries to solve sudoku.
	/// @param grid Grid.
	/// @param output Output stream for writing solution.
	/// @param write_num Number of solutions to write.
	/// @param exp Number of numbers to be placed.
	/// @return True if solution found.
	static bool Solve(Grid & grid, std::ostream & output,
		int & write_num, uint8 exp
	);
	
private:
	/// Places number to the grid and tries to solve it.
	/// @param grid Grid.
	/// @param output Output stream for writing solution.
	/// @param write_num Number of solutions to write.
	/// @param exp Number of numbers to be placed.
	/// @return True if solution found.
	static bool SolveByExperiment(Grid & grid, std::ostream & output, int & write_num, uint8 exp);
	
	/// Tries to solve sudoku by testing horizontal and vertical lines.
	/// @param grid Grid.
	/// @return Number of changes.
	static uint8 SolveByLines(Grid & grid);
	
	/// Tries to solve sudoku by testing blocks in the grid.
	/// @param grid Grid.
	/// @return Number of changes.
	static uint8 SolveByBlocks(Grid & grid);

	/// Tries to solve sudoku by testing individual fields.
	/// @param grid Grid.
	/// @return Number of changes.
	static uint8 SolveByFields(Grid & grid);

	/// Checks if number can be placed to the grid.
	/// @param grid Grid to test placement into.
	/// @param row Row coordinate to place.
	/// @param col Column coordinate to place.
	/// @param number Number to place.
	/// @return True if number can be placed.
	static bool CanPutNumber(const Grid & grid,
		uint8 row, uint8 col, uint8 number
	);

public:
	/// Checks if stored grid is valid.
	/// @param grid Grid to check.
	/// @return True if valid.
	static bool IsValidGrid(const Grid & grid);

private:
	/// Gets origin coordinates of the block from specified block index.
	/// @param index Block index in a grid.
	/// @return Origin coordinates.
	/// @note
	/// 0 [0,0]		1 [0,3]		2 [0,6]
	/// 3 [3,0]		4 [3,3]		5 [3,6]
	/// 6 [6,0]		7 [6,3]		8 [6,6]
	static Coords GetBlockStart(uint8 index);

	/// Gets origin coordinates of the block from specified row and column.
	/// @param row Row in a grid.
	/// @param col Column in a grid.
	/// @return Origin coordinates.
	/// @note
	/// 0 [0,0]		1 [0,3]		2 [0,6]
	/// 3 [3,0]		4 [3,3]		5 [3,6]
	/// 6 [6,0]		7 [6,3]		8 [6,6]
	static Coords GetBlockStart(uint8 row, uint8 col);
};

#endif
/// EOF
