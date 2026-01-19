#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace chrono;

// Predefined key matrix (3x3 example - must be invertible mod 26)
const vector<vector<int>> KEY_MATRIX = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Encrypt message using Hill cipher
string encryptMessage(const string& message) {
    int blockSize = KEY_MATRIX.size();
    
    // Pad and clean the message (preserve spaces)
    string paddedMessage = MatrixUtils::padString(message, blockSize, true);
    
    // Convert to numerical vector (preserve spaces)
    vector<int> messageVector = MatrixUtils::stringToVector(paddedMessage, true);
    
    // Encrypt only alphabetic characters, skip spaces (value 26)
    vector<int> encryptedVector;
    encryptedVector.reserve(messageVector.size());
    
    for (size_t i = 0; i < messageVector.size(); i += blockSize) {
        // Extract block
        vector<int> block;
        for (int j = 0; j < blockSize; j++) {
            block.push_back(messageVector[i + j]);
        }
        
        // Check if block contains spaces
        bool hasSpace = false;
        for (int num : block) {
            if (num == 26) {
                hasSpace = true;
                break;
            }
        }
        
        if (hasSpace) {
            // If block contains spaces, don't encrypt (copy as-is)
            encryptedVector.insert(encryptedVector.end(), block.begin(), block.end());
        } else {
            // Encrypt the block
            vector<int> encryptedBlock = MatrixUtils::multiplyMatrixVector(KEY_MATRIX, block, 26);
            
            // Add to result
            encryptedVector.insert(encryptedVector.end(), 
                                  encryptedBlock.begin(), encryptedBlock.end());
        }
    }
    
    // Convert back to string
    return MatrixUtils::vectorToString(encryptedVector);
}

int main() {
    cout << "=== Hill Cipher Encryption ===" << endl;
    cout << "Matrix Size: " << KEY_MATRIX.size() << "x" << KEY_MATRIX[0].size() << endl;
    cout << "Note: Spaces are preserved but not encrypted" << endl;
    
    // Get input from user
    string inputMethod;
    cout << "\nEnter '1' to type message or '2' to read from file: ";
    getline(cin, inputMethod);
    
    string message;
    
    if (inputMethod == "1") {
        cout << "Enter message to encrypt (spaces allowed): ";
        getline(cin, message);
    } else if (inputMethod == "2") {
        string filename;
        cout << "Enter filename: ";
        getline(cin, filename);
        
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "Error: Cannot open file " << filename << endl;
            return 1;
        }
        // Read entire file (preserve spaces)
        getline(inputFile, message);
        inputFile.close();
        cout << "Read message from file" << endl;
    } else {
        cerr << "Invalid choice!" << endl;
        return 1;
    }
    
    // Start timer
    auto startTime = high_resolution_clock::now();
    
    try {
        // Encrypt the message
        string encrypted = encryptMessage(message);
        
        // Stop timer
        auto endTime = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(endTime - startTime);
        
        // Display results
        cout << "\n--- Results ---" << endl;
        cout << "Original Message: " << message << endl;
        cout << "Encrypted Message: " << encrypted << endl;
        cout << "Encryption Time: " << duration.count() << " microseconds" << endl;
        
        // Save to file
        ofstream outputFile("encrypted.txt");
        if (outputFile) {
            outputFile << encrypted;
            outputFile.close();
            cout << "Encrypted message saved to 'encrypted.txt'" << endl;
        } else {
            cerr << "Warning: Could not save to file" << endl;
        }
        
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}