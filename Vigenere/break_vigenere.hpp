#ifndef BREAK_VIGENERE_HPP
#define BREAK_VIGENERE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "vigenere.hpp"

using std::string;
using std::vector;
using std::pair;
using std::unordered_map;
using std::cout;
using std::endl;
using std::sort;

extern unordered_map<char, double> portuguese_table;
extern unordered_map<char, double> english_table;

enum languages {ENGLISH, PORTUGUESE};

string get_normalized_cipher(string cipher);

vector<pair<string, vector<int>>> get_ngram_vector(unordered_map<string, vector<int>> n_grams_positions);

vector<vector<int>> get_distances(vector<pair<string, vector<int>>> n_grams_vector);

int break_key_length(string cipher, int n);

vector<double> get_gama(unordered_map<char, double> letter_count, int language);

char get_shift(vector<char> coset, int language);

string break_key(string cipher, int key_length, int language);

void break_vigenere(string filename, int language);

#endif // BREAK_VIGENERE_HPP