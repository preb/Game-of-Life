#include <array>
#include <iostream>

#include "Species.h"

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
