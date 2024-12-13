#include <fstream>
#include <iostream>
#include <string>

using namespace std;

struct Person {
    int id;
    string name;
    string surname;
    string address;
    string email;
    string* hobbies = new string[20];
};

void AddPerson(string name, string surname, string address, string email, string* hobbies) {
    ifstream infile("database.txt");
    int lastID = 0;
    string line;

    if (infile.is_open()) {
        while (getline(infile, line)) {
            size_t pos = line.find(';');
            if (pos != string::npos) {
                lastID = stoi(line.substr(0, pos));
            }
        }
        infile.close();
    }

    int newID = lastID + 1;

    ofstream outfile("database.txt", ios::app);
    if (outfile.is_open()) {
        outfile << newID << ";"
                << name << ";"
                << surname << ";"
                << address << ";"
                << email << ";";

        outfile << "[";
        for (int i = 0; i < 20 && !hobbies[i].empty(); ++i) {
            outfile << hobbies[i];
            if (i < 19 && !hobbies[i + 1].empty()) {
                outfile << ", ";
            }
        }
        outfile << "]" << endl;

        cout << "Poprawnie dodano osobę o ID " << newID << " do bazy"<< endl;
    } else {
        cout << "Wystąpił problem z plikiem" << endl;
    }
    outfile.close();
}



void DeletePerson(int id) {
    ifstream infile("database.txt");
    ofstream outfile("temp.txt");
    string line;
    bool found = false;

    if (infile.is_open() && outfile.is_open()) {
        while (getline(infile, line)) {
            size_t pos = line.find(';');
            if (pos != string::npos) {
                int currentID = stoi(line.substr(0, pos));
                if (currentID == id) {
                    found = true;
                    cout << "Znaleziono rekord: " << line << endl;
                    cout << "Czy na pewno chcesz usunąć ten rekord? (t/n): ";
                    char choice;
                    cin >> choice;
                    if (choice == 'n' || choice == 'N') {
                        outfile << line << endl;
                        cout << "Rekord nie został usunięty." << endl;
                        continue;
                    } else if (choice == 't' || choice == 'T') {
                        cout << "Rekord został pomyślnie usunięty." << endl;
                        continue;
                    }
                }
            }
            outfile << line << endl;
        }
        infile.close();
        outfile.close();

        if (found) {
            remove("database.txt");
            rename("temp.txt", "database.txt");
        } else {
            cout << "Nie znaleziono rekordu o ID: " << id << endl;
            remove("temp.txt");
        }
    } else {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
    }
}

void EditPerson(int id) {
    ifstream infile("database.txt");
    ofstream outfile("temp.txt");
    string line;
    bool found = false;

    if (infile.is_open() && outfile.is_open()) {
        while (getline(infile, line)) {
            size_t pos = line.find(';');
            if (pos != string::npos) {
                int currentID = stoi(line.substr(0, pos));
                if (currentID == id) {
                    found = true;
                    cout << "Znaleziono rekord: " << line << endl;

                    string name, surname, address, email, hobbies;
                    cout << "Podaj nowe dane. Jeśli chcesz zachować istniejące, zostaw pole puste." << endl;

                    cout << "Nowe imię: ";
                    getline(cin, name);
                    cout << "Nowe nazwisko: ";
                    getline(cin, surname);
                    cout << "Nowy adres: ";
                    getline(cin, address);
                    cout << "Nowy email: ";
                    getline(cin, email);
                    cout << "Nowe zainteresowania (oddzielone przecinkami): ";
                    getline(cin, hobbies);

                    string updatedRecord = to_string(id) + ";";

                    size_t firstSemicolon = line.find(';');
                    size_t secondSemicolon = line.find(';', firstSemicolon + 1);
                    size_t thirdSemicolon = line.find(';', secondSemicolon + 1);
                    size_t fourthSemicolon = line.find(';', thirdSemicolon + 1);

                    updatedRecord += (name.empty() ? line.substr(firstSemicolon + 1, secondSemicolon - firstSemicolon - 1) : name) + ";";
                    updatedRecord += (surname.empty() ? line.substr(secondSemicolon + 1, thirdSemicolon - secondSemicolon - 1) : surname) + ";";
                    updatedRecord += (address.empty() ? line.substr(thirdSemicolon + 1, fourthSemicolon - thirdSemicolon - 1) : address) + ";";
                    updatedRecord += (email.empty() ? line.substr(fourthSemicolon + 1, line.find(';', fourthSemicolon + 1) - fourthSemicolon - 1) : email) + ";";
                    updatedRecord += (hobbies.empty() ? line.substr(line.find('[', fourthSemicolon) + 1, line.find(']') - line.find('[', fourthSemicolon) - 1) : hobbies);

                    outfile << updatedRecord << endl;
                    cout << "Rekord został zaktualizowany." << endl;
                    continue;
                }
            }
            outfile << line << endl;
        }
        infile.close();
        outfile.close();

        if (found) {
            remove("database.txt");
            rename("temp.txt", "database.txt");
        } else {
            cout << "Nie znaleziono rekordu o ID: " << id << endl;
            remove("temp.txt");
        }
    } else {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
    }
}


void DisplayDatabase() {
    ifstream file("database.txt", ios::in);
    if (file.is_open()) {
        string line;
        cout << "ID;Imię;Nazwisko;Adres;Email;Zainteresowania" << endl;
        cout << "------------------------------------------" << endl;
        while (getline(file, line)) {
            cout << line << endl;
        }
        file.close();
    } else {
        cout << "Wystąpił problem z plikiem" << endl;
    }
}

void SearchPerson() {
    ifstream infile("database.txt");
    string searchTerm, line;
    int choice;

    if (infile.is_open()) {
        cout << "Wybierz parametr wyszukiwania:" << endl;
        cout << "1. ID" << endl;
        cout << "2. Imię" << endl;
        cout << "3. Nazwisko" << endl;
        cout << "4. Email" << endl;
        cout << "5. Inne (adres lub zainteresowania)" << endl;
        cout << "Twój wybór: ";
        cin >> choice;
        cin.ignore();

        cout << "Podaj wartość do wyszukania: ";
        getline(cin, searchTerm);

        bool found = false;
        cout << "Wyniki wyszukiwania:" << endl;
        while (getline(infile, line)) {
            bool matches = false;

            switch (choice) {
                case 1: {
                    size_t pos = line.find(';');
                    if (pos != string::npos) {
                        string id = line.substr(0, pos);
                        matches = (id == searchTerm);
                    }
                    break;
                }
                case 2:
                case 3:
                case 4:
                case 5:
                    matches = (line.find(searchTerm) != string::npos);
                break;
                default:
                    cout << "Niepoprawny wybór." << endl;
                infile.close();
                return;
            }

            if (matches) {
                cout << line << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "Brak wyników dla podanego kryterium." << endl;
        }
        infile.close();
    } else {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
    }
}



int MenuDisplay() {
    int choice;
    do {
        cout << "Menu: "<< endl;
        cout << "1. Dodaj osobę do bazy" << endl;
        cout << "2. Usuń osobę z bazy" << endl;
        cout << "3. Edytuj osobę" << endl;
        cout << "4. Wyświetl bazę" << endl;
        cout << "5. Grupuj po zainteresowaniach" << endl;
        cout << "6. Wyszukaj osobę" << endl;
        cout << "7. Wyjdź" << endl;
        cout << "Wybór: ";
        cin >> choice;
        switch (choice) {
            case 1: {
                int id;
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
            case 2: {
                int id;
                cout<<"Podaj id osoby do usunięcia: ";
                cin >> id;
                DeletePerson(id);
                break;
            }
            case 3: {
                int id;
                cout<<"Podaj id osoby do edycji: ";
                cin >> id;
                EditPerson(id);
                break;
            }
            case 4:
                DisplayDatabase();
                break;
            case 5:
                //AutomaticGrouping()
                break;
            case 6: {
                SearchPerson();
                break;
            }

            case 7:
                return 0;
            default:
                cout << "Wybrano złą opcję" << endl;
        }
    }while (choice != 7);

}

int main() {
    system("chcp 65001>>null");
    MenuDisplay();
    return 0;
}