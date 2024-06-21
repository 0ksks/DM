#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

typedef vector< vector< int > > Matrix;

/**
 * @brief Get the matrix object
 *
 * @return Matrix
 */
Matrix get_matrix();

/**
 * @brief Print the matrix object
 *
 * @param Matrix
 */
void print_matrix(Matrix &matrix);

/**
 * @brief check the legality of the matrix
 *
 * @param matrix
 * @return true
 * @return false
 */
bool validation(Matrix &matrix);

Matrix transitive_closure(Matrix matrix);

int main() {
    while (true) {
        cout << "any to start, q to quit\n";
        string expression;
        cin >> expression;
        if (expression == "q") {
            cout << "thanks for using\n";
            break;
        }
        Matrix a;
        a = get_matrix();
        if (validation(a)) {  // if valid, print the result
            cout << "transitivity closure:\n";
            Matrix tMatrix = transitive_closure(a);
            print_matrix(tMatrix);
        } else {  // else raise exception
            cout << "invalid input, please check\n";
        }
    }
    return 0;
}

Matrix get_matrix() {
    Matrix matrix;
    string line;
    cout << "Enter the matrix elements (separate elements with spaces, and rows with newlines). Enter EOF to finish:\n";
    while (getline(cin, line))  // split the stream by spaces and newlines
    {
        if (line == "EOF")
            break;
        istringstream iss(line);
        vector< int > row;
        int num;
        while (iss >> num)
            row.push_back(num);
        matrix.push_back(row);
    }
    matrix.erase(matrix.begin());
    return matrix;
}

void print_matrix(Matrix &matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix[i].size(); j++) {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

bool validation(Matrix &matrix) {
    int rows = matrix.size();
    for (int row = 0; row < rows; row++) {
        if (matrix[row].size() != rows) {
            return false;
        }
        for (int col = 0; col < matrix[row].size(); col++) {
            if (matrix[row][col] != 0 && matrix[row][col] != 1) {
                printf("invalid input in (%d,%d) %d\n", row, col, matrix[row][col]);
                return false;
            }
        }
    }
    return true;
}

Matrix transitive_closure(Matrix matrix) {
    for (int k = 0; k < matrix.size(); k++)
        for (int i = 0; i < matrix.size(); i++)
            for (int j = 0; j < matrix.size(); j++)
                matrix[i][j] = matrix[i][j] || (matrix[i][k] && matrix[k][j]);
    return matrix;
}