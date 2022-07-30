#ifndef VIGENRE_HPP
#define VIGENRE_HPP

#include <string>

#define ENCRYPT 1
#define DECRYPT 0

void normalize(std::string& text);

std::string vigenere(std::string plaintext, std::string key, bool encrypt);




#endif // VIGENRE_HPP