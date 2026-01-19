Hill Cipher Implementation in C++
🎯 Background & Purpose
The Hill cipher is a classical polygraphic substitution cipher invented by Lester S. Hill in 1929. Unlike simple substitution ciphers that encrypt one letter at a time, Hill cipher encrypts blocks of letters simultaneously using linear algebra and matrix multiplication. This project implements Hill cipher from scratch in C++ to demonstrate how mathematics (specifically linear algebra and modular arithmetic) can be applied to create practical encryption systems.

Why I built this: As a computer science student, I wanted to bridge the gap between theoretical cryptography and practical implementation. Hill cipher perfectly demonstrates how abstract mathematical concepts translate into real-world code.

📚 Educational Value
This project serves as an excellent learning tool for understanding:

Linear algebra applications in computer science

Modular arithmetic in cryptography

Algorithm design and optimization

Matrix operations and their computational complexity

C++ programming with object-oriented design

Perfect for students studying cryptography, linear algebra, or C++ programming.

🔧 The Four Core Files & Their Logic
1. matrix_utils.h - Blueprint for Operations
cpp
class MatrixUtils {
    // Core matrix operations for Hill cipher
    static int determinant(...);        // Calculate matrix determinant
    static vector<vector<int>> inverseMatrix(...);  // Matrix modular inverse
    static vector<int> multiplyMatrixVector(...);   // Encryption/Decryption
    // String processing utilities
    static vector<int> stringToVector(...);  // Convert text to numbers
    static string vectorToString(...);       // Convert numbers to text
};
Logic: Declares all matrix and string operations needed for Hill cipher. Acts as a contract between the encryption and decryption programs.

2. matrix_utils.cpp - Mathematical Engine
Key Algorithms Implemented:

Matrix Determinant (2×2 & 3×3)
cpp
// For 2×2: ad - bc
// For 3×3: a(ei - fh) - b(di - fg) + c(dh - eg)
Why this logic? Direct formulas are faster than general algorithms for small matrices.

Modular Inverse Calculation
cpp
int modInverse(int a, int m = 26) {
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    throw error;  // Inverse doesn't exist
}
Why brute-force? Since we're working modulo 26 (only 25 possible values), brute-force is efficient and simpler than Extended Euclidean Algorithm.

Matrix Inversion Modulo 26
cpp
K⁻¹ = (det(K))⁻¹ × adj(K) (mod 26)
Steps:

Calculate determinant det(K)

Find modular inverse of determinant det⁻¹

Calculate adjugate matrix adj(K)

Multiply each element: adj[i][j] × det⁻¹ (mod 26)

Space Preservation System
cpp
// Encode space as 26 (outside 0-25 range)
if (c == ' ') vec.push_back(26);
// During encryption, blocks with 26 skip encryption
Innovation: Most Hill cipher implementations ignore spaces. My version preserves spaces while maintaining encryption security for letters.

3. encryption.cpp - User Interface for Encryption
Program Flow:

text
1. User Input → "HELLO WORLD"
2. Preprocessing → Remove non-letters, convert to uppercase
3. Padding → "HELLOWORLDX" (add 'X' to make length multiple of 3)
4. Convert to Numbers → [7, 4, 11, 11, 14, 22, 14, 17, 11, 3, 23]
5. Block Processing (3 letters at a time):
   [7,4,11] × K = [2,5,14] → "CFO"
6. Output → "CFO..."
Key Features:

Interactive menu (type message or read file)

Timing measurements

File output (encrypted.txt)

Error handling for invalid inputs

4. decryption.cpp - User Interface for Decryption
Program Flow:

text
1. Read Ciphertext → "CFO..."
2. Convert to Numbers → [2,5,14,...]
3. Calculate K⁻¹ (inverse key matrix) ← Precomputed for efficiency
4. Block Processing:
   [2,5,14] × K⁻¹ = [7,4,11] → "HEL"
5. Remove Padding → Strip trailing 'X'
6. Output Original Message → "HELLO WORLD"
Optimization: Inverse matrix is calculated once and reused, avoiding redundant computations.

🎮 Example Usage
Installation & Compilation
bash
# Navigate to project folder
cd /workspaces/U-Can-t-See-This-/Cyptography

# Compile
g++ encryption.cpp matrix_utils.cpp -o ../build/encryption -std=c++11
g++ decryption.cpp matrix_utils.cpp -o ../build/decryption -std=c++11
Running the Programs
bash
# Run encryption
/workspaces/U-Can-t-See-This-/build/encryption

# Choose: 1 (Type message)
# Enter: HELLO WORLD

# Output:
# Original: HELLO WORLD
# Encrypted: CFO... (encrypted text)
# Saved to: encrypted.txt
bash
# Run decryption
/workspaces/U-Can-t-See-This-/build/decryption

# Choose: 2 (Read from encrypted.txt)

# Output:
# Encrypted: CFO... (from file)
# Decrypted: HELLO WORLD
# Saved to: decrypted.txt
Sample Input/Output
text
┌─────────────────────────────┬─────────────────────────────┐
│ Input (Plaintext)           │ Output (Ciphertext)         │
├─────────────────────────────┼─────────────────────────────┤
│ HELLO                       │ CFOPQ                       │
│ ATTACK AT DAWN              │ Encrypted with spaces       │
│ TEST                        │ TESTX → ZZZ... → TEST       │
│ MATHEMATICS                 │ Encrypted 3 letters at time │
└─────────────────────────────┴─────────────────────────────┘
🚀 Future Scope & Enhancements
1. Larger Matrix Support
cpp
// Currently: 1×1, 2×2, 3×3
// Future: 4×4, 5×5 for stronger security
// Challenge: Determinant calculation becomes O(n!)
2. ASCII Extension (0-255)
cpp
// Currently: A-Z only (mod 26)
// Future: Full ASCII (mod 256)
// Benefit: Encrypt spaces, punctuation, numbers
3. Key Management System
cpp
// Currently: Hardcoded key matrix
// Future: 
// - Generate random invertible matrices
// - Key exchange protocol
// - Multiple keys for different users
4. Performance Optimizations
cpp
// Future improvements:
// - Parallel block processing (OpenMP)
// - GPU acceleration for large matrices
// - Cache optimization for matrix operations
5. Modern Cryptography Integration
cpp
// Hybrid approach:
// 1. Use Hill cipher for confusion
// 2. Add diffusion with XOR operations
// 3. Implement CBC/CTR modes
// 4. Add authentication (HMAC)
6. User Interface Enhancements
cpp
// Planned features:
// - GUI (Qt/wxWidgets)
// - Web interface (HTML/CSS/JavaScript)
// - Mobile app (Flutter/React Native)
// - Command-line options for batch processing
7. Educational Tools
cpp
// Learning features:
// - Step-by-step visualization
// - Matrix operation animations
// - Cryptanalysis tools (known-plaintext attacks)
// - Comparison with other ciphers (Vigenère, AES)
🎓 Learning Outcomes
From building this project, I learned:

Mathematics in Practice: How linear algebra theorems translate to working code

Algorithm Optimization: Trading generality for performance in small matrices

Error Handling: Robust input validation and edge case management

Modular Design: Separating math logic from user interface

Cryptographic Principles: Beyond just implementation—understanding why algorithms work

🤔 Why This Matters
In an era of quantum computing and advanced cryptography, studying classical ciphers like Hill's might seem outdated. However, the mathematical foundations remain relevant:

Matrix operations are fundamental to machine learning

Modular arithmetic is essential in modern cryptography (RSA, ECC)

Algorithm design principles transfer to any computational problem

Understanding weaknesses in old systems helps build stronger new ones

This project isn't just about encrypting messages—it's about understanding the bridge between abstract mathematics and practical computer science.



HOW TO RUN AFTER DOWNLOADING:

Open the extracted folder in vsCode;

Make the exe Files inside build folder:
# Method A: Compile both at once
g++ build/Cryptography/encryption.cpp build/Cryptography/matrix_utils.cpp -o build/encryption.exe -std=c++11
g++ build/Cryptography/decryption.cpp build/Cryptography/matrix_utils.cpp -o build/decryption.exe -std=c++11

Run:
# Method B: Compile with optimizations
g++ encryption.cpp matrix_utils.cpp -o encryption.exe -std=c++11 -O2
g++ decryption.cpp matrix_utils.cpp -o decryption.exe -std=c++11 -O2

ENJOY


