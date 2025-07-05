#include <array>
#include "Classe.hpp"
#include "functionality.hpp"
#include "Eleve.hpp"

int main()
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

    return 0;
}