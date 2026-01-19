#include "matrix_utils.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <vector>
#include <sstream>
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
    cout << "|              H I L L   C I P H E R  (DECRYPT)                |\n";
    cout << "+==============================================================+\n";
}

void menu() {
    cout << "\n[1] Type encrypted text\n";
    cout << "[2] Read from file (encrypted.txt) with space map\n";
    cout << "[3] Exit\n";
    cout << "\nChoice > ";
}

void box(const string& k, const string& v) {
    cout << "| " << setw(12) << left << k << ": "
         << setw(45) << left << v << "|\n";
}

/* ---------- CORE ---------- */
// Reconstruct original message with spaces
string reconstructWithSpaces(const string& decrypted_letters, 
                            const vector<int>& space_positions, 
                            int original_length) {
    
    string result = decrypted_letters;
    
    // Insert spaces at stored positions
    for (int pos : space_positions) {
        if (pos <= result.length()) {
            result.insert(pos, " ");
        }
    }
    
    // Trim to original length if needed
    if (result.length() > original_length) {
        result = result.substr(0, original_length);
    }
    
    return result;
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

        string encrypted_text;
        vector<int> space_positions;
        int original_length = 0;
        
        if (choice == "1") {
            cout << "\nEnter encrypted text (no spaces) > ";
            if (!getline(cin, encrypted_text)) {
                cout << "\nInput error. Returning to menu.\n";
                cout << "Press ENTER to continue...";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                continue;
            }
            
            if (encrypted_text.empty()) {
                cout << "\n⚠ Message cannot be empty\n";
                cout << "Press ENTER to continue...";
                string dummy;
                getline(cin, dummy);
                continue;
            }
            
            cout << "\nNote: Manual input won't restore spaces.\n";
            cout << "Press ENTER to continue...";
            string dummy;
            getline(cin, dummy);
        }
        else if (choice == "2") {
            // Read encrypted text
            ifstream enc_in("encrypted.txt");
            if (!enc_in) {
                cout << "\n❌ Cannot open encrypted.txt. Run encryption first.\n";
                cout << "Press ENTER to continue...";
                string dummy;
                getline(cin, dummy);
                continue;
            }
            getline(enc_in, encrypted_text);
            enc_in.close();
            
            // Try to load space map
            ifstream space_in("space_map.txt");
            if (space_in) {
                int encrypted_length, space_count;
                space_in >> original_length >> encrypted_length >> space_count;
                
                space_positions.resize(space_count);
                for (int i = 0; i < space_count; i++) {
                    space_in >> space_positions[i];
                }
                space_in.close();
                cout << "\n✓ Loaded encrypted text and space map\n";
            } else {
                cout << "\n✓ Loaded encrypted text (no space map found)\n";
            }
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
            
            // Decrypt (standard Hill cipher decryption)
            int n = KEY_MATRIX.size();
            static vector<vector<int>> inverse_key = MatrixUtils::inverseMatrix(KEY_MATRIX);
            
            string decrypted_letters;
            for (size_t i = 0; i < encrypted_text.size(); i += n) {
                vector<int> block;
                for (int j = 0; j < n && (i + j) < encrypted_text.size(); j++) {
                    block.push_back(encrypted_text[i + j] - 'A');
                }

                vector<int> res = MatrixUtils::multiplyMatrixVector(inverse_key, block, 26);

                for (int x : res) {
                    decrypted_letters += char('A' + (x % 26));
                }
            }

            // Remove padding
            while (!decrypted_letters.empty() && decrypted_letters.back() == 'X') {
                decrypted_letters.pop_back();
            }
            
            // Reconstruct with spaces if we have space map
            string final_decrypted;
            if (!space_positions.empty() && original_length > 0) {
                final_decrypted = reconstructWithSpaces(decrypted_letters, space_positions, original_length);
            } else {
                final_decrypted = decrypted_letters;
            }
            
            auto end_time = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(end_time - start_time);

            // Display results
            clearScreen();
            cout << "\n+==============================================================+\n";
            cout << "|                         R E S U L T S                        |\n";
            cout << "+==============================================================+\n";
            box("Encrypted", encrypted_text);
            box("Decrypted", final_decrypted);
            if (!space_positions.empty()) {
                box("Spaces", "Restored from map");
            }
            box("Time", to_string(duration.count()) + " microseconds");
            cout << "+==============================================================+\n";

            // Save to file
            ofstream out("decrypted.txt");
            if (out) {
                out << final_decrypted;
                out.close();
                cout << "\n✓ Saved to decrypted.txt\n";
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
