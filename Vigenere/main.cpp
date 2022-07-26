#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

#define ENCRYPT 1
#define DECRYPT 0

void build_frequency_table() {

    // Builds the frequency table for the alphabet in portuguese
    
    ifstream file("frequency_table.txt");
    string line;

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

////////////////////////////////////////////////////////////////////// ATTACK

// get gcd of n numbers
int gcd(int n, int m) {
    if(m == 0) return n;
    return gcd(m, n % m);
}

int break_key_length(string cipher, int n) {
    
    unordered_map<string, vector<int>> n_grams;

    string tmp;

    for(auto& c : cipher) {
        if (c < 97 || c > 122) continue;
        tmp += c;
    }

    for(int i = 0; i < tmp.length() - n; i++) {
        n_grams[tmp.substr(i, n)].push_back(i);
    }

    vector<pair<string, vector<int>>> n_grams_vector;

    for(auto& n_gram : n_grams) {
        n_grams_vector.push_back(make_pair(n_gram.first, n_gram.second));
    }

    sort(n_grams_vector.begin(), n_grams_vector.end(), [](pair<string, vector<int>> &a, pair<string, vector<int>> &b) {
        return a.second.size() > b.second.size();
    });

    while(n_grams_vector.back().second.size() <= n_grams_vector.front().second.size() / 2) {
        // cout << n_grams_vector.back().first << endl;
        n_grams_vector.pop_back();
    }

    for(auto& n_gram : n_grams_vector) {

        cout << n_gram.first << ": ";
        for(auto& p : n_gram.second) cout << p << " ";
        cout << endl;
    }
    cout << "----------------------------------------------------" << endl;

    vector<int> distances[n_grams_vector.size()];
    
    for(int i = 0; i < n_grams_vector.size(); i++) {
        for(int j = 0; j < n_grams_vector[i].second.size()-1; j++) {
            distances[i].push_back(n_grams_vector[i].second[j+1] - n_grams_vector[i].second[j]);
        }
    }
    // cout << "----------------------------------------------------" << endl;

    // cout << "all distances: " << endl;
    // for(auto& d : distances) {
    //     for(auto& d_i : d) {
    //         cout << d_i << " ";
    //     }
    //     cout << endl;
    // }
    // cout << "----------------------------------------------------" << endl;

    unordered_map<int, int> gcd_count;

    for(int i = 0; i < n_grams_vector.size(); i++) {
        for(int j = 0; j < distances[i].size()-1; j++) {
            gcd_count[gcd(distances[i][j], distances[i][j+1])]++;
        }
    }

    // for(auto& gcd_count_i : gcd_count) {
    //     cout << gcd_count_i.first << ": " << gcd_count_i.second << endl;
    // }

    vector<pair<int, int>> gcd_count_vector;
    for(auto& gcd_count_i : gcd_count) {
        gcd_count_vector.push_back(make_pair(gcd_count_i.first, gcd_count_i.second));
    }

    sort(gcd_count_vector.begin(), gcd_count_vector.end(), [](pair<int, int> &a, pair<int, int> &b) {
        return a.second > b.second;
    });
    
    int pos = 0;
    while(gcd_count_vector[pos].first == 1) {
        pos++;
    }
    int probable_key_length = gcd_count_vector[pos].first;
    
    cout << "probable key length: " << probable_key_length << endl;
    
}   

void break_key(int key_length) {

}

void break_vigenere(string filename) {

    string cipher;

    read_message(filename, cipher);

    int key_length = break_key_length(cipher, 3);
    break_key(key_length);

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

    // build_frequency_table();

    break_vigenere("Cipher_text.txt");

    // int n, m; cin >> n >> m;
    // cout << gcd(n, m) << endl;

    return 0;
}
