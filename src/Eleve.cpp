#include "Eleve.hpp"
#include <iostream>

Eleve::Eleve(std::string id, std::string name, std::string prenom, Classe classe, std::string gender) : id(id), name(name), prenom(prenom), gender(gender), classe(classe) {}

Classe Eleve::getClasse() const {
    return this->classe;
}

std::string Eleve::getName() const {
    return this->name;
}

std::string Eleve::getPrenom() const {
    return this->prenom;
}

std::string Eleve::getGender() const {
    return this->gender;
}

std::string Eleve::getId() const {
    return this->id;
}
