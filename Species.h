#ifndef SPECIES_H
#define SPECIES_H

#include <array>
#include <cstddef>
#include <ostream>
#include <utility>

// Species implements Conway's Game of Life, which is a cellular automaton.
// The game is a zero-player game, meaning that its evolution is determined
// by its initial state, requiring no further input.
// The game begins with an initial configuration of cells, which then can
// be observed evolving.
// The universe of the game is a two-dimensional grid (std::array, wraps around) of
// square cells, each of which is in one of two possible states, alive or dead.
// Every cell interacts with its neighbours, the amount of these neighbours
// determines if the cell lives on or dies.
//
// The class takes an initial configuration of the NxN grid as input.
// Example:
//    std::array<std::array<Species<N>::CellState, N>, N> grid {};
//    Species<N> species_a(grid);

template <std::size_t size>
class Species {

public:

    enum class CellState {
        DEAD,
        ALIVE
    };

private:

    using coordinate = std::pair<std::size_t, std::size_t>;
    using grid = std::array<std::array<CellState, size>, size>;

    // All cells evolve or die simultaneously, therefore we need
    // one grid for the current generation and another grid for
    // the future/evolved generation.
    grid generation_a {};
    grid generation_b {};

    // Using pointers to each grid allows us to easily change
    // which grid is considered the current generation and which
    // is considered the future generation. We also don't need
    // to make unnecessary copies.
    grid* generation_current {&generation_a};
    grid* generation_future  {&generation_b};

    void change_state(const coordinate&, CellState);
    bool alive(const coordinate&) const;
    int  count_alive_neighbours(const coordinate&) const;

public:

    Species(const grid&);
    Species(const Species<size>&);
    Species<size>& operator=(const Species<size>&);

    void evolve();

    template <std::size_t samesize>
    friend std::ostream& operator<<(std::ostream&, const Species<samesize>&);
};

template <std::size_t size>
void Species<size>::change_state(const coordinate& cell, CellState state) {
    (*generation_future)[cell.first][cell.second] = state;
}

template <std::size_t size>
bool Species<size>::alive(const coordinate& cell) const {
    return (*generation_current)[cell.first][cell.second] == CellState::ALIVE;
}

template <std::size_t size>
int Species<size>::count_alive_neighbours(const coordinate& cell) const {

    std::array<coordinate, 8> coordinates;
    const std::size_t row    {cell.first};
    const std::size_t column {cell.second};
    const std::size_t edge   {size - 1};

    // Check north
    if (row == 0) {
        coordinates[0] = std::make_pair(edge, column);
    } else {
        coordinates[0] = std::make_pair(row - 1, column);
    }

    // Check south
    if (row == edge) {
        coordinates[1] = std::make_pair(0, column);
    } else {
        coordinates[1] = std::make_pair(row + 1, column);
    }

    // Check west
    if (column == 0) {
        coordinates[2] = std::make_pair(row, edge);
    } else {
        coordinates[2] = std::make_pair(row, column - 1);
    }

    // Check east
    if (column == edge) {
        coordinates[3] = std::make_pair(row, 0);
    } else {
        coordinates[3] = std::make_pair(row, column + 1);
    }

    // Check north west
    if (row == 0 && column == 0) {
        coordinates[4] = std::make_pair(edge, edge);
    } else if (row == 0) {
        coordinates[4] = std::make_pair(edge, column - 1);
    } else if (column == 0) {
        coordinates[4] = std::make_pair(row - 1, edge);
    } else {
        coordinates[4] = std::make_pair(row - 1, column - 1);
    }

    // Check north east
    if (row == 0 && column == edge) {
        coordinates[5] = std::make_pair(edge, 0);
    } else if (row == 0) {
        coordinates[5] = std::make_pair(edge, column + 1);
    } else if (column == edge) {
        coordinates[5] = std::make_pair(row - 1, 0);
    } else  {
        coordinates[5] = std::make_pair(row - 1, column + 1);
    }

    // Check south west
    if (row == edge && column == 0) {
        coordinates[6] = std::make_pair(0, edge);
    } else if (row == edge) {
        coordinates[6] = std::make_pair(0, column - 1);
    } else if (column == 0) {
        coordinates[6] = std::make_pair(row + 1, edge);
    } else {
        coordinates[6] = std::make_pair(row + 1, column - 1);
    }

    // Check south east
    if (row == edge && column == edge) {
        coordinates[7] = std::make_pair(0, 0);
    } else if (row == edge) {
        coordinates[7] = std::make_pair(0, column + 1);
    } else if (column == edge) {
        coordinates[7] = std::make_pair(row + 1, 0);
    } else {
        coordinates[7] = std::make_pair(row + 1, column + 1);
    }

    int alive_neighbours {0};
    for (const auto& cell : coordinates) {
        if (alive(cell)) ++alive_neighbours;
    }

    return alive_neighbours;
}

template <std::size_t size>
Species<size>::Species(const grid& generation_initial):
    generation_a {generation_initial} {}

template <std::size_t size>
Species<size>::Species(const Species& other) {
    if (this != &other) {
        // We only need to copy the current generation because
        // we will overwrite the future generation anyways.
        *generation_current = *other.generation_current;
    }
}

template <std::size_t size>
Species<size>& Species<size>::operator=(const Species& other) {
    if (this != &other) {
        // We only need to copy the current generation because
        // we will overwrite the future generation anyways.
        *generation_current = *other.generation_current;
    }
    return *this;
}

// Count every cell's neighbours:
//    1. If a cell has fewer than 2 or more than 3 it dies/stays dead.
//    2. If a cell has 3 it stays alive/comes alive.
//    3. If a cell is alive and has 2 neighbours it stays alive.
template <std::size_t size>
void Species<size>::evolve() {
    for (std::size_t row {0}; row < size; ++row) {
        for (std::size_t column {0}; column < size; ++column) {
            const coordinate cell(row, column);
            const int alive_neighbours {count_alive_neighbours(cell)};
            if (alive_neighbours == 3 || (alive(cell) && alive_neighbours == 2)) {
                change_state(cell, CellState::ALIVE);
            } else {
                change_state(cell, CellState::DEAD);
            }
        }
    }
    // Switch generations. The future generation becomes the new
    // current generation, the current generation becomes the next
    // future generation.
    std::swap(generation_current, generation_future);
}

template <std::size_t size>
std::ostream& operator<<(std::ostream& out, const Species<size>& species) {
    for (const auto& row : *species.generation_current) {
        for (const auto& cell : row) {
            if (cell == Species<size>::CellState::ALIVE) {
                out << '#';
            } else {
                out << ' ';
            }
        }
        out << '\n';
    }
    return out;
}

#endif // SPECIES_H
