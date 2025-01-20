#include <iostream>
#include <string>
#include <stdexcept>

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