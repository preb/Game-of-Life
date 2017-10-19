#include <array>
#include <iostream>

// TODO Template generation size
// TODO Instead of bool values for each cell use enum class with Alive/Dead
class Species {

public:

    enum class Cell {
        DEAD,
        ALIVE
    };

private:

    using grid = std::array<std::array<Cell, 10>, 10>;

    grid generation_a {};
    grid generation_b {};

    grid* generation_current {&generation_a};
    grid* generation_future  {&generation_b};

    void change_state(int, int, Cell);
    bool alive(int, int) const;
    int  count_alive_neighbours(int, int) const;

public:

    Species(const grid&);
    Species(const Species&);
    Species& operator=(const Species&);

    void evolve();
    friend std::ostream& operator<<(std::ostream&, const Species&);
};

void Species::change_state(int row, int column, Cell state) {
    // TODO throw exception if row, column out of bounds
    (*generation_future)[row][column] = state;
}

bool Species::alive(int row, int column) const {
    // TODO throw exception if row, column out of bounds
    return (*generation_current)[row][column] == Cell::ALIVE;
}

int Species::count_alive_neighbours(int row, int column) const {
    // TODO throw exception if row, column out of bounds
    int alive_neighbours {0};

    // Check north
    if (row == 0) {
        if (alive(9, column)) ++alive_neighbours;
    } else {
        if (alive(row - 1, column)) ++alive_neighbours;
    }

    // Check south
    if (row == 9) {
        if (alive(0, column)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column)) ++alive_neighbours;
    }

    // Check west
    if (column == 0) {
        if (alive(row, 9)) ++alive_neighbours;
    } else {
        if (alive(row, column - 1)) ++alive_neighbours;
    }

    // Check east
    if (column == 9) {
        if (alive(row, 0)) ++alive_neighbours;
    } else {
        if (alive(row, column + 1)) ++alive_neighbours;
    }

    // Check north west
    if (row == 0 && column == 0) {
        if (alive(9, 9)) ++alive_neighbours;
    } else if (row == 0) {
        if (alive(9, column - 1)) ++alive_neighbours;
    } else if (column == 0) {
        if (alive(row - 1, 9)) ++alive_neighbours;
    } else {
        if (alive(row - 1, column - 1)) ++alive_neighbours;
    }

    // Check north east
    if (row == 0 && column == 9) {
        if (alive(9, 0)) ++alive_neighbours;
    } else if (row == 0) {
        if (alive(9, column + 1)) ++alive_neighbours;
    } else if (column == 9) {
        if (alive(row - 1, 0)) ++alive_neighbours;
    } else  {
        if (alive(row - 1, column + 1)) ++alive_neighbours;
    }

    // Check south west
    if (row == 9 && column == 0) {
        if (alive(0, 9)) ++alive_neighbours;
    } else if (row == 9) {
        if (alive(0, column - 1)) ++alive_neighbours;
    } else if (column == 0) {
        if (alive(row + 1, 9)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column - 1)) ++alive_neighbours;
    }

    // Check south east
    if (row == 9 && column == 9) {
        if (alive(0, 0)) ++alive_neighbours;
    } else if (row == 9) {
        if (alive(0, column + 1)) ++alive_neighbours;
    } else if (column == 9) {
        if (alive(row + 1, 0)) ++alive_neighbours;
    } else {
        if (alive(row + 1, column + 1)) ++alive_neighbours;
    }

    return alive_neighbours;
}

Species::Species(const grid& generation_initial) {
    for (int row {0}; row < 10; ++row) {
        for (int column {0}; column < 10; ++column) {
            generation_a[row][column] = generation_initial[row][column];
        }
    }
}

Species::Species(const Species& other) {
    if (this != &other) {
        *generation_current = *other.generation_current;
    }
}

Species& Species::operator=(const Species& other) {
    if (this != &other) {
        *generation_current = *other.generation_current;
    }
    return *this;
}

// Checks for every cell if it can live on to the next generation
// with the help of its neighbours or if it dies.
// The evolved state for every cell is set for the future generation,
// which then becomes the current generation.
void Species::evolve() {
    for (int row {0}; row < 10; ++row) {
        for (int column {0}; column < 10; ++column) {
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

std::ostream& operator<<(std::ostream &out, const Species &species) {
    for (const auto& row : *species.generation_current) {
        for (const auto& cell : row) {
            if (cell == Species::Cell::ALIVE) {
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
    std::array<std::array<Species::Cell, 10>, 10> generation_initial {};
    generation_initial[2][5] = Species::Cell::ALIVE;
    generation_initial[3][5] = Species::Cell::ALIVE;
    generation_initial[4][5] = Species::Cell::ALIVE;

    Species species_a(generation_initial);
    species_a.evolve();
    std::cout << species_a;

    Species species_b(species_a);
    species_b.evolve();
    std::cout << species_b;

    Species species_c(species_b);
    species_c = species_a;
    std::cout << species_c;

    return 0;
}
