#include <iostream>

using namespace std;

int main() {
    int k;
    string str;
    cin >> k >> str;

    char ch = str[0], next_ch;
    for (int cnt = 0, i = 0, next_i; cnt < str.size(); cnt++) {
        next_i = (i + k) % str.size();
        next_ch = str[next_i];

        str[next_i] = ch;
        ch = next_ch;
        i = next_i;
    }
    cout << str << endl;
}