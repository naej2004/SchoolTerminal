#pragma once

#include "Eleve.hpp"
#include "Note.hpp"
#include <string>
#include <vector>

void beginProgram();

std::string toLower(const std::string& str);

std::string toUpper(const std::string & str);

std::vector<std::string> listMatiere(const Eleve &student);

int getNoteNumberRegex(const std::string& fileName);

std::vector<Note>listNoteOfMatiere(const Eleve &student, const std::string &matiere);

double truncateTo2Digits(double value);

void menu(const Eleve &student);

void addMatiere(const Eleve &student);

void addNoteOfMatiere(const Eleve &student, const std::string &matiere);

void modifyNoteofMatiere(const Eleve &student, Note &note, int niemeNote);

void deleteNoteofMatiere(const Eleve &student, const Note &note, int niemeNote);

double meanOfMatiere(const Eleve &student, const std::string &matiere);

double meanofStudent(const Eleve &student);

void infoOfStudent(const Eleve &student);

void endProgram(const Eleve &student);

std::string filesStudent(const std::string &idUser);

Eleve createStudent();

bool connexionStudent(std::array<std::string, 5> &infoStudents);

void try_again_connexion(std::array<std::string, 5> &infoStudents);

void entryErrorForInt();
