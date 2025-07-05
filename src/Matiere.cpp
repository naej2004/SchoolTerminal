#include "Matiere.hpp"

Matiere::Matiere(std::string name, int coefficent) : name(name), coefficient(coefficent) {}

std::string Matiere::getName() const {
    return this->name;
}

int Matiere::getCoefficient() const {
    return this->coefficient;
}

void Matiere::setCoefficient(int const &newCoefficient) {
    this->coefficient = newCoefficient;
}

void Matiere::setName(std::string const &newName) {
    this->name = newName;
}
