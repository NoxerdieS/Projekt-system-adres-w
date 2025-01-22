#include <iostream>

using namespace std;

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

int getIntInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    handleExit(input);
    return stoi(input);
}

void GetPersonId(int &id) {
    cout << "Podaj id osoby: ";
    cin >> id;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(2000000, '\n');
        cout << "Nieprawidłowy format ID. Musi to być liczba całkowita." << endl;
        throw runtime_error("Nieprawidłowy format ID. Musi to być liczba.");
    }
    cin.ignore(2000000, '\n');
}