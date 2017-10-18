#include <iostream>

// TODO Template generation size
class Species {

private:
    bool generation_a [10][10] {};
    bool generation_b [10][10] {};
    bool (*current_generation)[10] {generation_a};
    bool (*future_generation)[10]  {generation_b};

    void change_state(bool (*)[10], int, int);

public:
    friend std::ostream& operator<<(std::ostream&, const Species&);
};

void Species::change_state(bool (*generation)[10], int row, int column) {
    if (row >= 0 && row <= 10 && column >= 0 && column <= 10) {
        generation[row][column] = !generation[row][column];
    } else {
        // TODO throw proper exception
        std::cout << "Error: out of bounds!\n";
    }
}

std::ostream& operator<<(std::ostream &out, const Species &species) {
    for (int i {0}; i < 10; ++i) {
        for (int j {0}; j < 10; ++j) {
            out << species.current_generation[i][j] << ' ';
        }
        out << '\n';
    }
    return out;
}

int main() {
    Species speciesA;
    std::cout << speciesA;
    return 0;
}
