#include <array>
#include <iostream>

#include "Species.h"

int main() {

    const int size {10};

    std::array<std::array<Species<size>::CellState, size>, size> generation_initial {};
    generation_initial[1][1] = Species<size>::CellState::ALIVE;
    generation_initial[1][2] = Species<size>::CellState::ALIVE;
    generation_initial[2][1] = Species<size>::CellState::ALIVE;

    generation_initial[3][4] = Species<size>::CellState::ALIVE;
    generation_initial[4][3] = Species<size>::CellState::ALIVE;
    generation_initial[4][4] = Species<size>::CellState::ALIVE;

    Species<size> species_a(generation_initial);
    std::cout << species_a;

    species_a.evolve();
    std::cout << species_a;

    return 0;
}
