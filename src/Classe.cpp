#include "Classe.hpp"

Classe::Classe(std::string name) : name(name) {}

std::string Classe::getName() const {
    return this->name;
}
