#include <iostream>
#include <string>
using namespace std;
bool conjunction_(int p, int q) {
    return p & q;
}
bool disjunction_(int p, int q) {
    return p | q;
}
bool condition_(int p, int q) {
    return !p | q;
}
bool bicondition_(int p, int q) {
    return (p & q) | (!p & !q);
}
void string_repeat(string str, int repeat, bool end = true) {
    for (int i = 0; i < repeat; i++)
        cout << str;
    if (end)
        cout << endl;
}
void print_welcome(string style = "*") {
    string_repeat(style, 20);
    string_repeat(style, 2, false);
    string_repeat(" ", 4, false);
    cout << "welcome!";
    string_repeat(" ", 4, false);
    string_repeat(style, 2);
    string_repeat(style, 20);
}
bool value_validation(string value, string name) {
    if (value != "0" && value != "1") {
        cout << "invalid value for " << name << ", please retry" << endl;
        return false;
    }
    return true;
}
string get_value(string name) {
    cout << "please input " << name << "(0 or 1), end by enter, q to quit" << endl;
    string tmp;
    cin >> tmp;
    return tmp;
}

int main() {
    print_welcome();
    while (true) {
        string p, q;
        do {
            p = get_value("p");
            if (p == "q") {
                cout << "quit" << endl;
                return 0;
            }
        } while (!value_validation(p, "p"));
        do {
            q = get_value("q");
            if (q == "q") {
                cout << "quit" << endl;
                return 0;
            }
        } while (!value_validation(q, "q"));
        int intP, intQ = 0;
        if (p != "0")
            intP = 1;
        if (q != "0")
            intQ = 1;
        cout << "conjunction:\n\t" << intP << "/\\" << intQ << " " << conjunction_(intP, intQ) << endl;
        cout << "disjunction:\n\t" << intP << "\\/" << intQ << " " << disjunction_(intP, intQ) << endl;
        cout << "condition:\n\t" << intP << "->" << intQ << " " << condition_(intP, intQ) << endl;
        cout << "bicondition:\n\t" << intP << "<->" << intQ << " " << bicondition_(intP, intQ) << endl;
        cout << "any to continue, q to quit" << endl;
        string continueConfiguration;
        cin >> continueConfiguration;
        if (continueConfiguration == "q") {
            cout << "quit" << endl;
            return 0;
        }
    }
    return 0;
}