#include <iostream>
#include <vector>

using namespace std;

string insertStr(const string &target, string &dest) {
}

int main() {
    string str;
    cin >> str;
    for (int i = 0; i < str.size(); i++) {
        if (str[i] >= 48 && str[i] <= 57) {
            str.erase(i, 1);
            str.insert(i, "number");
        }
    }

    cout << str;
}