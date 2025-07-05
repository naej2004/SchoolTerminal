#pragma once

#include <string>
#include <vector>
#include "Classe.hpp"
#include "Matiere.hpp"
#include "Note.hpp"

class Eleve {
public:
    Eleve() = delete;
    Eleve(std::string id, std::string name, std::string prenom, Classe classe, std::string gender);
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getPrenom() const;
    [[nodiscard]] Classe getClasse() const;
    [[nodiscard]] std::string getGender() const;
    [[nodiscard]] std::string getId() const;
    ~Eleve() = default;
private:
    std::string id;
    std::string name;
    std::string prenom;
    std::string gender;
    Classe classe;
};
