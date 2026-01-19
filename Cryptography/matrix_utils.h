#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <vector>
#include <string>
#include <stdexcept>

class MatrixUtils {
public:
    // Matrix multiplication modulo 26
    static std::vector<int> multiplyMatrixVector(const std::vector<std::vector<int>>& matrix, 
                                                 const std::vector<int>& vector, 
                                                 int mod = 26);
    
    // Calculate modular inverse of a number modulo 26
    static int modInverse(int a, int m = 26);
    
    // Calculate determinant of a matrix
    static int determinant(const std::vector<std::vector<int>>& matrix);
    
    // Calculate adjugate of a matrix
    static std::vector<std::vector<int>> adjugate(const std::vector<std::vector<int>>& matrix);
    
    // Calculate modular inverse of a matrix
    static std::vector<std::vector<int>> inverseMatrix(const std::vector<std::vector<int>>& matrix);
    
    // Convert string to vector of integers (A=0, B=1, ..., Z=25)
    static std::vector<int> stringToVector(const std::string& str, bool includeSpaces = false);
    
    // Convert vector of integers to string
    static std::string vectorToString(const std::vector<int>& vec);
    
    // Pad string to make length multiple of matrix size
    static std::string padString(const std::string& str, int blockSize, bool preserveSpaces = false);
    
    // Remove padding characters
    static std::string removePadding(const std::string& str);
    
    // Extract alphabetic block from string
    static std::vector<int> extractAlphaBlock(const std::string& str, int start, int blockSize);
    
    // Count alphabetic characters in string
    static int countAlpha(const std::string& str);
};

#endif
