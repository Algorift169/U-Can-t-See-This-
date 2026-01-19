#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;
using namespace chrono;

// Predefined key matrix (3x3 example - must be invertible mod 26)
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

// Encrypt message using Hill cipher with space preservation
string encryptMessage(const string& message) {
    int blockSize = KEY_MATRIX.size();
    
    // Pad and clean the message (preserve spaces)
    string paddedMessage = MatrixUtils::padString(message, blockSize, true);
    
    // Encrypt block by block
    string encrypted;
    int alphaCount = 0;
    int totalAlpha = MatrixUtils::countAlpha(paddedMessage);
    
    for (size_t i = 0; alphaCount < totalAlpha; i++) {
        if (i < paddedMessage.length()) {
            if (paddedMessage[i] == ' ') {
                // Preserve spaces in output
                encrypted += ' ';
            } else {
                // Extract block of alphabetic characters
                vector<int> block;
                int charsTaken = 0;
                
                // Find next block of letters
                for (size_t j = i; j < paddedMessage.length() && charsTaken < blockSize; j++) {
                    if (isalpha(paddedMessage[j])) {
                        block.push_back(toupper(paddedMessage[j]) - 'A');
                        charsTaken++;
                        alphaCount++;
                    }
                }
                i += charsTaken - 1;
                
                // Pad if necessary
                while (block.size() < blockSize) {
                    block.push_back('X' - 'A');
                }
                
                // Encrypt the block
                vector<int> encryptedBlock = MatrixUtils::multiplyMatrixVector(KEY_MATRIX, block, 26);
                
                // Add to result
                for (int num : encryptedBlock) {
                    encrypted += static_cast<char>('A' + ((num % 26 + 26) % 26));
                }
            }
        }
    }
    
    return encrypted;
}

int main() {
    // Enhanced UI with design
    printHeader("HILL CIPHER ENCRYPTION");
    
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
    cout << "│  1. Type message manually                                    │\n";
    cout << "│  2. Read from file                                           │\n";
    cout << "╰──────────────────────────────────────────────────────────────╯\n";
    cout << "\nEnter your choice (1 or 2): ";
    getline(cin, inputMethod);
    
    string message;
    
    if (inputMethod == "1") {
        cout << "\n";
        cout << "╭──────────────────────────────────────────────────────────────╮\n";
        cout << "│                     ENTER MESSAGE                            │\n";
        cout << "╰──────────────────────────────────────────────────────────────╯\n";
        cout << "> ";
        getline(cin, message);
        cout << "\n✓ Message received: " << message << endl;
    } else if (inputMethod == "2") {
        string filename;
        cout << "\n";
        cout << "╭──────────────────────────────────────────────────────────────╮\n";
        cout << "│                     FILE SELECTION                           │\n";
        cout << "╰──────────────────────────────────────────────────────────────╯\n";
        cout << "Enter filename (press Enter for 'message.txt'): ";
        getline(cin, filename);
        
        if (filename.empty()) {
            filename = "message.txt";
        }
        
        ifstream inputFile(filename);
        if (!inputFile) {
            cerr << "\n❌ Error: Cannot open file " << filename << endl;
            return 1;
        }
        getline(inputFile, message);
        inputFile.close();
        cout << "✓ Message loaded from file: " << message << endl;
    } else {
        cerr << "\n❌ Invalid choice!" << endl;
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
        
        // Display results with enhanced design
        printHeader("ENCRYPTION RESULTS");
        
        printSeparator();
        printBox("Original Message", message);
        printBox("Encrypted Text", encrypted);
        printBox("Processing Time", to_string(duration.count()) + " microseconds");
        
        // Key matrix display
        cout << "│\n";
        cout << "│ Key Matrix (3x3):                                          │\n";
        cout << "│   [ 6, 24,  1]                                             │\n";
        cout << "│   [13, 16, 10]                                             │\n";
        cout << "│   [20, 17, 15]                                             │\n";
        
        printFooter();
        
        // Save to file
        ofstream outputFile("encrypted.txt");
        if (outputFile) {
            outputFile << encrypted;
            outputFile.close();
            cout << "\n✅ Encrypted message saved to 'encrypted.txt'\n";
        } else {
            cerr << "\n⚠️  Warning: Could not save to file\n";
        }
        
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════════════╗\n";
        cout << "║                     ENCRYPTION COMPLETE                      ║\n";
        cout << "╚══════════════════════════════════════════════════════════════╝\n";
        
    } catch (const exception& e) {
        cerr << "\n❌ Error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
