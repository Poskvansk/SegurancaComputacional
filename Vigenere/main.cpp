#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

#define ENCRYPT 1
#define DECRYPT 0

void build_frequency_table() {

    // Builds the frequency table for the alphabet in portuguese
    
    ifstream file("frequency_table.txt");
    string line;
    int i = 0;
    vector<pair<char, double>> portuguese_table;
    vector<pair<char, double>> english_table;
    
    // find specific word in the file    

    bool portuguese = true;
    while (getline(file, line)) {

        if(line == "English") portuguese = false;

        if(line.length() <= 1) continue;
        if(line == "Portuguese" || line == "English") continue;

        if(portuguese) {
            portuguese_table.push_back(make_pair(line[0], stod(line.substr(2))));
        }
        else {
            english_table.push_back(make_pair(line[0], stod(line.substr(2))));
        }
    }

    // sort the table by second element of the pair
    sort(portuguese_table.begin(), portuguese_table.end(), [](pair<char, double> &a, pair<char, double> &b) {
        return a.second > b.second;
    });

    sort(english_table.begin(), english_table.end(), [](pair<char, double> &a, pair<char, double> &b) {
        return a.second > b.second;
    });
}

void normalize(string& text) {

    for(size_t i = 0; i < text.length(); i++) {
        if(text[i] >= 'A' && text[i] <= 'Z') {
            text[i] += 32;
        }
    }
}

string vigenere(string message, string key, bool encrypt) {

    if(message.length() == 0) return message;

    if(key.length() == 0) return message;

    string extended_key = key;
    int pos = 0;

    normalize(message);
    normalize(key);

    for(size_t i = key.length(); i < message.length(); i++) {

        if(message[i] < 97 || message[i] > 122) {
            extended_key += message[i];
        }
        else {
            extended_key += key[pos % key.length()];
            pos++;
        }
    }

    string cipher = "";
    key = extended_key;

    if(encrypt) {
        
        for (size_t i = 0; i < message.length(); i++) {

            if(message[i] < 97 || message[i] > 122) {
                cipher += message[i];
                continue;
            }
            cipher += ((message[i]-97 + key[i]-97) % 26) + 97;
        }
    }

    else {
            
            for (size_t i = 0; i < message.length(); i++) {
    
                if(message[i] < 97 || message[i] > 122) {
                    cipher += message[i];
                    continue;
                }
                cipher += (( (message[i]-97) - (key[i]-97) + 26) % 26) + 97;
            }
    }

    return cipher;
}

void read_message(string filename, string& message) {

    ifstream file(filename);
    string line;

    if(file.is_open()) {
        while(getline(file, line)) {
            message += line;
            message += '\n';
        }
        file.close();
    }
    else {
        cout << "File not found" << endl;
    }
}

void write_cipher(string filename, string cipher) {

    ofstream file(filename);

    if(file.is_open()) {
        file << cipher;
        file.close();
    }
    else {
        cout << "File not found" << endl;
    }
}

int main(int argc, char const *argv[]) {
    
    // string filename = "Plain_text.txt";
    // string message;

    // read_message(filename, message);

    // // cout << message << endl;

    // string key = "limao";

    // // cout << vigenere(message, key, ENCRYPT) << endl;
    
    // string cipher = vigenere(message, key, ENCRYPT);

    // write_cipher("Cipher_text.txt", cipher);

    build_frequency_table();

    return 0;
}
