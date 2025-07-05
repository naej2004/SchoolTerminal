#pragma once

#include <string>

class Matiere {
    public:
        Matiere() = delete;
        Matiere(std::string name, int coefficient);
        [[nodiscard]] std::string getName() const;
        [[nodiscard]] int getCoefficient() const;
        void setCoefficient(int const &newCoefficient);
        void setName(std::string const &newName);
        ~Matiere() = default;
private:
    std::string name;
    int coefficient;
};
