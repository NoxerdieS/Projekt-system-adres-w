#include <iostream>
#include <string>

using namespace std;

struct Interest {
    int id;

    string name;
};

struct Person {
    int id;

    string name;
    string surname;
    string address;
    string email;
};

struct PersonHasInterests {
    int personId;
    int interestId;
};

struct PrimaryKeyMetadata {
    int lastPersonId;
    int lastInterestId;
};

struct Database {
    PrimaryKeyMetadata metadata;

    Interest[] interests;
    Person[] people;
    PersonHasInterests[] personHasInterests;
};

Database* LoadDatabase() {
    // TODO
}

void SaveDatabase(Database database) {
    // TODO
}

int main() {
    system("chcp 65001>>null");
    cout << "buh" << endl;
    return 0;
}
