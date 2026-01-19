#include "matrix_utils.h"
#include <iostream>
#include <algorithm>
#include <cctype>

using namespace std;

// Modular inverse using brute force
int MatrixUtils::modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) {
            return x;
        }
    }
    throw runtime_error("Modular inverse does not exist");
}

// Determinant for 1x1, 2x2, and 3x3 matrices
int MatrixUtils::determinant(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    
    if (n == 1) {
        return matrix[0][0];
    }
    
    if (n == 2) {
        int det = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        return ((det % 26) + 26) % 26;
    }
    
    if (n == 3) {
        int det = 
            matrix[0][0] * (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]) -
            matrix[0][1] * (matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]) +
            matrix[0][2] * (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
        
        return ((det % 26) + 26) % 26;
    }
    
    throw runtime_error("Matrix size not supported (only 1x1, 2x2, 3x3)");
}

// Adjugate matrix calculation
vector<vector<int>> MatrixUtils::adjugate(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    vector<vector<int>> adj(n, vector<int>(n, 0));
    
    if (n == 1) {
        adj[0][0] = 1;
        return adj;
    }
    
    if (n == 2) {
        adj[0][0] = matrix[1][1];
        adj[0][1] = -matrix[0][1];
        adj[1][0] = -matrix[1][0];
        adj[1][1] = matrix[0][0];
    } 
    else if (n == 3) {
        adj[0][0] = matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1];
        adj[0][1] = matrix[0][2] * matrix[2][1] - matrix[0][1] * matrix[2][2];
        adj[0][2] = matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1];
        
        adj[1][0] = matrix[1][2] * matrix[2][0] - matrix[1][0] * matrix[2][2];
        adj[1][1] = matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0];
        adj[1][2] = matrix[0][2] * matrix[1][0] - matrix[0][0] * matrix[1][2];
        
        adj[2][0] = matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0];
        adj[2][1] = matrix[0][1] * matrix[2][0] - matrix[0][0] * matrix[2][1];
        adj[2][2] = matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    // Apply modulo 26 to all elements
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adj[i][j] = ((adj[i][j] % 26) + 26) % 26;
        }
    }
    
    return adj;
}

// Inverse matrix calculation
vector<vector<int>> MatrixUtils::inverseMatrix(const vector<vector<int>>& matrix) {
    int det = determinant(matrix);
    
    if (det == 0) {
        throw runtime_error("Matrix is not invertible (determinant is 0)");
    }
    
    int detInv = modInverse(det, 26);
    vector<vector<int>> adj = adjugate(matrix);
    
    int n = matrix.size();
    vector<vector<int>> inverse(n, vector<int>(n, 0));
    
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            inverse[i][j] = (adj[i][j] * detInv) % 26;
            if (inverse[i][j] < 0) inverse[i][j] += 26;
        }
    }
    
    return inverse;
}

// Matrix-vector multiplication modulo mod
vector<int> MatrixUtils::multiplyMatrixVector(const vector<vector<int>>& matrix, 
                                              const vector<int>& vec, int mod) {
    int n = matrix.size();
    vector<int> result(n, 0);
    
    for (int i = 0; i < n; i++) {
        int sum = 0;
        for (int j = 0; j < n; j++) {
            sum += matrix[i][j] * vec[j];
        }
        result[i] = ((sum % mod) + mod) % mod;
    }
    
    return result;
}

// String to vector conversion (A=0, B=1, ..., Z=25)
// Now includes space as a special character (value 26, not encrypted)
vector<int> MatrixUtils::stringToVector(const string& str, bool preserveSpaces) {
    vector<int> vec;
    vec.reserve(str.length());
    
    for (char c : str) {
        if (c == ' ' && preserveSpaces) {
            vec.push_back(26);  // Special code for space
        } else if (isalpha(c)) {
            vec.push_back(toupper(c) - 'A');
        } else if (preserveSpaces) {
            // Ignore other non-alphabetic characters
            continue;
        } else {
            throw runtime_error("Invalid character in message. Only letters A-Z allowed.");
        }
    }
    return vec;
}

// Vector to string conversion (handles spaces)
string MatrixUtils::vectorToString(const vector<int>& vec) {
    string str;
    str.reserve(vec.size());
    
    for (int num : vec) {
        if (num == 26) {
            str.push_back(' ');  // Space character
        } else {
            str.push_back(static_cast<char>('A' + ((num % 26 + 26) % 26)));
        }
    }
    return str;
}

// Pad string with 'X' to make length multiple of blockSize
// Now preserves spaces
string MatrixUtils::padString(const string& str, int blockSize, bool preserveSpaces) {
    string cleanStr;
    
    for (char c : str) {
        if (preserveSpaces && c == ' ') {
            cleanStr.push_back(' ');
        } else if (isalpha(c)) {
            cleanStr.push_back(toupper(c));
        } else if (preserveSpaces) {
            // Skip other characters
            continue;
        }
    }
    
    // Add padding if needed (use 'X' for padding)
    int remainder = cleanStr.length() % blockSize;
    if (remainder != 0) {
        int paddingNeeded = blockSize - remainder;
        cleanStr.append(paddingNeeded, 'X');
    }
    
    return cleanStr;
}

// Remove trailing 'X' padding
string MatrixUtils::removePadding(const string& str) {
    string result = str;
    
    // Remove trailing 'X' characters (but not spaces)
    while (!result.empty() && result.back() == 'X') {
        result.pop_back();
    }
    
    return result;
}