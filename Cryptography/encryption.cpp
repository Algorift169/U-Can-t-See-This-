#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <vector>
#include <limits>

using namespace std;
using namespace chrono;

const vector<vector<int>> KEY_MATRIX = {
    {6, 24, 1},
    {13, 16, 10},
    {20, 17, 15}
};

/* ---------- UI ---------- */
void clearScreen() {
    for (int i = 0; i < 50; i++) cout << "\n";
}

void banner() {
    cout << "\n+==============================================================+\n";
    cout << "|              H I L L   C I P H E R  (ENCRYPT)                |\n";
    cout << "+==============================================================+\n";
}

void menu() {
    cout << "\n[1] Type message\n";
    cout << "[2] Read from file (message.txt)\n";
    cout << "[3] Exit\n";
    cout << "\nChoice > ";
}

void box(const string& k, const string& v) {
    cout << "| " << setw(12) << left << k << ": "
         << setw(45) << left << v << "|\n";
}

/* ---------- CORE ---------- */
// Returns: encrypted text, and space positions for reconstruction
pair<string, vector<int>> encryptWithSpaces(const string& msg) {
    int n = KEY_MATRIX.size();
    
    // Store original space positions
    vector<int> space_positions;
    for (size_t i = 0; i < msg.length(); i++) {
        if (msg[i] == ' ') {
            space_positions.push_back(i);
        }
    }
    
    // Remove spaces for Hill cipher
    string letters_only;
    for (char c : msg) {
        if (isalpha(c)) {
            letters_only += toupper(c);
        }
    }

    // Pad if needed
    while (letters_only.size() % n != 0)
        letters_only += 'X';

    // Encrypt blocks
    string encrypted_letters;
    for (size_t i = 0; i < letters_only.size(); i += n) {
        vector<int> block;
        for (int j = 0; j < n; j++) {
            block.push_back(letters_only[i + j] - 'A');
        }

        vector<int> res = MatrixUtils::multiplyMatrixVector(KEY_MATRIX, block, 26);

        for (int x : res) {
            encrypted_letters += char('A' + (x % 26));
        }
    }
    
    return {encrypted_letters, space_positions};
}

/* ---------- MAIN ---------- */
int main() {
    while (true) {
        clearScreen();
        banner();
        menu();

        string choice;
        getline(cin, choice);

        if (choice == "3") {
            cout << "\nExiting... Goodbye 👋\n";
            break;
        }

        string message;
        
        if (choice == "1") {
            cout << "\nEnter message > ";
            if (!getline(cin, message)) {
                cout << "\nInput error. Returning to menu.\n";
                cout << "Press ENTER to continue...";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            if (message.empty()) {
                cout << "\n⚠ Message cannot be empty\n";
                cout << "Press ENTER to continue...";
                string dummy;
                getline(cin, dummy);
                continue;
            }
        }
        else if (choice == "2") {
            ifstream in("message.txt");
            if (!in) {
                cout << "\n❌ Cannot open message.txt. Create it first.\n";
                cout << "Press ENTER to continue...";
                string dummy;
                getline(cin, dummy);
                continue;
            }
            getline(in, message);
            in.close();
            cout << "\n✓ Loaded from message.txt\n";
        }
        else {
            cout << "\n⚠ Invalid choice\n";
            cout << "Press ENTER to continue...";
            string dummy;
            getline(cin, dummy);
            continue;
        }

        try {
            auto start_time = high_resolution_clock::now();
            auto result = encryptWithSpaces(message);
            string encrypted = result.first;
            vector<int> space_positions = result.second;
            auto end_time = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end_time - start_time);

            // Display results
            clearScreen();
            cout << "\n+==============================================================+\n";
            cout << "|                         R E S U L T S                        |\n";
            cout << "+==============================================================+\n";
            box("Original", message);
            box("Encrypted", encrypted);
            box("Time", to_string(duration.count()) + " microseconds");
            cout << "+==============================================================+\n";

            // Save encrypted text
            ofstream out("encrypted.txt");
            if (out) {
                out << encrypted;
                out.close();
                cout << "\n✓ Encrypted text saved to encrypted.txt\n";
            }
            
            // Save space map
            ofstream space_out("space_map.txt");
            if (space_out) {
                // Format: original_length encrypted_length space_count pos1 pos2 pos3...
                space_out << message.length() << " " 
                         << encrypted.length() << " " 
                         << space_positions.size();
                for (int pos : space_positions) {
                    space_out << " " << pos;
                }
                space_out.close();
                cout << "✓ Space map saved to space_map.txt\n";
            }
            
        } catch (const exception& e) {
            cout << "\n❌ Error: " << e.what() << "\n";
            cout << "Press ENTER to continue...";
            string dummy;
            getline(cin, dummy);
            continue;
        }

        cout << "\nPress ENTER to return to menu...";
        string dummy;
        getline(cin, dummy);
    }
    return 0;
}
