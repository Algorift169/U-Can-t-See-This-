// In matrix_utils.cpp, modify stringToVector and padString functions:

vector<int> MatrixUtils::stringToVector(const string& str) {
    vector<int> vec;
    vec.reserve(str.length());
    
    for (char c : str) {
        if (isalpha(c)) {
            vec.push_back(toupper(c) - 'A');
        } else if (c == ' ') {
            // Encode space as 26 (one beyond Z)
            vec.push_back(26);
        } else {
            throw runtime_error("Invalid character in message. Only letters A-Z and spaces allowed.");
        }
    }
    return vec;
}

string MatrixUtils::vectorToString(const vector<int>& vec) {
    string str;
    str.reserve(vec.size());
    
    for (int num : vec) {
        if (num == 26) {
            str.push_back(' '); // Decode 26 as space
        } else {
            str.push_back(static_cast<char>('A' + ((num % 26 + 26) % 26)));
        }
    }
    return str;
}

string MatrixUtils::padString(const string& str, int blockSize) {
    string padded = str;
    
    // Convert to uppercase (keeping spaces)
    transform(padded.begin(), padded.end(), padded.begin(), ::toupper);
    
    // Remove non-alphabetic and non-space characters
    string cleanStr;
    for (char c : padded) {
        if (isalpha(c) || c == ' ') {
            cleanStr.push_back(c);
        }
    }
    
    // Add padding if needed
    int remainder = cleanStr.length() % blockSize;
    if (remainder != 0) {
        int paddingNeeded = blockSize - remainder;
        // Pad with 'X' but ensure proper modulo arithmetic
        cleanStr.append(paddingNeeded, 'X');
    }
    
    return cleanStr;
}

string MatrixUtils::removePadding(const string& str) {
    string result = str;
    
    // Remove trailing 'X' characters
    while (!result.empty() && result.back() == 'X') {
        result.pop_back();
    }
    
    return result;
}

// Update multiplyMatrixVector to handle mod 27 instead of 26
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
