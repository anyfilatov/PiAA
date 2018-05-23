#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

vector<int> prefix(string input);

string join(vector<int> entries, const string &delimiter);

vector<int> positions(const string &text, const string &pattern);

int main() {
    string pattern, text;
    getline(cin, pattern);
    getline(cin, text);
    auto entries = positions(text, pattern);
    if (!entries.empty()) cout << join(entries, ",");
    else cout << "-1";
    return 0;
}

/** Ищет вхождения строки `pattert` в строке `text` */
vector<int> positions(const string &text, const string &pattern) {
    string P = pattern + "0" + text;
    vector<int> positions;
    auto entries = prefix(P);
    for (int i = 0; i < P.length(); i++) {
        if (entries[i] == pattern.length()) {
            positions.push_back(i - 2 * pattern.length());
        }
    }
    return positions;
}

/** Префикс-функция */
vector<int> prefix(const string input) {
    int size = input.length();
    vector<int> prefix;
    for (int i = 0; i < size; i++) prefix.push_back(0);
    for (int i = 1; i < size; i++) {
        int j = prefix[i - 1];
        while ((j > 0) && (input[i] != input[j])) {
            j = prefix[j - 1];
        }
        if (input[i] == input[j]) {
            j++;
        }
        prefix[i] = j;
    }
    return prefix;
}

/** Объединяет значения вектора в строке с заданным разделителем */
string join(vector<int> entries, const string &delimiter) {
    string joint;
    for (int &position : entries) {
        if (&position != &entries[0]) joint += delimiter;
        joint += to_string(position);
    }
    return joint;
}