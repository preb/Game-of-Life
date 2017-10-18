#include <iostream>

// TODO Template generation size
// TODO Disable copy initialization/assignment or do deep copying/assignment
// TODO Instead of bool values for each cell use enum class with Alive/Dead
// TODO Change i, j iteration variables to row, column
// TODO Change variables names current_gen, future_gen to generation_...
class Species {

private:
    bool generation_a [10][10] {};
    bool generation_b [10][10] {};
    bool (*current_generation)[10] {generation_a};
    bool (*future_generation)[10]  {generation_b};

    bool alive(int, int) const;
    int count_alive_neighbours(int, int) const;
    void change_state(int, int, bool);

public:
    Species(bool (*)[10]);
    void evolve();
    friend std::ostream& operator<<(std::ostream&, const Species&);
};

Species::Species(bool (*initial_generation)[10]) {
    for (int i {0}; i < 10; ++i) {
        for (int j {0}; j < 10; ++j) {
            generation_a[i][j] = initial_generation[i][j];
        }
    }
}

bool Species::alive(int row, int column) const {
    // TODO throw exception if row, column out of bounds
    return current_generation[row][column];
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

// TODO Remove generation parameter, we only want to change future states.
void Species::change_state(int row, int column, bool state) {
    // TODO throw exception if row, column out of bounds
    future_generation[row][column] = state;
}

// Checks for every cell if it can live on to the next generation
// with the help of its neighbours or if it dies.
// The evolved state for every cell is set for the future generation,
// which then becomes the current generation.
void Species::evolve() {
    for (int i {0}; i < 10; ++i) {
        for (int j {0}; j < 10; ++j) {
            int alive_neighbours {count_alive_neighbours(i, j)};
            if (alive_neighbours < 2 || alive_neighbours > 3) {
                change_state(i, j, false);
            } else if (alive_neighbours == 3 || alive(i, j)) {
                change_state(i, j, true);
            }
        }
    }
    // Recycle generations
    bool (*temp_generation)[10] = current_generation;
    current_generation = future_generation;
    future_generation  = temp_generation;
}

std::ostream& operator<<(std::ostream &out, const Species &species) {
    for (int i {0}; i < 10; ++i) {
        for (int j {0}; j < 10; ++j) {
            if (species.alive(i, j)) {
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
    bool initial_generation[10][10] {};
    initial_generation[2][5] = true;
    initial_generation[3][5] = true;
    initial_generation[4][5] = true;

    Species speciesA(initial_generation);
    std::cout << speciesA;

    speciesA.evolve();
    std::cout << speciesA;

    speciesA.evolve();
    std::cout << speciesA;

    return 0;
}
