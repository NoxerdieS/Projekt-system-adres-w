#include <iostream>
#include <string>

using namespace std;

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

void LoadDatabase(Database* database) {
    database->nextPersonId = 0;
    database->people = new Person[2];
}

void SaveDatabase(Database* database) {
    // TODO
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
    Database* database = new Database();
    LoadDatabase(database);

    system("chcp 65001>>null");
    cout << "buh" << endl;
    return 0;
}
