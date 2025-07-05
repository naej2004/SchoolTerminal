#pragma once

#include <string>

class Classe {
public:
    Classe() = delete;
    explicit Classe(std::string name);
    [[nodiscard]] std::string getName() const;
    ~Classe() = default;
private:
    std::string name;

};
