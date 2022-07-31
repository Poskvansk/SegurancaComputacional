#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

#define ENCRYPT 1
#define DECRYPT 0

enum languages {ENGLISH, PORTUGUESE};

unordered_map<char, double> portuguese_table;
unordered_map<char, double> english_table;

void build_frequency_table() {

    // Builds the frequency table for the alphabet in portuguese
    ifstream file("frequency_table.txt");
    string line;

    enum languages language = PORTUGUESE;

    while (getline(file, line)) {

        if(line == "English") language = ENGLISH;

        if(line.length() <= 1) continue;
        if(line == "Portuguese" || line == "English") continue;

        if(language == PORTUGUESE) {
            portuguese_table[line[0]] = stod(line.substr(2)) / 100;
        }
        else if (language == ENGLISH) {
            english_table[line[0]] = stod(line.substr(2)) / 100;
        }
    }
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

    string extended_key;
    int pos = 0;

    normalize(message);
    normalize(key);

    for(size_t i = 0; i < message.length(); i++) {

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
            }
            else {
                cipher += ((message[i]-97 + key[i]-97) % 26) + 97;
            }
        }
    }

    else {            
        for (size_t i = 0; i < message.length(); i++) {

            if(message[i] < 97 || message[i] > 122) {
                cipher += message[i];
            }
            else {
                cipher += (( (message[i]-97) - (key[i]-97) + 26) % 26) + 97;
            }
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

string get_normalized_cipher(string cipher) {
    
    string tmp;

    normalize(cipher);
    
    for(auto& c : cipher) {
        if (c < 97 || c > 122) continue;
        if(c >= 'A' && c <= 'Z') c += 32;
        tmp += c;
    }

    return tmp;
}

vector<pair<string, vector<int>>> get_ngram_vector(unordered_map<string, vector<int>> n_grams_positions) {

    vector<pair<string, vector<int>>> n_grams_vector;

    for(auto& n_gram : n_grams_positions) {
        n_grams_vector.push_back(make_pair(n_gram.first, n_gram.second));
    }

    sort(n_grams_vector.begin(), n_grams_vector.end(), [](pair<string, vector<int>> &a, pair<string, vector<int>> &b) {
        return a.second.size() > b.second.size();
    });

    while(n_grams_vector.back().second.size() == 1) {
        n_grams_vector.pop_back();
    }

    return n_grams_vector;
}

vector<vector<int>> get_distances(vector<pair<string, vector<int>>> n_grams_vector) {

    vector<vector<int>> distances(n_grams_vector.size());

    for(int i = 0; i < n_grams_vector.size(); i++) {
        for(int j = 0; j < n_grams_vector[i].second.size()-1; j++) {
            distances[i].push_back(n_grams_vector[i].second[j+1] - n_grams_vector[i].second[j]);
        }
    }

    return distances;
}

int break_key_length(string cipher, int n) {
    
    unordered_map<string, vector<int>> n_grams_positions;

    string tmp = get_normalized_cipher(cipher);    

    for(int i = 0; i < tmp.length() - n; i += n) {
        n_grams_positions[tmp.substr(i, n)].push_back(i);
    }

    vector<pair<string, vector<int>>> n_grams_vector = get_ngram_vector(n_grams_positions);

    vector<vector<int>> distances = get_distances(n_grams_vector);

    unordered_map<int, int> gcd_count;

    for(int i = 0; i < distances.size(); i++) {

        if(distances[i].size() <= 1) continue;

        for(int j = 0; j < distances[i].size()-1; j++) {
            gcd_count[gcd(distances[i][j], distances[i][j+1])]++;
        }
    }

    if(gcd_count.size() == 0) {

        for(size_t i = 0; i < distances.size()-1; i++) {

            int a = distances[i][0];
            int b = distances[i+1][0];

            gcd_count[gcd(a, b)]++;
        }
    }

    int max_count = 0;
    int prob_key_len;
    for(auto& g : gcd_count) {
        if(g.second > max_count) {
            max_count = g.second;
            prob_key_len = g.first;
        }
    }
  
    cout << "probable key length: " << prob_key_len << endl;
    cout << "----------------------------------------------------" << endl;

    return prob_key_len;    
}   

vector<double> get_chi(unordered_map<char, double> letter_count, enum languages language) {

    vector<double> chi_vector;

    if(language != ENGLISH) language = PORTUGUESE;

    for(int i = 0; i < 26; i++) {
        double chi = 0;

        for(auto& lc : letter_count) {

            char shifted =((lc.first - 97) - i + 26) % 26 + 97;

            double aux;
            if(language == PORTUGUESE) {
                aux = (lc.second - portuguese_table[shifted]) * (lc.second - portuguese_table[shifted]);
                aux /= english_table[shifted];
            }

            else if(language == ENGLISH) {
                aux = (lc.second - english_table[shifted]) * (lc.second - english_table[shifted]);
                aux /= english_table[shifted];
            }

            chi += aux;
        }
        chi_vector.push_back(chi);
    }

    return chi_vector;
}

char get_shift(vector<char> coset, enum languages language) {

    unordered_map<char, double> letter_count;

    for(auto& c : coset) {
        letter_count[c]++;
    }

    for (auto& lc : letter_count) {
        lc.second /= coset.size();
    }

    vector<double> chi_vector = get_chi(letter_count, language);

    // //get index of smallest element n chi_vector
    int index = 0;
    for(int i = 0; i < chi_vector.size(); i++) {
        if(chi_vector[i] < chi_vector[index]) {
            index = i;
        }
    }

    return (char)(index + 97);
}

string break_key(string cipher, int key_length, enum languages language) {

    cipher = get_normalized_cipher(cipher);

    vector<char> cosets[key_length];

    for(int i = 0; i < key_length; i++) {

        for(int j = 0; j < cipher.length(); j += key_length) {

            if(cipher[j+i] < 97 || cipher[j+i] > 122) {
                continue;
            }

            cosets[i].push_back(cipher[j+i]);
        }
    }

    string key = "";

    for(int i = 0; i < key_length; i++) {
        key += get_shift(cosets[i], language);
    }

    return key;
}

string break_vigenere(string cipher, int n, enum languages language) {

    int key_length = break_key_length(cipher, n);

    string key = break_key(cipher, key_length, language);

    cout << "Probable key: " << key << endl;

    return vigenere(cipher, key, DECRYPT);
}

void user_interface() {

    bool run = true;

    while(run) {

        cout << "\n\n\n";
        cout << "----------------------------------------------------" << endl;

        cout << "Do you want to decrypt the message? (y/n)" << endl;
        char answer;
        cin >> answer;
        
        if(answer == 'y') {
    
            string cipher;
            read_message("Cipher_text.txt", cipher);

            cout << "What is the language of the message? (1: Portuguese, 2: English)" << endl;
            int option;
            cin >> option;
            cout << '\n';
        
            cout << "What chunk size do you want to use?" << endl;
            int chunk_size;
            cin >> chunk_size;
            cout << '\n';

            string break_cipher = break_vigenere(cipher, chunk_size, (languages)option);

            cout << "Decrypted message: \n" << break_cipher << endl;

            // string message;
            // read_message("Plain_text.txt", message);
            // normalize(message);
            // bool ans = message == break_cipher;
            // cout << ans << endl;

            cout << "Does it look like the message was decrypted? (y/n)" << endl;
            char again = 'n';
            cin >> again;
            if (again == 'y') run = false;
        }

        else if(answer == 'n') {
            run = false;
        }

        else {
            cout << "Invalid option" << endl;
            getchar();
        }

        cout << "----------------------------------------------------" << endl;
    }
}

int main(int argc, char const *argv[]) {
    
    // string filename = "Plain_text.txt";
    string filename = "english_text.txt";
    string message;

    read_message(filename, message);

    string key;
    cout << "Enter key: ";
    cin >> key;
    cout << endl;

    string key = "abcdrfg";

    string cipher = vigenere(message, key, ENCRYPT);

    write_cipher("Cipher_text.txt", cipher);

    build_frequency_table();

    user_interface();

    return 0;
}
