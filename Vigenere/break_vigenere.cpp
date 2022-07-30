#include "break_vigenere.hpp"


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

    // while(n_grams_vector.back().second.size() <= n_grams_vector.front().second.size() / 2) {
    //     n_grams_vector.pop_back();
    // }

    // for(auto& n_gram : n_grams_vector) {

    //     cout << n_gram.first << ": ";
    //     for(auto& p : n_gram.second) cout << p << " ";
    //     cout << endl;
    // }
    // cout << "----------------------------------------------------" << endl;

    return n_grams_vector;
}

vector<vector<int>> get_distances(vector<pair<string, vector<int>>> n_grams_vector) {

    vector<vector<int>> distances(n_grams_vector.size());

    for(int i = 0; i < n_grams_vector.size(); i++) {
        for(int j = 0; j < n_grams_vector[i].second.size()-1; j++) {
            distances[i].push_back(n_grams_vector[i].second[j+1] - n_grams_vector[i].second[j]);
        }
    }

    // print distances
    // for(int i = 0; i < distances.size(); i++) {
    //     cout << n_grams_vector[i].first << ": ";
    //     for(auto& d : distances[i]) cout << d << " ";
    //     cout << endl;
    // }

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
            // cout << distances[i][j] << " " << distances[i][j+1] << endl;
            gcd_count[gcd(distances[i][j], distances[i][j+1])]++;
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

    // for(auto& gcd_count_i : gcd_count) {
    //     cout << gcd_count_i.first << ": " << gcd_count_i.second << endl;
    // }
    
    cout << "probable key length: " << prob_key_len << endl;

    cout << "----------------------------------------------------" << endl;

    return prob_key_len;    
}   

vector<double> get_gama(unordered_map<char, double> letter_count, int language) {

    vector<double> gama_vector;
    
    for(int i = 0; i < 26; i++) {
        double gama = 0;

        for(auto& lc : letter_count) {

            char shifted =((lc.first - 97) - i + 26) % 26 + 97;
            // cout << lc.first << " shifted by " << (char)(i+97) << " = " << shifted << endl;
            // cout << lc.first << " : " <<lc.second << " vs " << shifted << " : "<< english_table[shifted] << endl;
            // cout << "----------------------------------------------------" << endl;

            double aux;
            if(language == PORTUGUESE) {
                aux = (lc.second - portuguese_table[shifted]) * (lc.second - portuguese_table[shifted]);
                aux /= english_table[shifted];
            }

            else if(language == ENGLISH) {
                aux = (lc.second - english_table[shifted]) * (lc.second - english_table[shifted]);
                aux /= english_table[shifted];
            }

            gama += aux;
        }
        // cout << "shifted by " << (char)(i + 97) << ": " << gama << endl;
        gama_vector.push_back(gama);
    }

    return gama_vector;
}

char get_shift(vector<char> coset, int language) {

    unordered_map<char, double> letter_count;

    // for(auto& c : coset) {
    //     cout << c << " ";
    // }
    // cout << endl;

    for(auto& c : coset) {
        letter_count[c]++;
    }

    // for(auto& l : letter_count) {
    //     cout << l.first << ": " << l.second << endl;
    // }

    for (auto& lc : letter_count) {
        lc.second /= coset.size();
    }

    // for(auto& lc : letter_count) {
    //     cout << lc.first << ": " << lc.second << endl;
    // }
    
    vector<double> gama_vector = get_gama(letter_count, language);

    // for (auto& g : gama_vector) {
    //     cout << g << " ";
    // }
    // cout << endl;


    // //get index of smallest element n gama_vector
    int index = 0;
    for(int i = 0; i < gama_vector.size(); i++) {
        if(gama_vector[i] < gama_vector[index]) {
            index = i;
        }
    }

    return (char)(index + 97);
}

string break_key(string cipher, int key_length, int language) {

    // cout << cipher << endl;
    cipher = get_normalized_cipher(cipher);
    // cout << cipher << endl;

    vector<char> cosets[key_length];

    for(int i = 0; i < key_length; i++) {
        for(int j = 0; j < cipher.length(); j += key_length) {
            if(cipher[j+i] < 97 || cipher[j+i] > 122) continue;
            cosets[i].push_back(cipher[j+i]);
        }
    }
    string key = "";
    // get_shift(cosets[0]);
    for(int i = 0; i < key_length; i++) {
        key += get_shift(cosets[i], language);
    }
    // cout << "key: " << key << endl;
    return key;
}

void break_vigenere(string filename, int language) {

    string cipher;

    read_message(filename, cipher);

    int key_length = break_key_length(cipher, 4);

    string key = break_key(cipher, key_length, language);

    cout << "Probable key: " << key << endl;
    cout << "Decrypted message: " << vigenere(cipher, key, DECRYPT) << endl;
}
