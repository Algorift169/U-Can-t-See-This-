#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

// Same key matrix as encryption (3x3)
const vector<vector<int>> KEY_MATRIX = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Decrypt message using Hill cipher
string decryptMessage(const string& encryptedMessage) {
    int blockSize = KEY_MATRIX.size();
    
    // Calculate inverse matrix (precompute for efficiency)
    static vector<vector<int>> inverseKey = MatrixUtils::inverseMatrix(KEY_MATRIX);
    
    // Convert encrypted message to numerical vector
    vector<int> encryptedVector = MatrixUtils::stringToVector(encryptedMessage);
    
    // Decrypt block by block
    vector<int> decryptedVector;
    decryptedVector.reserve(encryptedVector.size());
    
    for (size_t i = 0; i < encryptedVector.size(); i += blockSize) {
        // Extract block
        vector<int> block;
        for (int j = 0; j < blockSize; j++) {
            block.push_back(encryptedVector[i + j]);
        }
        
        // Decrypt the block using inverse matrix
        vector<int> decryptedBlock = MatrixUtils::multiplyMatrixVector(inverseKey, block, 26);
        
        // Add to result
        decryptedVector.insert(decryptedVector.end(), 
                              decryptedBlock.begin(), decryptedBlock.end());
    }
    
    // Convert back to string and remove padding
    string decrypted = MatrixUtils::vectorToString(decryptedVector);
    return MatrixUtils::removePadding(decrypted);
}

int main() {
    cout << "=== Hill Cipher Decryption ===" << endl;
    cout << "Matrix Size: " << KEY_MATRIX.size() << "x" << KEY_MATRIX[0].size() << endl;
    
    // Get input from user
    string inputMethod;
    cout << "\nEnter '1' to type encrypted message or '2' to read from file: ";
    getline(cin, inputMethod);
    
    string encryptedMessage;
    
    if (inputMethod == "1") {
        cout << "Enter encrypted message: ";
        getline(cin, encryptedMessage);
    } else if (inputMethod == "2") {
        string filename;
        cout << "Enter filename (default: encrypted.txt): ";
        getline(cin, filename);
        
        if (filename.empty()) {
            filename = "encrypted.txt";
        }
        
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error: Cannot open file " << filename << endl;
            return 1;
        }
        getline(inputFile, encryptedMessage);
        inputFile.close();
        cout << "Read encrypted message from file" << endl;
    } else {
        cerr << "Invalid choice!" << endl;
        return 1;
    }
    
    // Start timer
    auto startTime = high_resolution_clock::now();
    
    try {
        // Decrypt the message
        string decrypted = decryptMessage(encryptedMessage);
        
        // Stop timer
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime);
        
        // Display results
        cout << "\n--- Results ---" << endl;
        cout << "Encrypted Message: " << encryptedMessage << endl;
        cout << "Decrypted Message: " << decrypted << endl;
        cout << "Decryption Time: " << duration.count() << " microseconds" << endl;
        
        // Save to file
        ofstream outputFile("decrypted.txt");
        if (outputFile) {
            outputFile << decrypted;
            outputFile.close();
            cout << "Decrypted message saved to 'decrypted.txt'" << endl;
        } else {
            cerr << "Warning: Could not save to file" << endl;
        }
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
