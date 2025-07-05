#include "Note.hpp"

Note::Note(double value, double max_point, int multiple, Matiere field) : value(value), max_point(max_point), multiple(multiple), field(field) {}

double Note::getValue() const {
    return this->value;
}

double Note::getMaxPoint() const {
    return this->max_point;
}

int Note::getMultiple() const
{
    return this->multiple;
}

Matiere Note::getField() const {
    return this->field;
}

void Note::setValue(double const &newValue) {
    this->value = newValue;
}

void Note::setMaxPoint(double const &newMaxPoint) {
    this->max_point = newMaxPoint;
}

void Note::setField(std::string const &newNameField, int const &newCoefficientField) {
    this->field.setName(newNameField);
    this->field.setCoefficient(newCoefficientField);
}
