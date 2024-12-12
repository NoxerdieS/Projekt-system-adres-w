#include <fstream>
#include <iostream>
#include <string>

using namespace std;

int peopleCount = 0;

struct Person {
    int id;

    string name;
    string surname;
    string address;
    string email;

    string* hobbys = new string[100];
};

struct Database {
    int nextPersonId;
    Person* people = new Person[100];
};


void AddPerson(Database* database, Person person, int peopleCount) {
    database->people[database->peopleCount] = person;
    database->peopleCount++;
}

string[] SplitStringByDelimiter(string payload, char delimiter) {
    string[] splitted = new string[2];

    for (int i = 0; i < payload.length(); i++) {
        if (payload[i] == delimiter) {
            splitted[0] = payload.substr(0, i);
            payload = payload.substr(i + 1, payload.length() - i - 1);
            i = 0;
        }
    }

    return splitted;
}

void LoadDatabase(Database* database, int peopleCount) {
    ifstream file("database.csv");

    if (!file) {
        cerr << "Wystąpił błąd podczas odczytywania bazy danych." << endl;
        exit(1);
    }

    database->nextPersonId = 0;
    database->peopleCount = 0;
    database->people = new Person[2];

    string line;
    while (getline(file, line)) {
        string[] splittedLine = SplitStringByDelimiter(line, ';');

        if (splittedLine.length() != 6) {
            cerr << "Niepoprawny format wiersza w bazie danych: \"" << line << "\"" << endl;
            exit(1);
        }

        loadedPerson = new Person();
        loadedPerson->id = stoi(splittedLine[0]); // wywali błąd
        loadedPerson->name = splittedLine[1];
        loadedPerson->surname = splittedLine[2];
        loadedPerson->address = splittedLine[3];
        loadedPerson->hobbies = SplitStringByDelimiter(splittedLine[4], ',');

        AddPerson(database, loadedPerson);
    }

    // sprawdź czy ID nie są zdublowane
    for (int i = 0; i < sizeof(database->people); i++) {
        for (int j = 0; j < sizeof(database->people); j++) {
            if (j != 0) { // żeby nie sprawdzać tego samego ID
                if (database->people[i]->id == database->people[j]->id) {
                    cerr << "Zduplikowane ID w bazie danych: " << database->people[i]->id << endl;
                    exit(1);
                }
            }
        }
    }
}

void SaveDatabase(Database* database) {
    ofstream file("database.csv");

    if (!file) {
        cerr << "Wystąpił błąd podczas zapisywania bazy danych." << endl;
        exit(1);
    }

    for (int i = 0; i < sizeof(database->people); i++) {
        file << database->people[i]->id << ";";
        file << database->people[i]->name << ";";
        file << database->people[i]->surname << ";";
        file << database->people[i]->address << ";";
        for (int j = 0; j < sizeof(database->people[i]->hobbies); j++) {
            file << database->people[i]->hobbies[j] << ",";
        }
        file << endl;
    }
}

int MenuDisplay() {
    cout << "Menu: "<< endl;
    cout << "1. Dodaj osobę do bazy" << endl;
    cout << "2. Usuń osobę do bazy" << endl;
    cout << "3. Edytuj osobę" << endl;
    cout << "4. Wyświetl bazę" << endl;
    cout << "5. Grupuj po zainteresowaniach" << endl;
    cout << "6. Wyjdź" << endl;
    cout << "Wybór: ";
    int choice;
    cin >> choice;
    switch (choice) {
        case 1:
            //AddPerson()
        case 2:
            //DeletePerson()
        case 3:
            //EditPerson()
        case 4:
            //DisplayDatabase()
        case 5:
            //AutomaticGrouping()
        case 6:
            return 0;
        default:
            cout << "Wybrano złą opcję" << endl;
    }
}

int main() {
    system("chcp 65001>>null");
    Database* database = new Database();
    LoadDatabase(database, peopleCount);
    return 0;
}
