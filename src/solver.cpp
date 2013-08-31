///
/// @Author: Pavel Grim, pavelgrim (at) gmail (dot) com
/// @Project: Sudoku solver
/// @File: solver.cpp
/// @Created: 6. 4. 2007
///
#include "solver.h"

bool Solver::Solve(Grid & grid,
	std::ostream & output, int & write_num, uint8 exp)
{
	uint8 changes;
	do {
		changes = 0;
		changes += Solver::SolveByLines(grid);
		changes += Solver::SolveByBlocks(grid);
		changes += Solver::SolveByFields(grid);
	} while (changes != 0);

	if (grid.Solved()) {
		if (write_num != 0) {
			grid.Print(output);
			if (write_num > 0) write_num--;
		}
		return true;
	}

	if (exp == 0) return false;
	return Solver::SolveByExperiment(grid, output, write_num, exp);
}

bool Solver::SolveByExperiment(Grid & grid,
	std::ostream & output, int & write_num, uint8 exp)
{
	bool success = false;
	for (uint8 row = 0; row < Grid::size_grid; row++) {
		for (uint8 col = 0; col < Grid::size_grid; col++) {
			if (grid.GetNumber(row, col) != 0) continue;
			// Place number and try to solve sudoku now.
			for (uint8 number = 1; number <= Grid::size_grid; number++) {
				if (!Solver::CanPutNumber(grid, row, col, number)) {
					continue;
				}
				
				grid.SetNumber(row, col, number);
				bool res = Solver::Solve(grid, output, write_num, exp - 1);
				if (res) success = true;
			}
			// Remove number after the try.
			grid.SetNumber(row, col, 0);
		}
	}

	return success;
}

uint8 Solver::SolveByLines(Grid & grid)
{
	// Number of changes.
	uint8 changes = 0;

	for (uint8 number = 1; number <= Grid::size_grid; number++) {
		// Check rows and columns at the same time.
		// (1) Row check    - coord1 = row, coord2 = col
		// (2) Column check - coord1 = col, coord2 = row
		for (uint8 coord1 = 0; coord1 < Grid::size_grid; coord1++) {
			uint8 chances1 = 0;
			Coords coords1;
			uint8 chances2 = 0;
			Coords coords2;
			// For all (1)columns / (2)rows.
			for (uint8 coord2 = 0; coord2 < Grid::size_grid; coord2++) {
				// (1) Row check
				if (Solver::CanPutNumber(grid, coord1, coord2, number)) {
					// Free field and can put the number.
					chances1++;
					// Save this field position.
					coords1 = Coords(coord1, coord2);
				}
				// (2) Column check
				if (Solver::CanPutNumber(grid, coord2, coord1, number)) {
					// Free field and can put the number.
					chances2++;
					// Save this field position.
					coords2 = Coords(coord2, coord1);
				}
			} // for coord2

			// Check results.
			if (chances1 == 1) {
				grid.SetNumber(coords1.row, coords1.col, number);
				changes++;
			}
			if (chances2 == 1) {
				grid.SetNumber(coords2.row, coords2.col, number);
				changes++;
			}
		} // for coord1
	} // for number

	return changes;
}

uint8 Solver::SolveByBlocks(Grid & grid)
{
	// Number of changes.
	uint8 changes = 0;

	for (uint8 block = 0; block < Grid::size_grid; block++) {
		const Coords coord = Solver::GetBlockStart(block);
		for (uint8 number = 1; number <= Grid::size_grid; number++) {
			uint8 chances = 0;
			Coords field;
			// Go through whole block.
			for (uint8 row_off = 0; row_off < Grid::size_block; row_off++) {
				for (uint8 col_off = 0; col_off < Grid::size_block; col_off++) {
					const uint8 row = coord.row + row_off;
					const uint8 col = coord.col + col_off;
					if (Solver::CanPutNumber(grid, row, col, number)) {
						chances++;
						field = Coords(row, col);
					}
				}
			}

			// If a number can be put to the only spot.
			if (chances == 1) {
				grid.SetNumber(field.row, field.col, number);
				changes++;
			}
		}
	}
	return changes;
}

uint8 Solver::SolveByFields(Grid & grid)
{
	// Number of changes.
	uint8 changes = 0;

	for (uint8 row = 0; row < Grid::size_grid; row++) {
		for (uint8 col = 0; col < Grid::size_grid; col++) {
			if (grid.GetNumber(row, col) != 0) continue;
			uint8 chances = 0;
			uint8 last_chance;
			for (uint8 number = 1; number <= Grid::size_grid; number++) {
				if (Solver::CanPutNumber(grid, row, col, number)) {
					chances++;
					last_chance = number;
				}
			}

			if (chances == 1) {
				grid.SetNumber(row, col, last_chance);
			}
		}
	}
	return changes;
}

bool Solver::CanPutNumber(const Grid & grid,
	const uint8 row, const uint8 col, const uint8 number)
{
	// Space already occupied.
	if (grid.GetNumber(row, col) != 0) {
		return false;
	}
	
	// Check specified row for the number.
	for (uint8 col2 = 0; col2 < Grid::size_grid; col2++) {
		const uint8 number2 = grid.GetNumber(row, col2);
		if (number2 == number) return false;
	}

	// Check specified column for the number.
	for (uint8 row2 = 0; row2 < Grid::size_grid; row2++) {
		const uint8 number2 = grid.GetNumber(row2, col);
		if (number2 == number) return false;
	}

	// Check current block for the number.
	Coords coord = Solver::GetBlockStart(row, col);
	for (uint8 row_off = 0; row_off < Grid::size_block; row_off++) {
		for (uint8 col_off = 0; col_off < Grid::size_block; col_off++) {
			const uint8 row2 = coord.row + row_off;
			const uint8 col2 = coord.col + col_off;
			const uint8 number2 = grid.GetNumber(row2, col2);
			if (number2 == number) return false;
		}
	}

	return true;
}

bool Solver::IsValidGrid(const Grid & grid)
{
	// Check rows for duplicates.
	for (uint8 row = 0; row < Grid::size_grid; row++) {
		uint8 present[Grid::size_grid + 1];
		std::memset(present, 0, Grid::size_grid + 1);
		for (uint8 col = 0; col < Grid::size_grid; col++) {
			const uint8 number = grid.GetNumber(row, col);
			if (number == 0) continue;
			if (present[number] == 1) return false;
			present[number] = 1;
		}
	}

	// Check columns for duplicates.
	for (uint8 col = 0; col < Grid::size_grid; col++) {
		uint8 present[Grid::size_grid + 1];
		std::memset(present, 0, Grid::size_grid + 1);
		for (uint8 row = 0; row < Grid::size_grid; row++) {
			const uint8 number = grid.GetNumber(row, col);
			if (number == 0) continue;
			if (present[number] == 1) return false;
			present[number] = 1;
		}
	}

	// Check grid blocks for duplicates.
	for (uint8 block = 0; block < Grid::size_grid; block++) {
		Coords coord = Solver::GetBlockStart(block);
		uint8 present[Grid::size_grid + 1];
		std::memset(present, 0, Grid::size_grid + 1);
		for (uint8 row_off = 0; row_off < Grid::size_block; row_off++) {
			for (uint8 col_off = 0; col_off < Grid::size_block; col_off++) {
				const uint8 row = coord.row + row_off;
				const uint8 col = coord.col + col_off;
				const uint8 number = grid.GetNumber(row, col);
				if (number == 0) continue;
				if (present[number] == 1) return false;
				present[number] = 1;
			}
		}
	}

	return true;
}

Solver::Coords Solver::GetBlockStart(uint8 index)
{
	// 0 [0,0]		1 [0,3]		2 [0,6]
	// 3 [3,0]		4 [3,3]		5 [3,6]
	// 6 [6,0]		7 [6,3]		8 [6,6]
	Coords coords;
	coords.col = (index % Grid::size_block) * Grid::size_block;
	coords.row = (index / Grid::size_block) * Grid::size_block;
	return coords;
}

Solver::Coords Solver::GetBlockStart(uint8 row, uint8 col)
{
	// 0 [0,0]		1 [0,3]		2 [0,6]
	// 3 [3,0]		4 [3,3]		5 [3,6]
	// 6 [6,0]		7 [6,3]		8 [6,6]
	Coords coords;
	coords.col = (col / Grid::size_block) * Grid::size_block;
	coords.row = (row / Grid::size_block) * Grid::size_block;
	return coords;
}

/// EOF
