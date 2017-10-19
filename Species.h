#ifndef SPECIES_H
#define SPECIES_H

#include <ostream>

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
//    std::array<std::array<Species<N>::Cell, N>, N> grid {};
//    Species<N> species_a(grid);

template <int size>
class Species {

public:

    enum class Cell {
        DEAD,
        ALIVE
    };

private:

    using grid = std::array<std::array<Cell, size>, size>;

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

    void change_state(int, int /* coordinates */, Cell);
    bool alive(int, int /* coordinates */) const;
    int  count_alive_neighbours(int, int /* coordinates */) const;

public:

    Species(const grid&);
    Species(const Species<size>&);
    Species<size>& operator=(const Species<size>&);

    void evolve();

    template <int samesize>
    friend std::ostream& operator<<(std::ostream&, const Species<samesize>&);
};

template <int size>
void Species<size>::change_state(int row, int column, Cell state) {
    (*generation_future)[row][column] = state;
}

template <int size>
bool Species<size>::alive(int row, int column) const {
    return (*generation_current)[row][column] == Cell::ALIVE;
}

template <int size>
int Species<size>::count_alive_neighbours(int row, int column) const {
    const int edge {size - 1};
    int alive_neighbours {0};

    // Check north
    if (row == 0) {
        if (alive(edge, column)) ++alive_neighbours;
    } else {
        if (alive(row - 1, column)) ++alive_neighbours;
    }

    // Check south
    if (row == edge) {
        if (alive(0, column)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column)) ++alive_neighbours;
    }

    // Check west
    if (column == 0) {
        if (alive(row, edge)) ++alive_neighbours;
    } else {
        if (alive(row, column - 1)) ++alive_neighbours;
    }

    // Check east
    if (column == edge) {
        if (alive(row, 0)) ++alive_neighbours;
    } else {
        if (alive(row, column + 1)) ++alive_neighbours;
    }

    // Check north west
    if (row == 0 && column == 0) {
        if (alive(edge, edge)) ++alive_neighbours;
    } else if (row == 0) {
        if (alive(edge, column - 1)) ++alive_neighbours;
    } else if (column == 0) {
        if (alive(row - 1, edge)) ++alive_neighbours;
    } else {
        if (alive(row - 1, column - 1)) ++alive_neighbours;
    }

    // Check north east
    if (row == 0 && column == edge) {
        if (alive(edge, 0)) ++alive_neighbours;
    } else if (row == 0) {
        if (alive(edge, column + 1)) ++alive_neighbours;
    } else if (column == edge) {
        if (alive(row - 1, 0)) ++alive_neighbours;
    } else  {
        if (alive(row - 1, column + 1)) ++alive_neighbours;
    }

    // Check south west
    if (row == edge && column == 0) {
        if (alive(0, edge)) ++alive_neighbours;
    } else if (row == edge) {
        if (alive(0, column - 1)) ++alive_neighbours;
    } else if (column == 0) {
        if (alive(row + 1, edge)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column - 1)) ++alive_neighbours;
    }

    // Check south east
    if (row == edge && column == edge) {
        if (alive(0, 0)) ++alive_neighbours;
    } else if (row == edge) {
        if (alive(0, column + 1)) ++alive_neighbours;
    } else if (column == edge) {
        if (alive(row + 1, 0)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column + 1)) ++alive_neighbours;
    }

    return alive_neighbours;
}

template <int size>
Species<size>::Species(const grid& generation_initial):
    generation_a {generation_initial} {}

template <int size>
Species<size>::Species(const Species& other) {
    if (this != &other) {
        // We only need to copy the current generation because
        // we will overwrite the future generation anyways.
        *generation_current = *other.generation_current;
    }
}

template <int size>
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
template <int size>
void Species<size>::evolve() {
    for (int row {0}; row < size; ++row) {
        for (int column {0}; column < size; ++column) {
            int alive_neighbours {count_alive_neighbours(row, column)};
            if (alive_neighbours < 2 || alive_neighbours > 3) {
                change_state(row, column, Cell::DEAD);
            } else if (alive_neighbours == 3 || alive(row, column)) {
                change_state(row, column, Cell::ALIVE);
            }
        }
    }
    // Switch generations. The future generation becomes the new
    // current generation, the current generation becomes the next
    // future generation.
    grid* generation_temporary {generation_current};
    generation_current = generation_future;
    generation_future  = generation_temporary;
}

template <int size>
std::ostream& operator<<(std::ostream& out, const Species<size>& species) {
    for (const auto& row : *species.generation_current) {
        for (const auto& cell : row) {
            if (cell == Species<size>::Cell::ALIVE) {
                out << "# ";
            } else {
                out << ' ';
            }
        }
        out << '\n';
    }
    return out;
}

#endif // SPECIES_H
