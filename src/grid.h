///
/// @Author: Pavel Grim, pavelgrim (at) gmail (dot) com
/// @Project: Sudoku solver
/// @File: grid.h
/// @Created: 6. 4. 2007
///
#ifndef GRID_H
#define GRID_H

#include "type.h"
#include <istream>
#include <ostream>

/// Holds the sudoku grid.
class Grid
{
public:
	/// Size of the one side of a block in the grid.
	static const uint8 size_block = 3;
	/// Size of the one side of the grid.
	static const uint8 size_grid = size_block * size_block;

private:
	uint8 grid[size_grid][size_grid]; /// Sudoku grid.
	uint8 zeroes;                     /// Number of zeros in the grid.
	
public:
	/// Constructor.
	Grid();
	
private:
	/// Resets the grid.
	void Clear();

public:
	/// Loads a grid from the input.
	/// @param input Input to read from.
	/// @return True if loaded successfully.
	bool LoadGrid(std::istream & input);

	/// Prints the grid.
	/// @param output Output stream to print to.
	void Print(std::ostream & output) const;

	/// Gets if the grid is solved.
	/// @return True if solved.
	inline bool Solved() { return this->zeroes == 0; }

	/// Sets the number to the grid specified by a row and a column.
	/// @param row Row of the grid.
	/// @param col Column of the grid.
	/// @param number Number to set.
	inline void SetNumber(uint8 row, uint8 col, uint8 number)
	{
		// Update number of zeros.
		if (this->grid[row][col] == 0) {
			if (number != 0) {
				this->zeroes--;
			}
		} else {
			if (number == 0) {
				this->zeroes++;
			}
		}

		this->grid[row][col] = number;
		return;
	}

	/// Gets a number from the grid specified by a row and a column.
	/// @param col Column of the grid.
	/// @return Number in those coordinates.
	inline uint8 GetNumber(uint8 row, uint8 col) const
	{
		return this->grid[row][col];
	}
};

#endif
/// EOF
