#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

struct Node {  // define node structure
    int weight;
    Node *l;
    Node *r;

    Node(int w) : weight(w), l(nullptr), r(nullptr) {}
};

struct Compare {  // define comparison structure
    bool operator()(Node *a, Node *b) { return a->weight > b->weight; }
};

Node *build_HFM_tree(vector< int > &freq) {  // build tree
    priority_queue< Node *, vector< Node * >, Compare > pq;
    for (int i = 0; i < freq.size(); i++) {  // build priority queue
        Node *node = new Node(freq[i]);
        pq.push(node);
    }

    while (pq.size() > 1) {  // build HFM tree
        Node *left = pq.top();
        pq.pop();
        Node *right = pq.top();
        pq.pop();

        Node *parent = new Node(left->weight + right->weight);
        parent->l = left;
        parent->r = right;
        pq.push(parent);
    }
    return pq.top();
}

void gen_code(Node *root, unordered_map< int, string > &codes, string code = "0") {
    if (root == nullptr)
        return;
    if (root->l == nullptr && root->r == nullptr)
        codes[root->weight] = code;
    gen_code(root->l, codes, code + "0");
    gen_code(root->r, codes, code + "1");
}

int main() {
    while (true) {
        vector< int > freq;
        vector< string > invalid;
        vector< int > invalidIdx;
        cout << "Input frequency(Qq to quit)\n";

        string input;
        getline(cin, input);
        if (input == "Qq") {
            cout << "thanks for using\n";
            break;
        }
        istringstream ss(input);
        string token;
        int idx = 0;
        while (ss >> token) {
            bool isInteger = true;
            for (int i = 0; i < token.size(); i++) {
                if (!isdigit(token[i])) {
                    invalid.push_back(token);
                    invalidIdx.push_back(idx);
                    isInteger = false;
                    break;
                }
            }
            if (isInteger) {
                int number = stoi(token);
                if (number > 0) {
                    freq.push_back(number);
                }
            }
            idx++;
        }
        if (freq.size() == 0) {
            cout << "none of the inputs is valid, please retry\n";
            continue;
        }
        Node *tree = build_HFM_tree(freq);
        unordered_map< int, string > codes;
        gen_code(tree, codes);
        cout << "you input invalid frequencies, we have ignored them\n";
        for (int i = 0; i < invalid.size(); i++) {
            printf("%s[%d]\t", invalid[i].c_str(), invalidIdx[i] + 1);
        }
        cout << "\nHuffman Codes:\n";
        for (unordered_map< int, string >::iterator it = codes.begin(); it != codes.end(); ++it) {
            pair< int, string > it_pair = *it;
            cout << it_pair.first << ":\t" << it_pair.second << endl;
        }
    }
    return 0;
}