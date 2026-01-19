#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// Same key matrix as encryption (3x3)
const vector<vector<int>> KEY_MATRIX = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

// Function to print decorative header
void printHeader(const string& title) {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════════════╗\n";
    cout << "║                                                              ║\n";
    cout << "║                " << setw(40) << left << title << "                ║\n";
    cout << "║                                                              ║\n";
    cout << "╚══════════════════════════════════════════════════════════════╝\n";
}

// Function to print section separator
void printSeparator() {
    cout << "\n┌──────────────────────────────────────────────────────────────┐\n";
}

// Function to print boxed text
void printBox(const string& label, const string& content) {
    cout << "│ " << setw(15) << left << label << ": " << setw(47) << left << content << "│\n";
}

// Function to print footer
void printFooter() {
    cout << "\n└──────────────────────────────────────────────────────────────┘\n";
}

// Decrypt message using Hill cipher with space preservation
string decryptMessage(const string& encryptedMessage) {
    int blockSize = KEY_MATRIX.size();
    
    // Calculate inverse matrix
    static vector<vector<int>> inverseKey = MatrixUtils::inverseMatrix(KEY_MATRIX);
    
    // Process the encrypted message
    string decrypted;
    int charIndex = 0;
    
    while (charIndex < encryptedMessage.length()) {
        if (encryptedMessage[charIndex] == ' ') {
            // Preserve spaces
            decrypted += ' ';
            charIndex++;
        } else {
            // Extract block of letters
            vector<int> block;
            for (int j = 0; j < blockSize && charIndex < encryptedMessage.length(); j++) {
                // Skip spaces in encrypted text
                while (charIndex < encryptedMessage.length() && encryptedMessage[charIndex] == ' ') {
                    decrypted += ' ';
                    charIndex++;
                }
                
                if (charIndex < encryptedMessage.length()) {
                    block.push_back(toupper(encryptedMessage[charIndex]) - 'A');
                    charIndex++;
                }
            }
            
            // If we have a complete block, decrypt it
            if (block.size() == blockSize) {
                vector<int> decryptedBlock = MatrixUtils::multiplyMatrixVector(inverseKey, block, 26);
                
                for (int num : decryptedBlock) {
                    decrypted += static_cast<char>('A' + ((num % 26 + 26) % 26));
                }
            }
        }
    }
    
    // Remove padding
    string result = MatrixUtils::removePadding(decrypted);
    
    // Convert to proper case (first letter uppercase, rest lowercase)
    bool newWord = true;
    for (char& c : result) {
        if (c == ' ') {
            newWord = true;
        } else if (newWord) {
            c = toupper(c);
            newWord = false;
        } else {
            c = tolower(c);
        }
    }
    
    return result;
}

int main() {
    // Enhanced UI with design
    printHeader("HILL CIPHER DECRYPTION");
    
    cout << "\n";
    cout << "┌──────────────────────────────────────────────────────────────┐\n";
    cout << "│                     CONFIGURATION                            │\n";
    cout << "├──────────────────────────────────────────────────────────────┤\n";
    cout << "│  Matrix Size: " << setw(48) << left << "3x3" << "│\n";
    cout << "│  Space Preservation: " << setw(40) << left << "Enabled" << "│\n";
    cout << "└──────────────────────────────────────────────────────────────┘\n";
    
    // Get input from user
    string inputMethod;
    cout << "\n\n";
    cout << "╭──────────────────────────────────────────────────────────────╮\n";
    cout << "│                     INPUT METHOD                             │\n";
    cout << "├──────────────────────────────────────────────────────────────┤\n";
    cout << "│  1. Type encrypted message                                   │\n";
    cout << "│  2. Read from file                                           │\n";
    cout << "╰──────────────────────────────────────────────────────────────╯\n";
    cout << "\nEnter your choice (1 or 2): ";
    getline(cin, inputMethod);
    
    string encryptedMessage;
    
    if (inputMethod == "1") {
        cout << "\n";
        cout << "╭──────────────────────────────────────────────────────────────╮\n";
        cout << "│                     ENTER ENCRYPTED TEXT                     │\n";
        cout << "╰──────────────────────────────────────────────────────────────╯\n";
        cout << "> ";
        getline(cin, encryptedMessage);
        cout << "\n✓ Encrypted text received\n";
    } else if (inputMethod == "2") {
        string filename;
        cout << "\n";
        cout << "╭──────────────────────────────────────────────────────────────╮\n";
        cout << "│                     FILE SELECTION                           │\n";
        cout << "╰──────────────────────────────────────────────────────────────╯\n";
        cout << "Enter filename (press Enter for 'encrypted.txt'): ";
        getline(cin, filename);
        
        if (filename.empty()) {
            filename = "encrypted.txt";
        }
        
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "\n❌ Error: Cannot open file " << filename << endl;
            return 1;
        }
        getline(inputFile, encryptedMessage);
        inputFile.close();
        cout << "✓ Encrypted text loaded from file\n";
    } else {
        cerr << "\n❌ Invalid choice!" << endl;
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
        
        // Display results with enhanced design
        printHeader("DECRYPTION RESULTS");
        
        printSeparator();
        printBox("Encrypted Text", encryptedMessage);
        printBox("Decrypted Message", decrypted);
        printBox("Processing Time", to_string(duration.count()) + " microseconds");
        
        // Inverse matrix display
        cout << "│\n";
        cout << "│ Inverse Matrix (mod 26):                                   │\n";
        try {
            vector<vector<int>> inverseKey = MatrixUtils::inverseMatrix(KEY_MATRIX);
            cout << "│   [" << setw(2) << inverseKey[0][0] << ", " << setw(2) << inverseKey[0][1] << ", " << setw(2) << inverseKey[0][2] << "]                                       │\n";
            cout << "│   [" << setw(2) << inverseKey[1][0] << ", " << setw(2) << inverseKey[1][1] << ", " << setw(2) << inverseKey[1][2] << "]                                       │\n";
            cout << "│   [" << setw(2) << inverseKey[2][0] << ", " << setw(2) << inverseKey[2][1] << ", " << setw(2) << inverseKey[2][2] << "]                                       │\n";
        } catch (...) {
            cout << "│   Could not calculate inverse matrix                       │\n";
        }
        
        printFooter();
        
        // Save to file
        ofstream outputFile("decrypted.txt");
        if (outputFile) {
            outputFile << decrypted;
            outputFile.close();
            cout << "\n✅ Decrypted message saved to 'decrypted.txt'\n";
        } else {
            cerr << "\n⚠️  Warning: Could not save to file\n";
        }
        
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════════════╗\n";
        cout << "║                     DECRYPTION COMPLETE                      ║\n";
        cout << "╚══════════════════════════════════════════════════════════════╝\n";
        
    } catch (const exception& e) {
        cerr << "\n❌ Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
