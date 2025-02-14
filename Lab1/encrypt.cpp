#include <string>
#include <cstring>
#include <vector>
#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <iostream>

#include "encrypt.h"

std::string caesar_encrypt(const std::string &message, int shift) {
	std::string encrypted;
	for (char c : message) {
		if (std::isalpha(c)) {
			char base = std::islower(c) ? 'a' : 'A';
			encrypted += (c - base + shift) % 26 + base;
		} else {
			encrypted += c;
		}
	}
	return encrypted;
}

std::string generate_repeating_key(const std::string &key, int message_length) {
    std::string actual_key;
    int k_len = key.length();

    int reps = message_length / k_len;
    int rem = message_length % k_len;

    for (int i = 0; i < reps; i++) {
        actual_key += key;
    }

    actual_key += key.substr(0, rem);
    return actual_key;
}

std::string vigenere_encrypt(const std::string &message, const std::string &key) {
    std::string encrypted;
    std::string actual_key = generate_repeating_key(key, message.length());

    for (int i = 0; i < message.length(); i++) {
        char c = message[i];
        char k = actual_key[i];
        if (std::isalpha(c)) {
            char base = std::islower(c) ? 'a' : 'A';
            encrypted += ((c - base + (k - base)) % 26) + base;
        } else {
            encrypted += c; // Preserve non-alphabet characters
        }
    }
    return encrypted;
}

std::string vigenere_decrypt(const std::string &message, const std::string &key) {
    std::string decrypted;
    std::string actual_key = generate_repeating_key(key, message.length());

    for (int i = 0; i < message.length(); i++) {
        char c = message[i];
        char k = actual_key[i];
        if (std::isalpha(c)) {
            char base = std::islower(c) ? 'a' : 'A';
            decrypted += ((c - base - (k - base) + 26) % 26) + base;
        } else {
            decrypted += c; // Preserve non-alphabet characters
        }
    }
    return decrypted;
}

std::string vernam_encrypt(const std::string &message, const std::string &key) {
    if (key.length() < message.length()) {
        throw std::invalid_argument("Key length must be at least equal to the message length.");
    }

    std::string encrypted;
    for (size_t i = 0; i < message.length(); i++) {
        // XOR the characters and ensure the result is printable by masking with 0x7F (ASCII range)
        encrypted += (message[i] ^ key[i]) & 0x7F;
    }
    return encrypted;
}

std::string vernam_decrypt(const std::string &encrypted, const std::string &key) {
    std::string decrypted;
    for (size_t i = 0; i < encrypted.length(); i++) {
        // XOR the characters to get the original message
        decrypted += (encrypted[i] ^ key[i]) & 0x7F;
    }
    return decrypted;
}

// Function to remove duplicate characters from a string
std::string removeDuplicates(const std::string& str) {
    std::string result;
    for (char ch : str) {
        if (result.find(ch) == std::string::npos) {
            result += ch;
        }
    }
    return result;
}

// Function to generate the 5x5 Playfair cipher key matrix
std::vector<std::vector<char>> generateKeyMatrix(const std::string& key) {
    std::string processedKey = removeDuplicates(key);
    processedKey.erase(std::remove(processedKey.begin(), processedKey.end(), 'j'), processedKey.end());
    std::string alphabet = "abcdefghiklmnopqrstuvwxyz"; // No 'j'

    for (char ch : alphabet) {
        if (processedKey.find(ch) == std::string::npos) {
            processedKey += ch;
        }
    }

    std::vector<std::vector<char>> keyMatrix(5, std::vector<char>(5));
    int index = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            keyMatrix[i][j] = processedKey[index++];
        }
    }

		for(auto row: keyMatrix) {
				for(char ch: row) {
						std::cout << ch << " ";
				}
				std::cout << std::endl;
		}
		
    return keyMatrix;
}

// Function to find the position of a character in the key matrix
std::pair<int, int> findPosition(const std::vector<std::vector<char>>& keyMatrix, char ch) {
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            if (keyMatrix[i][j] == ch) {
                return {i, j};
            }
        }
    }
    return {-1, -1};
}

// Function to preprocess the plaintext
std::string preprocessPlaintext(std::string plaintext) {
    plaintext.erase(std::remove_if(plaintext.begin(), plaintext.end(), [](char ch) { return !std::isalpha(ch); }), plaintext.end());
    std::transform(plaintext.begin(), plaintext.end(), plaintext.begin(), ::tolower);
    std::replace(plaintext.begin(), plaintext.end(), 'j', 'i');

    std::string processedText;
    for (size_t i = 0; i < plaintext.length(); ++i) {
        processedText += plaintext[i];
        if (i + 1 < plaintext.length() && plaintext[i] == plaintext[i + 1]) {
            processedText += 'x';
        }
    }

    if (processedText.length() % 2 != 0) {
        processedText += 'x';
    }
    return processedText;
}

// Function to encrypt a pair of characters
std::string encryptPair(const std::vector<std::vector<char>>& keyMatrix, char a, char b) {
    auto pos1 = findPosition(keyMatrix, a);
    auto pos2 = findPosition(keyMatrix, b);

    if (pos1.first == pos2.first) {
        // Same row
        return {keyMatrix[pos1.first][(pos1.second + 1) % 5], keyMatrix[pos2.first][(pos2.second + 1) % 5]};
    } else if (pos1.second == pos2.second) {
        // Same column
        return {keyMatrix[(pos1.first + 1) % 5][pos1.second], keyMatrix[(pos2.first + 1) % 5][pos2.second]};
    } else {
        // Rectangle swap
        return {keyMatrix[pos1.first][pos2.second], keyMatrix[pos2.first][pos1.second]};
    }
}

std::string decryptPair(const std::vector<std::vector<char>>& keyMatrix, char a, char b) {
    auto pos1 = findPosition(keyMatrix, a);
    auto pos2 = findPosition(keyMatrix, b);

    if (pos1.first == pos2.first) {
        // Same row
        return {keyMatrix[pos1.first][(pos1.second + 4) % 5], keyMatrix[pos2.first][(pos2.second + 4) % 5]};
    } else if (pos1.second == pos2.second) {
        // Same column
        return {keyMatrix[(pos1.first + 4) % 5][pos1.second], keyMatrix[(pos2.first + 4) % 5][pos2.second]};
    } else {
        // Rectangle swap
        return {keyMatrix[pos1.first][pos2.second], keyMatrix[pos2.first][pos1.second]};
    }
}

// Function to encrypt plaintext using the Playfair cipher
std::string playfair_encrypt(const std::string& plaintext, const std::string& key) {
    std::vector<std::vector<char>> keyMatrix = generateKeyMatrix(key);
    std::string processedText = preprocessPlaintext(plaintext);

    std::string ciphertext;
    for (size_t i = 0; i < processedText.length(); i += 2) {
        ciphertext += encryptPair(keyMatrix, processedText[i], processedText[i + 1]);
    }
    return ciphertext;
}


// Function to mod a number to always return positive
int mod(int a, int b) {
    return (a % b + b) % b;
}

// Function to validate if a matrix is invertible modulo 26
int determinant(const std::vector<std::vector<int>>& matrix, int n) {
    if (n == 1) return matrix[0][0];

    int det = 0;
    std::vector<std::vector<int>> submatrix(n - 1, std::vector<int>(n - 1));
    for (int x = 0; x < n; x++) {
        int subi = 0;
        for (int i = 1; i < n; i++) {
            int subj = 0;
            for (int j = 0; j < n; j++) {
                if (j == x) continue;
                submatrix[subi][subj] = matrix[i][j];
                subj++;
            }
            subi++;
        }
        det += (x % 2 == 0 ? 1 : -1) * matrix[0][x] * determinant(submatrix, n - 1);
    }
    return det;
}

int modularInverse(int a, int m) {
    a = mod(a, m);
    for (int x = 1; x < m; ++x) {
        if ((a * x) % m == 1) return x;
    }
    throw std::invalid_argument("Matrix determinant is not invertible modulo 26.");
}

std::vector<std::vector<int>> matrixInverse(const std::vector<std::vector<int>>& matrix, int n) {
    int det = determinant(matrix, n);
    int detModInverse = modularInverse(det, 26);

    std::vector<std::vector<int>> adjoint(n, std::vector<int>(n));
    if (n == 1) {
        adjoint[0][0] = 1;
        return adjoint;
    }

    std::vector<std::vector<int>> submatrix(n - 1, std::vector<int>(n - 1));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int subi = 0;
            for (int x = 0; x < n; x++) {
                if (x == i) continue;
                int subj = 0;
                for (int y = 0; y < n; y++) {
                    if (y == j) continue;
                    submatrix[subi][subj] = matrix[x][y];
                    subj++;
                }
                subi++;
            }
            adjoint[j][i] = (determinant(submatrix, n - 1) * ((i + j) % 2 == 0 ? 1 : -1));
        }
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            adjoint[i][j] = mod(adjoint[i][j] * detModInverse, 26);
        }
    }
    return adjoint;
}

std::string hill_encrypt(const std::string& plaintext, const std::vector<std::vector<int>>& key) {
    int n = key.size();
    std::string processedText = plaintext;
    processedText.erase(std::remove_if(processedText.begin(), processedText.end(), [](char c) { return !std::isalpha(c); }), processedText.end());
    std::transform(processedText.begin(), processedText.end(), processedText.begin(), ::tolower);

    while (processedText.size() % n != 0) {
        processedText += 'x';
    }

    std::string ciphertext;
    for (size_t i = 0; i < processedText.size(); i += n) {
        for (int row = 0; row < n; ++row) {
            int sum = 0;
            for (int col = 0; col < n; ++col) {
                sum += key[row][col] * (processedText[i + col] - 'a');
            }
            ciphertext += (sum % 26) + 'a';
        }
    }
    return ciphertext;
}

std::string hill_decrypt(const std::string& ciphertext, const std::vector<std::vector<int>>& key) {
    int n = key.size();
    std::vector<std::vector<int>> inverseKey = matrixInverse(key, n);

    std::string plaintext;
    for (size_t i = 0; i < ciphertext.size(); i += n) {
        for (int row = 0; row < n; ++row) {
            int sum = 0;
            for (int col = 0; col < n; ++col) {
                sum += inverseKey[row][col] * (ciphertext[i + col] - 'a');
            }
            plaintext += (mod(sum, 26)) + 'a';
        }
    }
    return plaintext;
}

std::string caesar_decrypt(const std::string &message, int shift) {
	std::string decrypted;
	for (char c : message) {
		if (std::isalpha(c)) {
			char base = std::islower(c) ? 'a' : 'A';
			decrypted += (c - base - shift) % 26 + base;
		} else {
			decrypted += c;
		}
	}
	return decrypted;
}

std::string playfair_decrypt(const std::string& ciphertext, const std::string& key) {
    std::vector<std::vector<char>> keyMatrix = generateKeyMatrix(key);

    std::string plaintext;
    for (size_t i = 0; i < ciphertext.length(); i += 2) {
        plaintext += decryptPair(keyMatrix, ciphertext[i], ciphertext[i + 1]);
    }
    return plaintext;
}

std::string columnar_encrypt(const std::string& plaintext, const std::string& key) {
    int numCols = key.size();
    int numRows = (plaintext.size() + numCols - 1) / numCols;

    // Create a grid and fill it with the plaintext
    std::vector<std::string> grid(numRows, std::string(numCols, ' '));
    for (size_t i = 0; i < plaintext.size(); ++i) {
        grid[i / numCols][i % numCols] = plaintext[i];
    }

    // Determine the column order based on the key
    std::vector<int> columnOrder(numCols);
    for (size_t i = 0; i < key.size(); ++i) {
        columnOrder[i] = i;
    }

    std::sort(columnOrder.begin(), columnOrder.end(), [&key](int a, int b) {
        return key[a] < key[b];
    });

    // Generate ciphertext by reading columns in the determined order
    std::string ciphertext;
    for (int col : columnOrder) {
        for (int row = 0; row < numRows; ++row) {
            if (grid[row][col] != ' ') {
                ciphertext += grid[row][col];
            }
        }
    }

    return ciphertext;
}

// Function to decrypt using columnar transposition cipher
std::string columnar_decrypt(const std::string& ciphertext, const std::string& key) {
    int numCols = key.size();
    int numRows = (ciphertext.size() + numCols - 1) / numCols;

    // Calculate how many characters each column should have
    std::vector<int> columnLengths(numCols, numRows);
    int extraChars = ciphertext.size() % numCols;
    for (int i = 0; i < numCols; ++i) {
        if (i >= extraChars) {
            columnLengths[i]--;
        }
    }

    // Determine the column order based on the key
    std::vector<int> columnOrder(numCols);
    for (size_t i = 0; i < key.size(); ++i) {
        columnOrder[i] = i;
    }

    std::sort(columnOrder.begin(), columnOrder.end(), [&key](int a, int b) {

return key[a] < key[b];
    });

    // Fill the grid column by column based on the ciphertext
    std::vector<std::string> grid(numCols);
    int index = 0;
    for (int col : columnOrder) {
        grid[col] = ciphertext.substr(index, columnLengths[col]);
        index += columnLengths[col];
    }

    // Reconstruct plaintext by reading the grid row by row
    std::string plaintext;
    for (int row = 0; row < numRows; ++row) {
        for (int col = 0; col < numCols; ++col) {
            if (row < grid[col].size()) {
                plaintext += grid[col][row];
            }
        }
    }

    return plaintext;
}

std::string encrypt(const std::string &message, int choice) {
	switch(choice) {
		case 1: return caesar_encrypt(message, 3);
		case 2: return vigenere_encrypt(message, "KEDAR");
		case 3: return vernam_encrypt(message, "KEDAR");
		case 4: return playfair_encrypt(message, "minimum");
		case 5: return hill_encrypt(message, {{2, 3}, {1, 4}});
		case 6: return columnar_encrypt(message, "ZEBRAS");
		default: return message;
	}
}

std::string decrypt(const std::string &message, int choice){
	switch(choice) {
		case 1: return caesar_decrypt(message, 3);
		case 2: return vigenere_decrypt(message, "KEDAR");
		case 3: return vernam_decrypt(message, "KEDAR");
		case 4: return playfair_decrypt(message, "minimum");
		case 5: return hill_decrypt(message, {{2, 3}, {1, 4}});
		case 6: return columnar_decrypt(message, "ZEBRAS");
		default: return message;
	}
} 
