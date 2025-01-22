#include <iostream>
#include <fstream>

using namespace std;

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