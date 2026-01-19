#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <vector>
#include <string>
#include <stdexcept>

class MatrixUtils {
public:
    // Matrix multiplication modulo 27 (now supports spaces)
    static std::vector<int> multiplyMatrixVector(const std::vector<std::vector<int>>& matrix, 
                                                 const std::vector<int>& vector, 
                                                 int mod = 27);  // Changed from 26 to 27
    
    // Calculate modular inverse of a number modulo mod
    static int modInverse(int a, int m = 27);  // Changed from 26 to 27
    
    // Calculate determinant of a matrix
    static int determinant(const std::vector<std::vector<int>>& matrix);
    
    // Calculate adjugate of a matrix
    static std::vector<std::vector<int>> adjugate(const std::vector<std::vector<int>>& matrix);
    
    // Calculate modular inverse of a matrix
    static std::vector<std::vector<int>> inverseMatrix(const std::vector<std::vector<int>>& matrix);
    
    // Convert string to vector of integers (A=0, B=1, ..., Z=25, Space=26)
    static std::vector<int> stringToVector(const std::string& str);
    
    // Convert vector of integers to string
    static std::string vectorToString(const std::vector<int>& vec);
    
    // Pad string to make length multiple of matrix size
    static std::string padString(const std::string& str, int blockSize);
    
    // Remove padding characters
    static std::string removePadding(const std::string& str);
};

#endif
