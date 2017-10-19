#include <array>
#include <iostream>

// TODO use seperate files for class
template <int size>
class Species {

public:

    enum class Cell {
        DEAD,
        ALIVE
    };

private:

    using grid = std::array<std::array<Cell, size>, size>;

    grid generation_a {};
    grid generation_b {};

    grid* generation_current {&generation_a};
    grid* generation_future  {&generation_b};

    void change_state(int, int, Cell);
    bool alive(int, int) const;
    int  count_alive_neighbours(int, int) const;

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
    // TODO throw exception if row, column out of bounds
    (*generation_future)[row][column] = state;
}

template <int size>
bool Species<size>::alive(int row, int column) const {
    // TODO throw exception if row, column out of bounds
    return (*generation_current)[row][column] == Cell::ALIVE;
}

template <int size>
int Species<size>::count_alive_neighbours(int row, int column) const {
    // TODO throw exception if row, column out of bounds
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
Species<size>::Species(const grid& generation_initial) {
    for (int row {0}; row < size; ++row) {
        for (int column {0}; column < size; ++column) {
            generation_a[row][column] = generation_initial[row][column];
        }
    }
}

template <int size>
Species<size>::Species(const Species& other) {
    if (this != &other) {
        *generation_current = *other.generation_current;
    }
}

template <int size>
Species<size>& Species<size>::operator=(const Species& other) {
    if (this != &other) {
        *generation_current = *other.generation_current;
    }
    return *this;
}

// Checks for every cell if it can live on to the next generation
// with the help of its neighbours or if it dies.
// The evolved state for every cell is set for the future generation,
// which then becomes the current generation.
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
    // Recycle generations
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

int main() {

    const int size {10};

    std::array<std::array<Species<size>::Cell, size>, size> generation_initial {};
    generation_initial[2][5] = Species<size>::Cell::ALIVE;
    generation_initial[3][5] = Species<size>::Cell::ALIVE;
    generation_initial[4][5] = Species<size>::Cell::ALIVE;

    Species<size> species_a(generation_initial);
    species_a.evolve();
    std::cout << species_a;

    Species<size> species_b(species_a);
    species_b.evolve();
    std::cout << species_b;

    Species<size> species_c(species_b);
    species_c = species_a;
    std::cout << species_c;

    return 0;
}
