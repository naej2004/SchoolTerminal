#pragma once

#include "Matiere.hpp"
#include <string>

class Note {
public:
    Note() = delete;
    Note(double value, double max_point, int multiple, Matiere field);
    [[nodiscard]] double getValue() const;
    [[nodiscard]] double getMaxPoint() const;
    [[nodiscard]] int getMultiple() const;
    [[nodiscard]] Matiere getField() const;
    void setValue(double const &newValue);
    void setMaxPoint(double const &newMaxPoint);
    void setField(std::string const &newNameField, int const &newCoefficientField);
    ~Note() = default;
private:
    double value;
    double max_point;
    int multiple;
    Matiere field;
};
