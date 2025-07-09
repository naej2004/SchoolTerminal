#include <array>
#include <iostream>
#include <filesystem>
#include <limits>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iomanip>
#include <regex>
#include "Eleve.hpp"
#include "Classe.hpp"
#include "Note.hpp"
#include "Matiere.hpp"
#include "functionality.hpp"


void beginProgram()
{
    int choice;
    do
    {
        std::cout << "OPTION DISPONIBLE\n" << std::endl;
        std::cout << "\t1- Se connecter " << std::endl;
        std::cout << "\t2- Quitter l'application " << std::endl;
        std::cout << "\nEntrer votre choix : ";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        entryErrorForInt();
    }while (choice != 1 && choice != 2);
    if (choice == 1)
    {
        std::array<std::string, 5> infoStudents{};
        if (const bool isConnected = connexionStudent(infoStudents); !isConnected)
        {
            try_again_connexion(infoStudents);
        }
        else
        {
            Classe const classeStudent(infoStudents[3]);
            const Eleve student(infoStudents[0], infoStudents[1], infoStudents[2], classeStudent, infoStudents[4]);
            menu(student);
        }
    }
}

std::vector<std::string> listMatiere(const Eleve &student)
{
    std::string const basePath = "../../data";
    std::filesystem::path const fullPath = std::filesystem::path(basePath) /student.getId();
    std::vector<std::string> allMatiere;
    for (auto const& folder : std::filesystem::directory_iterator(fullPath))
    {
        if (folder.is_directory())
        {
            allMatiere.push_back(folder.path().filename().string());
        }
    }
    return allMatiere;
}

std::string toUpper(const std::string & str)
{
    std::string result = str;
    std::ranges::transform(result, result.begin(),
                           [](const unsigned char c) { return std::toupper(c); });
    return result;
}

std::string toLower(const std::string& str) {
    std::string result = str;
    std::ranges::transform(result, result.begin(),
                           [](const unsigned char c) { return std::tolower(c); });
    return result;
}

std::vector<Note> listNoteOfMatiere(const Eleve& student, const std::string &matiere)
{
    std::string const basePath = "../../data";
    std::string notes = "notes";
    std::filesystem::path const fullPath = std::filesystem::path(basePath) /student.getId() / matiere;
    std::vector<Note> allNoteOfMatiere;

    std::array<std::string, 2> infoField;
    std::string infoMatiereFile = "infoMatiere.txt";
    std::filesystem::path infoMatierePath = std::filesystem::path(fullPath) / infoMatiereFile;
    std::ifstream infoMatiere {infoMatierePath};
    if (infoMatiere.is_open())
    {
        std::string line;
        int i = 0;
        while (std::getline(infoMatiere >> std::ws, line))
        {
            infoField[i] = line;
            i++;
        }
    }
    else
    {
        std::cout << "Chemin vers 'infoMatiere.txt est incorrect !" << std::endl;
    }

    for (auto const& folder : std::filesystem::directory_iterator(fullPath))
    {
        if (folder.is_directory())
        {
            for (auto const &contenu : std::filesystem::directory_iterator(folder.path()))
            {
                std::string noteMatiere = contenu.path().filename().string();
                std::ifstream note {contenu.path()};
                if (note.is_open())
                {
                    std::array<std::string, 3> infoNote;
                    std::string line;
                    int i = 0;
                    while (std::getline(note >> std::ws, line))
                    {
                        infoNote[i] = line;
                        i++;
                    }
                    double noteStudent = std::stod(infoNote[0]);
                    double maxNote = std::stod(infoNote[1]);
                    int multiple = std::stoi(infoNote[2]);
                    Matiere field {matiere, std::stoi(infoField[1])};
                    Note noteInstance{noteStudent, maxNote, multiple,field};
                    allNoteOfMatiere.push_back(noteInstance);
                }
                else
                {
                    std::cout << "Le fichier n'existe pas !" << std::endl;
                }
            }
        }
    }
    return allNoteOfMatiere;
}

std::vector<Eleve> listStudent()
{
    std::vector<Eleve> listEleve;
    auto const dataPath = std::filesystem::path("../../data");
    for (auto const &id : std::filesystem::directory_iterator(dataPath))
    {
        if (id.is_directory())
        {
            for (auto const &contenu : std::filesystem::directory_iterator(id))
            {
                if (!(contenu.is_directory()))
                {
                    if (contenu.path().filename().string() == "infoStudent.txt")
                    {
                        std::array<std::string, 5> infoStudent;
                        std::ifstream infoStudentFile{contenu.path()};
                        if (infoStudentFile.is_open())
                        {
                            std::string line;
                            int i = 0;
                            while (std::getline(infoStudentFile, line))
                            {
                                infoStudent[i] = line;
                                i++;
                            }
                            infoStudentFile.close();
                        }
                        Classe classe(infoStudent[3]);
                        Eleve student(infoStudent[0], infoStudent[1], infoStudent[2], classe, infoStudent[4]);
                        listEleve.push_back(student);
                    }
                    else
                    {
                        std::cout << "Fichier 'infoStudent.txt' introuvable" << std::endl;
                        return listEleve;
                    }
                }

            }
        }
    }
    return listEleve;
}

int getNoteNumberRegex(const std::string& fileName) {
    const std::regex pattern(R"(note(\d+)\.txt)"); // Regex : note + chiffres + .txt
    std::smatch match;

    if (std::regex_search(fileName, match, pattern)) {
        return std::stoi(match[1]); // match[1] contient le chiffre capturé
    }
    return -1; // Aucun numéro trouvé
}

double truncateTo2Digits(double value)
{
    return static_cast<int>(value * 100) / 100.0;
}

void menu(const Eleve &student)
{
    bool go = true;
    while (go){
        std::cout << "\n========================================SCHOOLTERMINAL========================================\n"
                  << std::endl;
        std::cout << "1- Ajouter une matiere" << std::endl;
        std::cout << "2- Ajouter une note a une matiere" << std::endl;
        std::cout << "3- Modifier la note d'une matiere" << std::endl;
        std::cout << "4- Supprimer une note d'une matiere" << std::endl;
        std::cout << "5- Voir mes notes" << std::endl;
        std::cout << "6- Calculer la moyenne d'une matiere" << std::endl;
        std::cout << "7- Calculer ma moyenne generale" << std::endl;
        std::cout << "8- Voir mes informations" << std::endl;
        std::cout << "9- Quitter\n" << std::endl;
        std::cout << "\nEntrer votre choix : ";
        int choice;
        std::cin >> choice;
        entryErrorForInt();
        switch (choice)
        {
        case 1:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                addMatiere(student);
                break;
            }
        case 2:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous d'ajouter une note a une matiere donnee !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES MATIERES : " << std::endl;
                for (auto i = 1; i <= allMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allMatiere[i-1] << std::endl;
                }
                std::cout << "Choisissez la matiere dont vous souhaitez ajouter une note (ex : 2) : ";
                int choixMatiere;
                std::cin >> choixMatiere;
                addNoteOfMatiere(student, toUpper(allMatiere[choixMatiere - 1]));
                break;
            }
        case 3:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de modifier la note d'une matiere !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES MATIERES : " << std::endl;
                for (auto i = 1; i <= allMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allMatiere[i-1] << std::endl;
                }
                std::cout << "Choisissez la matiere dont vous souhaitez modifier une note (ex : 2) : ";
                int choixMatiere;
                std::cin >> choixMatiere;
                std::vector<Note> allNoteOfMatiere = listNoteOfMatiere(student, allMatiere[choixMatiere - 1]);
                if (allNoteOfMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune note pour la matiere '" <<
                        allMatiere[choixMatiere - 1] << "' a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de modifier une note pour cette matiere !" << std::endl;
                    std::cout << "Pour ajouter une note, selectionner l'option '2' !" << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES NOTES (" << toUpper(allMatiere[choixMatiere - 1]) << ")" << std::endl;
                for (auto i = 1; i <= allNoteOfMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allNoteOfMatiere[i-1].getValue() <<  "/" <<
                        allNoteOfMatiere[i-1].getMaxPoint() << std::endl;
                }
                std::cout << "Choisissez la note que vous souhaitez modifier (ex : 2) : ";
                int choixNote;
                std::cin >> choixNote;
                int const niemeNote = choixNote;
                modifyNoteofMatiere(student, allNoteOfMatiere[choixNote - 1], niemeNote);
                break;
            }
        case 4:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de modifier la note d'une matiere !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES MATIERES : " << std::endl;
                for (auto i = 1; i <= allMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allMatiere[i-1] << std::endl;
                }
                std::cout << "Choisissez la matiere dont vous souhaitez supprimer une note (ex : 2) : ";
                int choixMatiere;
                std::cin >> choixMatiere;
                std::vector<Note> allNoteOfMatiere = listNoteOfMatiere(student, allMatiere[choixMatiere - 1]);
                if (allNoteOfMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune note pour la matiere '" <<
                        allMatiere[choixMatiere - 1] << "' a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de supprimer une note pour cette matiere !" << std::endl;
                    std::cout << "Pour ajouter une note, selectionner l'option '2' !" << std::endl;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES NOTE DE " << toUpper(allMatiere[choixMatiere - 1]) << " :" <<  std::endl;
                for (auto i = 1; i <= allNoteOfMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allNoteOfMatiere[i-1].getValue() <<  "/" <<
                        allNoteOfMatiere[i-1].getMaxPoint() << std::endl;
                }
                std::cout << "Choisissez la note que vous souhaitez modifier (ex : 2) : ";
                int choixNote;
                std::cin >> choixNote;
                deleteNoteofMatiere(student, allNoteOfMatiere[choixNote - 1], choixNote);
                break;
            }
        case 5:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de voir la liste de vos notes !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "\nLISTE DES NOTES DE " << student.getName() << " " << student.getPrenom() << " (" <<
                    student.getId() << ") : \n" << std::endl;

                for (auto i = 1; i <= allMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allMatiere[i-1] << ": " << std::endl;
                    auto const listNote = listNoteOfMatiere(student, allMatiere[i-1]);
                    for (auto const &note : listNote)
                    {
                        std::cout << "\t- " << note.getValue() << "/" << note.getMaxPoint() << std::endl;
                    }
                    std::cout << std::endl;
                }
                std::cout << "Appuie sur la touche Entrer pour continuer... ";
                std::cin.get();
                break;
            }
        case 6:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de calculer la moyenne d'une matiere !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }
                std::cout << "LISTE DES MATIERES : " << std::endl;
                for (auto i = 1; i <= allMatiere.size(); ++i)
                {
                    std::cout << i << "- " << allMatiere[i-1] << std::endl;
                }
                std::cout << "Choisissez une matiere (ex : 2) : ";
                int choixMatiere;
                std::cin >> choixMatiere;
                double const moyenneMatiere = meanOfMatiere(student, toUpper(allMatiere[choixMatiere - 1]));
                std::cout << student.getName() << " " << student.getPrenom() << " (" << student.getClasse().getName()
                << ") votre moyenne a la matiere " << allMatiere[choixMatiere - 1] << " est de " << moyenneMatiere
                << "/20" << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                std::cin.get();
                break;
            }
        case 7:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::vector<std::string> allMatiere =  listMatiere(student);
                if (allMatiere.empty())
                {
                    std::cout << "Vous ne possedez actuellement aucune matiere a votre compte !" << std::endl;
                    std::cout << "Il est donc impossible pour vous de calculer la moyenne d'une matiere !" << std::endl;
                    std::cout << "Pour ajouter une matiere, selectionner l'option '1' !" << std::endl;
                    //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                    std::cin.get();
                    break;
                }

                double const moyenneStudent = meanofStudent(student);
                if (moyenneStudent == 0)
                {
                    break;
                }
                std::cout << student.getName() << " " << student.getPrenom() << " (" << student.getClasse().getName()
                << ") votre moyenne global est de " << moyenneStudent << "/20" << std::endl;
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nAppuie sur la touche Entrer pour continuer... ";
                std::cin.get();
                break;
            }
        case 8:
            {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                infoOfStudent(student);
                break;
            }
        case 9:
            {
                go = false;
                endProgram(student);
                break;
            }
        default:
            std::cout << "Commande inconnu ! " << std::endl;
        }
    }
}

void addNoteOfMatiere(const Eleve &student, const std::string &matiere)
{
    std::string const basePath = "../../data";
    std::string const id = student.getId();
    std::filesystem::path const fullPath = std::filesystem::path(basePath) / id;
    for (const auto &folder : std::filesystem::directory_iterator{fullPath})
    {
        if(folder.is_directory() && folder.path().filename().string() == matiere)
        {
            //std::cout << "La matierer existe !" << std::endl;
            int nombreNotes = 0;
            for (auto const &sub_folder : std::filesystem::directory_iterator(folder.path()))
            {
                if (sub_folder.is_directory())
                {
                    if (sub_folder.path().filename().string() == "notes")
                    {
                        for ([[maybe_unused]] auto const &contenu :
                            std::filesystem::recursive_directory_iterator(sub_folder.path()))
                        {
                            nombreNotes++;
                        }
                    }
                }
            }
            std::string fileName = "note";
            if (nombreNotes == 0)
            {
                nombreNotes = 1;
            }
            else
            {
                nombreNotes++;
            }
            fileName += std::to_string(nombreNotes);
            fileName += ".txt";
            std::string notes = "notes";
            std::filesystem::path newFullPath = std::filesystem::path(fullPath) / matiere / notes / fileName;
            newFullPath = newFullPath.string();
            std::ofstream newNote(newFullPath);
            if (newNote.is_open())
            {
                double maxNote;
                do
                {
                    std::cout << "Quelle est la note maximale possible d'obtenir pour cette note ? : ";
                    std::cin >> maxNote;
                    if (maxNote != 20 && maxNote != 40)
                    {
                        std::cout << "Choisissez '20' ou '40' pour la note maximale à obtenir !" << std::endl;
                    }
                    entryErrorForInt();
                }while (maxNote != 20 && maxNote != 40);
                double studentNote;
                do
                {
                    std::cout << "Combien avez-vous obtenez sur " << maxNote << " ? : ";
                    std::cin >> studentNote;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    if (studentNote > maxNote || studentNote < 0)
                    {
                        std::cout << "Entrez un nombre compris entre 0 et " << maxNote << " !" << std::endl;
                    }
                    entryErrorForInt();
                }while (studentNote > maxNote || studentNote < 0);
                int multiple;
                if (maxNote == 20)
                {
                    multiple = 1;
                }
                if (maxNote == 40)
                {
                    multiple = 2;
                }
                newNote << studentNote << "\n" << maxNote << "\n" << multiple;
                std::cout << "Votre note (" << studentNote << "/" << maxNote << ") a bien ete ajoute !" << std::endl;
                newNote.close();
            }
        }
    }
}

void addMatiere(const Eleve &student)
{
    std::cout << "Ecris le nom de la matiere : ";
    std::string nameMatiere;
    std::getline(std::cin, nameMatiere);
    nameMatiere = toUpper(nameMatiere);
    std::string const basePath = "../../data";
    std::string const id = student.getId();
    std::filesystem::path const fullPath = std::filesystem::path(basePath) / id;
    for (const auto &folder : std::filesystem::directory_iterator{fullPath})
    {
        if(folder.is_directory() && folder.path().filename().string() == nameMatiere)
        {
            //std::cout << "Cette matiere existe deja !" << std::endl;
            int choix;
            do
            {
                std::cout << "Option possible : " << std::endl;;
                std::cout << "1- Entrer une autre matiere" << std::endl;
                std::cout << "2- Annuler mon action (ajout de matiere)" << std::endl;
                std::cout << "Entrez votre choix : " << std::endl;
                std::cin >> choix;
                entryErrorForInt();
                if (choix == 1)
                {
                    std::cout << "Entrer un autre nom : " << std::endl;
                    std::getline(std::cin, nameMatiere);
                    nameMatiere = toUpper(nameMatiere);
                }else if (choix == 2)
                {
                    return;
                }
                else
                {
                    std::cout << "Action impossible" << std::endl;
                }
            }
            while (choix != 1 && choix != 2);
        }
    }
    std::cout << "Entrer le coefficient de la matiere : ";
    int coefficentMatiere;
    std::cin >> coefficentMatiere;
    std::filesystem::path directoryMatiere = std::filesystem::path(fullPath) / nameMatiere;
    if (std::filesystem::create_directory(directoryMatiere))
    {
        std::cout << nameMatiere << " ajoute avec succes !" << std::endl;
        std::string pathInfoMatiere = directoryMatiere.string() + "/infoMatiere.txt";
        std::ofstream infoMatiere{pathInfoMatiere};
        if (infoMatiere.is_open())
        {
            infoMatiere << nameMatiere << "\n" << coefficentMatiere;
            //std::cout << "Fichier infoMatiere.txt cree avec succes !" << std::endl;
            infoMatiere.close();
        }
        std::filesystem::path const &noteDirectory = directoryMatiere / "notes";
        std::filesystem::create_directory(noteDirectory);
    }
    else
    {
       std::cout << "Matiere non cree !" << std::endl;
    }
}

void modifyNoteofMatiere(const Eleve &student, Note &note, int niemeNote)
{
    // Mettre deux chiffre après la vuirgule : std::cout << std::fixed << std::setprecision(2);
    double newNoteValue;
    do
    {
        std::cout << "Entrez la nouvelle note sur " << note.getMaxPoint() << " : ";
        std::cin >> newNoteValue;
        if (newNoteValue > note.getMaxPoint() || newNoteValue < 0.0)
        {
            std::cout << "Entrez un nombre entre 0 et " << note.getMaxPoint() << std::endl;
        }
        entryErrorForInt();
    }while (newNoteValue > note.getMaxPoint() || newNoteValue < 0.0);
    note.setValue(newNoteValue);
    const std::filesystem::path matierePath = std::filesystem::path ("../../data") / student.getId() /
        note.getField().getName();
    for (auto const &contenu : std::filesystem::directory_iterator(matierePath))
    {
        if (contenu.is_directory())
        {
            for (auto const &otherContenu : std::filesystem::directory_iterator(contenu))
            {
                if (niemeNote == getNoteNumberRegex(otherContenu.path().filename().string()))
                {
                    std::ifstream mark {otherContenu.path()};
                    std::array<std::string, 3> contenuNote;
                    if (mark.is_open())
                    {
                        std::string line;
                        int i = 0;
                        while (std::getline(mark >> std::ws, line))
                        {
                            contenuNote[i] = line;
                            i++;
                        }
                        mark.close();
                    }
                    else
                    {
                        std::cout << "La note n'existe pas !" << std::endl;
                        return;
                    }
                    contenuNote[0] = std::to_string(note.getValue());

                    std::ofstream newMark {otherContenu.path(), std::ios::ate};
                    if (newMark.is_open())
                    {
                        newMark << contenuNote[0] << "\n" << contenuNote[1] << "\n" << contenuNote[2];
                        newMark.close();
                    }
                }
            }
        }
    }
}

void deleteNoteofMatiere(const Eleve &student, const Note &note, int niemeNote)
{
    std::filesystem::path pathField = std::filesystem::path("../../data") / student.getId() / note.getField().getName();
    for (auto const &folder : std::filesystem::directory_iterator(pathField))
    {
        if (folder.is_directory())
        {
            for (auto const &contenu : std::filesystem::directory_iterator(folder))
            {
                if (niemeNote == getNoteNumberRegex(contenu.path().filename().string()))
                {
                    std::filesystem::remove(contenu.path());
                    std::cout << "Note supprime avec succees !" << std::endl;
                    return;
                }
            }
        }
    }
}

double meanOfMatiere(const Eleve &student, const std::string &matiere)
{
    std::vector<Note> const &listNote = listNoteOfMatiere(student, matiere);
    if (listNote.empty())
    {
        return 0;
    }
    double somme = 0;
    int divBy = 0;
    for (auto const &note : listNote)
    {
        divBy += note.getMultiple();
        somme += note.getValue();
    }
    const double result = truncateTo2Digits(somme / divBy);
    return result;
}

double meanofStudent(const Eleve &student)
{
    //std::cout << "Begin" << std::endl;
    int divBy = 0;
    double sommeMean = 0.0;
    std::filesystem::path const fullPath = std::filesystem::path("../../data") / student.getId();
    for (auto const &folder : std::filesystem::directory_iterator(fullPath))
    {
        if (folder.is_directory())
        {
            std::vector<Note> allNoteOfMatiere = listNoteOfMatiere(student, folder.path().filename().string());
            if (allNoteOfMatiere.empty())
            {
                std::cout << "Vous ne possedez actuellement aucune note pour la matiere '" <<
                    folder.path().filename().string() << "' a votre compte !" << std::endl;
                std::cout << "Il est donc impossible pour vous de calculer votre moyenne generale !" << std::endl;
                std::cout << "Pour ajouter une note, selectionner l'option '2' !" << std::endl;
                //std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Appuie sur la touche Entrer pour continuer... ";
                std::cin.get();
                return 0;
            }
            std::filesystem::path const infoMatierePath = std::filesystem::path(folder) / "infoMatiere.txt";
            std::ifstream infoMatiere {infoMatierePath};
            //std::cout << "infoMatierePath = " << infoMatierePath << std::endl;
            std::array<std::string, 2> infoField;
            if (infoMatiere.is_open())
            {
                std::string line;
                int i = 0;
                while (std::getline(infoMatiere >> std::ws, line))
                {
                    infoField[i] = line;
                    i++;
                }
            }
            else
            {
                std::cout << "Le fichier 'infoMatiere' ne s'ouvre pas !" << std::endl;
                return 1;
            }
            int const &coefficientMatiere = std::stoi(infoField[1]);
            double meanOfFolder = meanOfMatiere(student, folder.path().filename().string());
            meanOfFolder *= coefficientMatiere;
            sommeMean += meanOfFolder;
            divBy += coefficientMatiere;
        }
    }
    double const result = truncateTo2Digits(sommeMean / divBy);
    return result;
}

void infoOfStudent(const Eleve &student)
{
    std::cout <<"\nNom : " << student.getName() << std::endl;
    std::cout << "Prenom : " << student.getPrenom() << std::endl;
    std::cout << "Classe : " << student.getClasse().getName() << std::endl;
    std::cout << "LISTES DES MATIERES : " << std::endl;
    std::vector<std::string> const listField = listMatiere(student);
    if (listField.empty())
    {
        std::cout << "Aucune matiere disponible !" << std::endl;

    }
    else
    {
        int nombreMatiere = 0;
        for (auto const &contenu : listField)
        {
            nombreMatiere++;
            std::cout << nombreMatiere << "- " << contenu << " (Moyenne : " << meanOfMatiere(student, contenu) <<
            " /20) "<< std::endl;
        }
        std::cout << "Votre moyenne generale est de " << meanofStudent(student) << "/20 !" << std::endl;
        std::cout << "\nAppuie sur la touche Entrer pour quitter le programme... ";
        std::cin.get();
    }
}

void endProgram(const Eleve &student)
{
    std::cout << "Good bye " << student.getName() << " " << student.getPrenom() << std::endl;
    std::cout << "Appuie sur la touche Entrer pour quitter le programme... ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string filesStudent(const std::string &idUser)
{
    std::filesystem::path searchPath{"../../data"};
    std::string result;
    for (const auto &entry : std::filesystem::directory_iterator(searchPath))
    {
        if (entry.is_directory() && entry.path().filename().string() == idUser)
        {
            for (const auto &contenu : std::filesystem::directory_iterator(entry.path()))
            {
                if (contenu.path().filename() == "infoStudent.txt")
                {
                    result = contenu.path().string();
                    return result;
                }
            }
            break; // On a trouvé le bon dossier, on quitte la boucle
        }
    }
    return result;
}

Eleve createStudent()
{
    std::cout << "Entrer un ID : ";
    std::string id;
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    id = toLower(id);
    std::cout << "Entrer votre nom de famille : ";
    std::string nom;
    std::cin >> nom;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Entrer votre prenom : ";
    std::string prenom;
    std::getline(std::cin, prenom);
    std::cout << "Entrer votre classe : ";
    std::string classe;
    std::getline(std::cin, classe);
    int choice;
    std::string genre;
    do
    {
        std::cout << "Entrer votre genre (1- Masculin/ 2- Feminin) : ";
        std::cin >> choice;
        entryErrorForInt();
        if (choice == 1)
        {
            genre = "Masculin";
        }
        else if (choice == 2)
        {
            genre = "Feminin";
        }
        else
        {
            std::cout << "Action impossible" << std::endl;
        }
    }
    while (choice != 1 && choice != 2);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    Classe const classeStudent(classe);
    Eleve newStudent(id, nom, prenom, classeStudent, genre);
    std::string basePath{"../../data/"};
    std::string newDirectory{id};
    std::filesystem::path fullPath = std::filesystem::path(basePath) / newDirectory;
    if (std::filesystem::create_directory(fullPath))
    {
        std::string fileName = "infoStudent.txt";
        std::filesystem::path newFullPath = std::filesystem::path(fullPath) / fileName;
        newFullPath = newFullPath.string();
        std::ofstream newFile(newFullPath);
        if (newFile.is_open())
        {
            newFile << id << "\n"
                    << nom << "\n"
                    << prenom << "\n"
                    << classe << "\n"
                    << genre;
            newFile.close();
            std::cout << std::endl;
        }
        std::ofstream idFile("../../data/id.txt", std::ios::app);
        if (idFile.is_open())
        {
            idFile << id << "\n";
            idFile.close();
            std::cout << std::endl;
        }
        //std::cout << "Eleve cree avec success !" << std::endl;
    }
    return newStudent;
}

bool connexionStudent(std::array<std::string, 5> &infoStudents)
{
    std::cout << "Entrer votre ID svp : ";
    std::string id;
    std::cin >> id;
    bool success = false;
    // std::cout << "Chemin courant : " << std::filesystem::current_path() << std::endl;
    if (std::ifstream idFile{"../../data/id.txt"}; idFile.is_open())
    {
        std::string line;
        while (std::getline(idFile >> std::ws, line))
        {
            if (id == line)
            {
                auto const &dataEleve = filesStudent(id);
                if (std::ifstream infoEleve{dataEleve}; infoEleve.is_open())
                {
                    int i = 0;
                    while (std::getline(infoEleve >> std::ws, line))
                    {
                        infoStudents[i] = line;
                        i++;
                    }
                    success = true;
                    infoEleve.close();
                }
                else
                {
                    std::cout << "Fichier inexistant !" << std::endl;
                }
            }
        }
        idFile.close();
    }
    else
    {
        std::cout << "Fichier non trouve !" << std::endl;
    }
    return success;
}

void try_again_connexion(std::array<std::string, 5> &infoStudents)
{
    bool try_again = true;
    while (try_again)
    {
        std::cout << "Aucun compte ne possede cette ID !" << std::endl;
        int decision;
        do
        {
            std::cout << "Options disponible : " << std::endl;
            std::cout << "1- Reessayer" << std::endl;
            std::cout << "2- Creer un compte" << std::endl;
            std::cout << "3- Quitter l'application" << std::endl;
            std::cout << "\nQuel est votre decision ? : ";
            std::cin >> decision;
            entryErrorForInt();
            if (decision == 1)
            {
                if (connexionStudent(infoStudents))
                {
                    try_again = false;
                }
            }else if (decision == 2)
            {
                try_again = false;
                Eleve newStudent = createStudent();
                menu(newStudent);
            }else if (decision == 3)
            {
                return;
            }
            else
            {
                std::cout << "Action impossible !" << std::endl;
            }
        }while (decision != 1 && decision != 2);
    }
}

void entryErrorForInt()
{
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Pas de lettres ! Uniquement des nombres !.\n";
    }
}