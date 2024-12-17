#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

using namespace std;

struct Person {
    int id;
    string name;
    string surname;
    string address;
    string email;
    string* hobbies = new string[20];
};

void handleExit(const string& input) {
    if (input == "wyjdź") {
        throw runtime_error("Powrót do menu");
    }
}

string getInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    handleExit(input);
    return input;
}

void AddPerson() {
    ifstream infile("database.txt");
    int lastID = 0;
    string line;
    string name, surname, address, email;
    string* hobbies = new string[20];
    int x;
    cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu"<<endl;
    name = getInput("Podaj imię: ");
    surname = getInput("Podaj nazwisko: ");
    address = getInput("Podaj adres: ");
    email = getInput("Podaj email: ");
    cout << "Ile zainteresowań chcesz dodać: ";
    cin >> x;
    cin.ignore();

    for (int j = 0; j < x; j++) {
        hobbies[j] = getInput("Podaj zainteresowanie: ");
    }
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

        cout << "Poprawnie dodano osobę o ID " << newID << " do bazy" << endl;
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
                    string choice;
                    getline(cin, choice);
                    handleExit(choice);
                    if (choice == "n" || choice == "N") {
                        outfile << line << endl;
                        cout << "Rekord nie został usunięty." << endl;
                        continue;
                    } else if (choice == "t" || choice == "T") {
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
                    cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu"<<endl;
                    cout << "Podaj nowe dane. Jeśli chcesz zachować istniejące, zostaw pole puste." << endl;

                    try {
                        name = getInput("Nowe imię: ");
                        surname = getInput("Nowe nazwisko: ");
                        address = getInput("Nowy adres: ");
                        email = getInput("Nowy email: ");
                        hobbies = getInput("Nowe zainteresowania (oddzielone przecinkami): ");
                    } catch (runtime_error& e) {
                        return;
                    }

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

void GroupByHobbies() {
    ifstream infile("database.txt");
    const int MAX_HOBBIES = 100;
    const int MAX_PEOPLE = 100;

    string hobbies[MAX_HOBBIES];
    string groupedPeople[MAX_HOBBIES][MAX_PEOPLE];
    int hobbyCounts[MAX_HOBBIES] = {0};
    int hobbyIndex = 0;

    if (infile.is_open()) {
        string line;

        while (getline(infile, line)) {
            size_t idPos = line.find(';');
            size_t hobbyStart = line.find('[', idPos);
            size_t hobbyEnd = line.find(']', hobbyStart);

            if (hobbyStart != string::npos && hobbyEnd != string::npos) {
                string idName = line.substr(0, line.find(';', idPos + 1));
                string hobbiesList = line.substr(hobbyStart + 1, hobbyEnd - hobbyStart - 1);

                size_t start = 0, end;
                while ((end = hobbiesList.find(',', start)) != string::npos || start < hobbiesList.length()) {
                    string hobby = hobbiesList.substr(start, (end == string::npos ? hobbiesList.length() : end) - start);
                    hobby = hobby.substr(hobby.find_first_not_of(" "));

                    bool found = false;
                    int foundIndex = -1;
                    for (int i = 0; i < hobbyIndex; ++i) {
                        if (hobbies[i] == hobby) {
                            found = true;
                            foundIndex = i;
                            break;
                        }
                    }

                    if (!found) {
                        hobbies[hobbyIndex] = hobby;
                        foundIndex = hobbyIndex++;
                    }

                    groupedPeople[foundIndex][hobbyCounts[foundIndex]++] = idName;

                    if (end == string::npos) break;
                    start = end + 1;
                }
            }
        }
        infile.close();

        cout << "Grupowanie według zainteresowań:" << endl;
        for (int i = 0; i < hobbyIndex; ++i) {
            cout << hobbies[i] << ":" << endl;
            for (int j = 0; j < hobbyCounts[i]; ++j) {
                cout << "  - " << groupedPeople[i][j] << endl;
            }
        }
    } else {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
    }
}



void SearchPerson() {
    ifstream infile("database.txt");
    string searchTerm, line;
    int choice;

    if (infile.is_open()) {
        cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu"<<endl;
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
    cin.exceptions(ios::failbit);
    do {
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

        try {
            cin >> choice;
        } catch (const ios_base::failure& e) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "To nie jest liczba. Spróbuj ponownie." << endl;
            continue;
        }
        cin.ignore();

        try {
            switch (choice) {
                case 1: {
                    AddPerson();
                    break;
                }
                case 2: {
                    int id;
                    cout << "Podaj id osoby do usunięcia: ";
                    cin >> id;
                    cin.ignore();
                    DeletePerson(id);
                    break;
                }
                case 3: {
                    int id;
                    cout << "Podaj id osoby do edycji: ";
                    cin >> id;
                    cin.ignore();
                    EditPerson(id);
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
                    return 0;
                default:
                    cout << "Wybrano złą opcję" << endl;
            }
        } catch (runtime_error& e) {
            cout << "Powrót do menu" << endl;
        }
    } while (choice != 7);
}

int main() {
    system("chcp 65001>>null");
    MenuDisplay();
    return 0;
}