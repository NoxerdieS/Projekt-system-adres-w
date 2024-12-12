#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct Person {
    string name;
    string surname;
    string address;
    string email;
    string* hobbies = new string[20];
};

void AddPerson(string name, string surname, string address, string email, string* hobbies) {
    Person person;
    person.name = name;
    person.surname = surname;
    person.address = address;
    person.email = email;
    person.hobbies = hobbies;

    ofstream outfile("database.txt", ios::app);
    if (outfile.is_open()) {
        outfile << person.name << ";"
                << person.surname << ";"
                << person.address << ";"
                << person.email << ";";

        outfile << "[";
        for (int i = 0; i < 20 && !person.hobbies[i].empty(); ++i) {
            outfile << person.hobbies[i];
            if (i < 19 && !person.hobbies[i + 1].empty()) {
                outfile << ", ";
            }
        }
        outfile << "]" << endl;

        cout << "Poprawnie dodano użytkownika do bazy" << endl;
    } else {
        cout << "Wystąpił problem z plikiem" << endl;
    }
    outfile.close();
}

void DisplayDatabase() {
    cout << endl;
    ifstream file("database.txt", ios::in);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Wystąpił problem z plikiem" << endl;
    }
    cout << endl;
}

int MenuDisplay() {
    int choice;
    do {
        cout << "Menu: "<< endl;
        cout << "1. Dodaj osobę do bazy" << endl;
        cout << "2. Usuń osobę do bazy" << endl;
        cout << "3. Edytuj osobę" << endl;
        cout << "4. Wyświetl bazę" << endl;
        cout << "5. Grupuj po zainteresowaniach" << endl;
        cout << "6. Wyjdź" << endl;
        cout << "Wybór: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                string name;
                string surname;
                string address;
                string email;
                string* hobbies = new string[20];
                int n;
                int x;
                cout << "Ilu użytkowników chcesz dodać: ";
                cin >> n;
                for (int i = 0; i < n; i++) {
                    cout << "Podaj imię użytkownika " << i + 1 << ": ";
                    cin >> name;
                    cout << "Podaj nazwisko użytkownika " << i + 1 << ": ";
                    cin >> surname;
                    cout << "Podaj adres użytkownika " << i + 1 << ": ";
                    cin >> address;
                    cout << "Podaj adres email użytkownika " << i + 1 << ": ";
                    cin >> email;
                    cout << "Ile zainteresowań chcesz dodać: ";
                    cin >> x;
                    for (int j = 0; j < x; j++) {
                        cout << "Podaj zainteresowanie " << j + 1 << ": ";
                        cin >> hobbies[j];
                    }
                    AddPerson(name, surname, address, email, hobbies);
                    cout << endl;
                }
                delete[] hobbies;
                break;
            }
            case 2:
                //DeletePerson()
                break;
            case 3:
                //EditPerson()
                break;
            case 4:
                DisplayDatabase();
                break;
            case 5:
                //AutomaticGrouping()
                break;
            case 6:
                return 0;
            default:
                cout << "Wybrano złą opcję" << endl;
        }
    }while (choice != 6);

}

int main() {
    system("chcp 65001>>null");
    MenuDisplay();
    return 0;
}


// struct Database {
//     int nextPersonId;
//     Person* people = new Person[100];
//     int peopleCount = 0;
// };
//
//
// void AddPerson(Database* database, Person person) {
//     database->people[database->peopleCount] = person;
//     database->peopleCount++;
// }
//
// string* SplitStringByDelimiter(string payload, char delimiter) {
//     string* splitted = new string[2];
//
//     for (int i = 0; i < payload.length(); i++) {
//         if (payload[i] == delimiter) {
//             splitted[0] = payload.substr(0, i);
//             payload = payload.substr(i + 1, payload.length() - i - 1);
//             i = 0;
//         }
//     }
//
//     return splitted;
// }
//
// void LoadDatabase(Database* database, int peopleCount) {
//     ifstream file("database.csv");
//
//     if (!file) {
//         cerr << "Wystąpił błąd podczas odczytywania bazy danych." << endl;
//         exit(1);
//     }
//
//     database->nextPersonId = 0;
//     database->peopleCount = 0;
//     database->people = new Person[2];
//
//     string line;
//     while (getline(file, line)) {
//         string* splittedLine = SplitStringByDelimiter(line, ';');
//
//         if (splittedLine->size() != 6) {
//             cerr << "Niepoprawny format wiersza w bazie danych: \"" << line << "\"" << endl;
//             exit(1);
//         }
//
//         Person* loadedPerson = new Person();
//         loadedPerson->id = stoi(splittedLine[0]); // wywali błąd
//         loadedPerson->name = splittedLine[1];
//         loadedPerson->surname = splittedLine[2];
//         loadedPerson->address = splittedLine[3];
//         loadedPerson->hobbys = SplitStringByDelimiter(splittedLine[4], ',');
//
//         AddPerson(database, loadedPerson);
//     }
//
//     // sprawdź czy ID nie są zdublowane
//     for (int i = 0; i < sizeof(database->people); i++) {
//         for (int j = 0; j < sizeof(database->people); j++) {
//             if (j != 0) { // żeby nie sprawdzać tego samego ID
//                 if (database->people[i]->id == database->people[j]->id) {
//                     cerr << "Zduplikowane ID w bazie danych: " << database->people[i]->id << endl;
//                     exit(1);
//                 }
//             }
//         }
//     }
// }
//
// void SaveDatabase(Database* database) {
//     ofstream file("database.csv");
//
//     if (!file) {
//         cerr << "Wystąpił błąd podczas zapisywania bazy danych." << endl;
//         exit(1);
//     }
//
//     for (int i = 0; i < sizeof(database->people); i++) {
//         file << database->people[i]->id << ";";
//         file << database->people[i]->name << ";";
//         file << database->people[i]->surname << ";";
//         file << database->people[i]->address << ";";
//         for (int j = 0; j < sizeof(database->people[i]->hobbies); j++) {
//             file << database->people[i]->hobbies[j] << ",";
//         }
//         file << endl;
//     }
// }
