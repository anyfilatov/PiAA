#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

using namespace std;

int *prefix(const string &input);

int shift(const string &text, string &pattern);

int main() {
    string s1, s2;
    getline(cin, s1);
    getline(cin, s2);
    cout << shift(s1, s2);
}

/**
 * Возвращает порядок циклического сдвига строки.
 * Например, ля строк `abcdef` и `defabc` возвращает 3.
 * Если строчки не являются циклическим сдвигом друг друга, возварщает -1
 */
int shift(const string &text, string &pattern) {
    int size = pattern.length();
    pattern += "@" + text + text;
    auto entries = prefix(pattern);
    for (int i = 0; i < pattern.length(); i++) {
        if (entries[i] == size) {
            return i - 2 * size;
        }
    }
    return -1;
}

/** Префикс-функция */
int *prefix(const string &input) {
    size_t n = input.length();
    auto sub_size = new int[n];
    for (int i = 0; i < n; i++) {
        sub_size[i] = 0;
    }
    for (size_t i = 1; i < n; i++) {
        size_t j = sub_size[i - 1];
        while ((j > 0) && (input[i] != input[j])) {
            j = sub_size[j - 1];
        }
        if (input[i] == input[j]) {
            j++;
        }
        sub_size[i] = j;
    }
    return sub_size;
}