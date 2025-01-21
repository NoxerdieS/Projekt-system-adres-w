#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
// #include "validationFunctions.cpp"

using namespace std;

struct Person {
    int id;
    string name;
    string surname;
    string address;
    string email;
    string* hobbies = new string[20];
};

void handleExit(const string& input);
string getInput(const string& prompt);
int getIntInput(const string& prompt);

void GetPersonId(int &id) {
    cout << "Podaj id osoby: ";
    cin >> id;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(2000000, '\n');
        throw runtime_error("Nieprawidłowy format ID. Musi to być liczba.");
    }
    cin.ignore(2000000, '\n');
}

Person generatePerson() {
    Person person;
    person.hobbies = new string[20];

    cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu" << endl;
    cin.ignore(20000, '\n');

    person.name = getInput("Podaj imię: ");
    person.surname = getInput("Podaj nazwisko: ");
    person.address = getInput("Podaj adres: ");
    person.email = getInput("Podaj email: ");

    int hobbyCount;
    while (!cin.fail()) {
        try {
            hobbyCount = getIntInput("Ile zainteresowań chcesz dodać (max 20): ");
            if (hobbyCount >= 0 && hobbyCount <= 20) {
                break;
            }
            cout << "Podano złą wartość" << endl;
        }catch(exception &err) {
            if (string(err.what()) != "stoi") {
                throw runtime_error("Powrót do menu");
            }
            cout << "Podano złą wartość" << endl;
        }
    }

    for (int i = 0; i < hobbyCount; i++) {
        person.hobbies[i] = getInput("Podaj zainteresowanie: ");
    }

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
    } else {
        throw runtime_error("Powrót do menu, nie można otworzyć pliku");
    }

    person.id = lastID + 1;

    return person;
}

void AddPerson() {
    try {
        Person person = generatePerson();

        ofstream outfile("database.txt", ios::app);
        if (outfile.is_open()) {
            outfile << person.id << ";"
                    << person.name << ";"
                    << person.surname << ";"
                    << person.address << ";"
                    << person.email << ";[";

            for (int i = 0; i < 20 && !person.hobbies[i].empty(); ++i) {
                outfile << person.hobbies[i];
                if (i < 19 && !person.hobbies[i + 1].empty()) {
                    outfile << ", ";
                }
            }
            outfile << "]" << endl;

            cout << "Poprawnie dodano osobę o ID " << person.id << " do bazy" << endl;
            outfile.close();
        } else {
            throw runtime_error("Wystąpił problem z zapisem do pliku.");
        }

        delete[] person.hobbies;

    } catch (runtime_error& e) {
        cout << "Operacja została przerwana. " << endl;
    }
}


void DeletePerson() {
    ifstream infile("database.txt");
    ofstream outfile("temp.txt");
    string line;
    bool found = false;
    int id;
    GetPersonId(id);

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

void updatePersonData(Person& person) {
    try {
        string input;

        input = getInput("Nowe imię: ");
        if (!input.empty()) person.name = input;

        input = getInput("Nowe nazwisko: ");
        if (!input.empty()) person.surname = input;

        input = getInput("Nowy adres: ");
        if (!input.empty()) person.address = input;

        input = getInput("Nowy email: ");
        if (!input.empty()) person.email = input;

        cout << "Nowe zainteresowania (oddzielone przecinkami, max 20): ";
        getline(cin, input);

        if (!input.empty()) {
            size_t hobbyStart = 0, hobbyEnd;
            int hobbyIndex = 0;
            while ((hobbyEnd = input.find(',', hobbyStart)) != string::npos && hobbyIndex < 20) {
                string hobby = input.substr(hobbyStart, hobbyEnd - hobbyStart);
                hobby.erase(0, hobby.find_first_not_of(" "));
                hobby.erase(hobby.find_last_not_of(" ") + 1);

                person.hobbies[hobbyIndex++] = hobby;
                hobbyStart = hobbyEnd + 1;
            }
            if (hobbyStart < input.length()) {
                string hobby = input.substr(hobbyStart);
                hobby.erase(0, hobby.find_first_not_of(" "));
                hobby.erase(hobby.find_last_not_of(" ") + 1);
                person.hobbies[hobbyIndex++] = hobby;
            }

            for (int i = hobbyIndex; i < 20; ++i) {
                person.hobbies[i].clear();
            }
        }
        cout << "Rekord został zaktualizowany." << endl;
    } catch (runtime_error&) {
        cout << "Przerwano edycję danych." << endl;
    }
}


Person parsePerson(const string& line) {
    Person person;
    person.hobbies = new string[20];

    size_t start = 0;
    size_t end = line.find(';');

    person.id = stoi(line.substr(start, end - start));

    start = end + 1;
    end = line.find(';', start);
    person.name = line.substr(start, end - start);

    start = end + 1;
    end = line.find(';', start);
    person.surname = line.substr(start, end - start);

    start = end + 1;
    end = line.find(';', start);
    person.address = line.substr(start, end - start);

    start = end + 1;
    end = line.find(';', start);
    person.email = line.substr(start, end - start);

    start = line.find('[', end) + 1;
    end = line.find(']', start);
    string hobbies = line.substr(start, end - start);
    hobbies += ',';

    size_t hobbyStart = 0, hobbyEnd;
    int hobbyIndex = 0;

    while ((hobbyEnd = hobbies.find(',', hobbyStart)) != string::npos && hobbyIndex < 20) {
        string hobby = hobbies.substr(hobbyStart, hobbyEnd - hobbyStart);
        hobby.erase(0, hobby.find_first_not_of(" "));
        hobby.erase(hobby.find_last_not_of(" ") + 1);
        person.hobbies[hobbyIndex++] = hobby;
        hobbyStart = hobbyEnd + 1;
    }

    return person;
}


void EditPerson() {
    ifstream infile("database.txt");
    ofstream outfile("temp.txt");
    string line;
    bool found = false;

    int id;
    GetPersonId(id);

    cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu" << endl;
    if (infile.is_open() && outfile.is_open()) {
        while (getline(infile, line)) {
            size_t pos = line.find(';');
            if (pos != string::npos) {
                int currentID = stoi(line.substr(0, pos));
                if (currentID == id) {
                    found = true;

                    Person person = parsePerson(line);
                    cout << "Znaleziono rekord: " << line << endl;
                    cout << "Wprowadź nowe dane (pozostaw puste, aby zachować istniejące)." << endl;

                    updatePersonData(person);

                    outfile << person.id << ";"
                            << person.name << ";"
                            << person.surname << ";"
                            << person.address << ";"
                            << person.email << ";[";

                    for (int i = 0; i < 20 && !person.hobbies[i].empty(); ++i) {
                        outfile << person.hobbies[i];
                        if (i < 19 && !person.hobbies[i + 1].empty()) {
                            outfile << ", ";
                        }
                    }
                    outfile << "]" << endl;


                    delete[] person.hobbies;
                    continue;
                }
                outfile << line << endl;
            }
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


void ParseLine(const string& line, string hobbies[], string groupedPeople[][100], int hobbyCounts[], int& hobbyIndex, int maxPeople) {
    size_t idPos = line.find(';');
    size_t namePos = line.find(';', idPos + 1);
    size_t surnamePos = line.find(';', namePos + 1);
    size_t hobbyStart = line.find('[', idPos);
    size_t hobbyEnd = line.find(']', hobbyStart);

    if (hobbyStart != string::npos && hobbyEnd != string::npos) {
        string idName = line.substr(0, surnamePos + 1);
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

            if (hobbyCounts[foundIndex] < maxPeople) {
                groupedPeople[foundIndex][hobbyCounts[foundIndex]++] = idName;
            }

            if (end == string::npos) break;
            start = end + 1;
        }
    }
}

void DisplayGroupedHobbies(const string hobbies[], const string groupedPeople[][100], const int hobbyCounts[], int hobbyIndex) {
    cout << "Grupowanie według zainteresowań:" << endl;
    for (int i = 0; i < hobbyIndex; ++i) {
        cout << hobbies[i] << ":" << endl;
        for (int j = 0; j < hobbyCounts[i]; ++j) {
            cout << "  - " << groupedPeople[i][j] << endl;
        }
    }
}

void GroupByHobbies() {
    const int maxHobbies = 100;
    const int maxPeople = 100;

    string hobbies[maxHobbies];
    string groupedPeople[maxHobbies][maxPeople];
    int hobbyCounts[maxHobbies] = {0};
    int hobbyIndex = 0;

    ifstream infile("database.txt");

    if (infile.is_open()) {
        string line;
        while (getline(infile, line)) {
            ParseLine(line, hobbies, groupedPeople, hobbyCounts, hobbyIndex, maxPeople);
        }
        infile.close();
        DisplayGroupedHobbies(hobbies, groupedPeople, hobbyCounts, hobbyIndex);
    } else {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
    }
}


bool ProcessLine(const string& line, int choice, const string& searchTerm) {
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
        throw runtime_error("Niepoprawny wybór");
    }

    return matches;
}
void SearchPerson() {
    ifstream infile("database.txt");
    if (!infile.is_open()) {
        cout << "Wystąpił problem z otwarciem pliku." << endl;
        return;
    }

    try {
        cout << "Możesz wpisać \"wyjdź\" w każdym momencie, aby wrócić do menu" << endl;
        cin.sync();
        int choice = getIntInput("Wybierz parametr wyszukiwania:\n"
                                   "1. ID\n"
                                   "2. Imię\n"
                                   "3. Nazwisko\n"
                                   "4. Email\n"
                                   "5. Inne (adres lub zainteresowania)\n"
                                   "Twój wybór: ");

        string searchTerm = getInput("Podaj wartość do wyszukania: ");
        cout << endl;

        bool found = false;
        cout << "Wyniki wyszukiwania:" << endl;

        string line;
        while (getline(infile, line)) {
            if (ProcessLine(line, choice, searchTerm)) {
                cout << line << endl;
                found = true;
            }
        }

        if (!found) {
            cout << "Brak wyników dla podanego kryterium." << endl;
        }

        infile.close();
    } catch(exception &err) {
        if (string(err.what()) != "stoi") {
            throw runtime_error("Powrót do menu");
        }
        cout << "Podano złą wartość" << endl;
    }
}

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
            cout << "Powrót do menu" << endl;
        }
    } while (choice != 7);
    return 0;
}