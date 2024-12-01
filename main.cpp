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

Database LoadDatabase() {

}

void SaveDatabase(Database database) {
    // TODO
}

int main() {
    system("chcp 65001>>null");
    cout << "buh" << endl;
    return 0;
}
