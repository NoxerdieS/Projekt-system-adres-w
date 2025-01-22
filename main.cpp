#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
#include "database_operations.cpp"
#include "hobby_operations.cpp"

using namespace std;

void MenuDisplay(int &choice) {
    cout << endl;
    cout << "Menu: " << endl;
    cout << "1. Dodaj osobę do bazy" << endl;
    cout << "2. Usuń osobę z bazy" << endl;
    cout << "3. Edytuj osobę" << endl;
    cout << "4. Wyświetl bazę" << endl;
    cout << "5. Grupuj po zainteresowaniach" << endl;
    cout << "6. Wyszukaj osobę" << endl;
    cout << "7. Wyjdź" << endl;
    cout << "Wybór: ";

    cin >> choice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(2000000, '\n');
        cout << "To nie jest liczba. Spróbuj ponownie." << endl;
        choice = 0;
    }
}

int main() {
    system("chcp 65001>>null");
    int choice;
    do {
        MenuDisplay(choice);
        try {
            switch (choice) {
                case 1: {
                    AddPerson();
                    break;
                }
                case 2: {
                    DeletePerson();
                    break;
                }
                case 3: {
                    EditPerson();
                    break;
                }
                case 4:
                    DisplayDatabase();
                    break;
                case 5:
                    GroupByHobbies();
                    break;
                case 6: {
                    SearchPerson();
                    break;
                }
                case 7:
                    continue;
                default:
                    cout << "Wybrano złą opcję" << endl;
            }
        } catch (runtime_error& e) {
            cout << "Następuje powrót do menu" << endl;
        }
    } while (choice != 7);
    return 0;
}