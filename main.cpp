#include <iostream>
#include <string>

using namespace std;

struct Person {
    int id;

    string name;
    string surname;
    string address;
    string email;

    string[] hobbies;
};

struct Database {
    int nextPersonId;
    Person[] people;
};

void LoadDatabase(Database* database) {
    database->nextPersonId = 0;
    database->people = new Person[2];
}

void SaveDatabase(Database* database) {
    // TODO
}

int main() {
    Database database = new Database();
    LoadDatabase(&database);

    system("chcp 65001>>null");
    cout << "buh" << endl;
    return 0;
}
