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

Matrix reflexive_closure(Matrix matrix);

Matrix symmetry_closure(Matrix matrix);

Matrix transitive_closure(Matrix matrix);

/**
 * @brief calculate A[i][:]*B[:][j]
 *
 * @param A
 * @param B
 * @param i
 * @param j
 * @return int
 */
int product(Matrix &A, Matrix &B, int i, int j);

/**
 * @brief calculate AB
 *
 * @param A
 * @param B
 * @return Matrix
 */
Matrix multi(Matrix &A, Matrix &B);

/**
 * @brief calculate A+B
 *
 * @param A
 * @param B
 * @return Matrix
 */
Matrix add(Matrix &A, Matrix &B);

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
            cout << "reflexivity closure:\n";
            Matrix rMatrix = reflexive_closure(a);
            print_matrix(rMatrix);
            cout << "symmetry closure:\n";
            Matrix sMatrix = symmetry_closure(a);
            print_matrix(sMatrix);
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

Matrix reflexive_closure(Matrix matrix) {
    for (int i = 0; i < matrix.size(); i++)
        matrix[i][i] = 1;
    return matrix;
}

Matrix symmetry_closure(Matrix matrix) {
    for (int i = 0; i < matrix.size(); i++)
        for (int j = i + 1; j < matrix[i].size(); j++)
            matrix[j][i] = matrix[i][j] = matrix[i][j] | matrix[j][i];
    return matrix;
}

Matrix transitive_closure(Matrix matrix) {
    Matrix sumMat = matrix;
    Matrix mulMat = matrix;
    for (int i = 0; i < matrix.size() - 1; i++) {
        mulMat = multi(mulMat, matrix);
        sumMat = add(sumMat, mulMat);
    }
    return sumMat;
}

int product(Matrix &A, Matrix &B, int i, int j) {
    int result = 0;
    for (int k = 0; k < A.size(); k++) {
        result = result || (A[i][k] && B[k][j]);
    }
    return result;
}

Matrix multi(Matrix &A, Matrix &B) {
    Matrix result;
    int size = A.size();
    for (int i = 0; i < size; i++) {
        vector< int > row;
        for (int j = 0; j < size; j++) {
            row.push_back(product(A, B, i, j));
        }
        result.push_back(row);
    }
    return result;
}

Matrix add(Matrix &A, Matrix &B) {
    Matrix result;
    int size = A.size();
    for (int i = 0; i < size; i++) {
        vector< int > row;
        for (int j = 0; j < size; j++) {
            row.push_back(A[i][j] || B[i][j]);
        }
        result.push_back(row);
    }
    return result;
}