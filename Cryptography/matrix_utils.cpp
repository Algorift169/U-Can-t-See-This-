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

// Adjugate matrix calculation (FIXED - now returns correct adjugate)
vector<vector<int>> MatrixUtils::adjugate(const vector<vector<int>>& matrix) {
    int n = matrix.size();
    
    if (n == 1) {
        return {{1}};
    }
    
    if (n == 2) {
        vector<vector<int>> adj(2, vector<int>(2, 0));
        adj[0][0] = matrix[1][1];
        adj[0][1] = (-matrix[0][1] + 26) % 26;
        adj[1][0] = (-matrix[1][0] + 26) % 26;
        adj[1][1] = matrix[0][0];
        return adj;
    }
    
    if (n == 3) {
        vector<vector<int>> adj(3, vector<int>(3, 0));
        // Calculate cofactor matrix first
        vector<vector<int>> cofactor(3, vector<int>(3, 0));
        
        cofactor[0][0] = (matrix[1][1] * matrix[2][2] - matrix[1][2] * matrix[2][1]);
        cofactor[0][1] = -(matrix[1][0] * matrix[2][2] - matrix[1][2] * matrix[2][0]);
        cofactor[0][2] = (matrix[1][0] * matrix[2][1] - matrix[1][1] * matrix[2][0]);
        
        cofactor[1][0] = -(matrix[0][1] * matrix[2][2] - matrix[0][2] * matrix[2][1]);
        cofactor[1][1] = (matrix[0][0] * matrix[2][2] - matrix[0][2] * matrix[2][0]);
        cofactor[1][2] = -(matrix[0][0] * matrix[2][1] - matrix[0][1] * matrix[2][0]);
        
        cofactor[2][0] = (matrix[0][1] * matrix[1][2] - matrix[0][2] * matrix[1][1]);
        cofactor[2][1] = -(matrix[0][0] * matrix[1][2] - matrix[0][2] * matrix[1][0]);
        cofactor[2][2] = (matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]);
        
        // Transpose to get adjugate
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                adj[i][j] = ((cofactor[j][i] % 26) + 26) % 26;
            }
        }
        
        return adj;
    }
    
    throw runtime_error("Matrix size not supported");
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

// String to vector conversion (A=0, B=1, ..., Z=25) - UPDATED for spaces
vector<int> MatrixUtils::stringToVector(const string& str, bool includeSpaces) {
    vector<int> vec;
    vec.reserve(str.length());
    
    for (char c : str) {
        if (isalpha(c)) {
            vec.push_back(toupper(c) - 'A');
        } else if (c == ' ' && includeSpaces) {
            vec.push_back(-1); // Use -1 to represent space
        } else {
            throw runtime_error("Invalid character in message. Only letters and spaces allowed.");
        }
    }
    return vec;
}

// Vector to string conversion - UPDATED for spaces
string MatrixUtils::vectorToString(const vector<int>& vec) {
    string str;
    str.reserve(vec.size());
    
    for (int num : vec) {
        if (num == -1) {
            str.push_back(' '); // Space
        } else {
            str.push_back(static_cast<char>('A' + ((num % 26 + 26) % 26)));
        }
    }
    return str;
}

// Pad string with 'X' to make length multiple of blockSize - UPDATED for spaces
string MatrixUtils::padString(const string& str, int blockSize, bool preserveSpaces) {
    string processed = str;
    
    // Convert to uppercase
    transform(processed.begin(), processed.end(), processed.begin(), ::toupper);
    
    // Remove non-alphabetic characters (except spaces if preserveSpaces is true)
    string cleanStr;
    for (char c : processed) {
        if (isalpha(c) || (c == ' ' && preserveSpaces)) {
            cleanStr.push_back(c);
        }
    }
    
    // If preserving spaces, count only letters for block alignment
    if (preserveSpaces) {
        string lettersOnly;
        for (char c : cleanStr) {
            if (isalpha(c)) lettersOnly.push_back(c);
        }
        
        int remainder = lettersOnly.length() % blockSize;
        if (remainder != 0) {
            int paddingNeeded = blockSize - remainder;
            cleanStr.append(paddingNeeded, 'X');
        }
    } else {
        // Original logic without spaces
        int remainder = cleanStr.length() % blockSize;
        if (remainder != 0) {
            int paddingNeeded = blockSize - remainder;
            cleanStr.append(paddingNeeded, 'X');
        }
    }
    
    return cleanStr;
}

// Remove trailing 'X' padding - UPDATED
string MatrixUtils::removePadding(const string& str) {
    string result = str;
    
    // Remove trailing 'X' characters that are not part of original message
    while (!result.empty() && result.back() == 'X') {
        result.pop_back();
    }
    
    return result;
}

// NEW: Split string into blocks while preserving spaces
vector<int> MatrixUtils::extractAlphaBlock(const string& str, int start, int blockSize) {
    vector<int> block;
    int count = 0;
    
    for (size_t i = start; i < str.length() && count < blockSize; i++) {
        if (isalpha(str[i])) {
            block.push_back(toupper(str[i]) - 'A');
            count++;
        }
    }
    
    // If not enough letters at the end, pad with 'X'
    while (block.size() < blockSize) {
        block.push_back('X' - 'A'); // 23 for X
    }
    
    return block;
}

// NEW: Count alphabetic characters in string
int MatrixUtils::countAlpha(const string& str) {
    int count = 0;
    for (char c : str) {
        if (isalpha(c)) count++;
    }
    return count;
}
