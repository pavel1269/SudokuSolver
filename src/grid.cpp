///
/// @Author: Pavel Grim, pavelgrim (at) gmail (dot) com
/// @Project: Sudoku solver
/// @File: grid.cpp
/// @Created: 6. 4. 2007
///
#include "grid.h"

Grid::Grid()
{
	this->Clear();
	return;
}

void Grid::Clear()
{
	this->zeroes = Grid::size_grid * Grid::size_grid;
	std::memset(this->grid, 0, sizeof(uint8) * this->zeroes);
	return;
}

/// Converts one digit number.
/// @param c One digit.
/// @return Converted number or -1 if not a digit.
static int8 ConvertDigitToNumber(char c)
{
	if (c < '0') return -1;
	if (c > '9') return -1;
	return c - '0';
}

bool Grid::LoadGrid(std::istream & input)
{
	uint8 num = 0;
	for (uint8 row = 0; row < Grid::size_grid; row++) {
		for (uint8 col = 0; col < Grid::size_grid; col++) {
			char c;
			while (input >> c) {
				int8 number = ConvertDigitToNumber(c);
				if (number == -1) continue;
				this->SetNumber(row, col, number);
				num++;
				break;
			}
		}
	}
	
	// Check if we read enough numbers.
	bool success = num == Grid::size_grid * Grid::size_grid;
	if (!success) {
		this->Clear();
	}

	return success;
}

void Grid::Print(std::ostream & output) const
{
	output << std::endl;
	for (uint8 row = 0; row < Grid::size_grid; row++) {
		// Insert more white spaces to distinguish between game board squares.
		if (row > 0 && (row % Grid::size_block) == 0) {
			output << std::endl;
		}
		for (uint8 col = 0; col < Grid::size_grid; col++) {
			// Another extra style white spaces.
			if (col > 0 && (col % Grid::size_block) == 0) {
				output << "  ";
			}

			output << (int)this->grid[row][col];

			if (col < Grid::size_grid - 1) {
				output << " ";
			}
		}
		output << std::endl;
	}
	output << std::endl;

	return;
}

/// EOF
