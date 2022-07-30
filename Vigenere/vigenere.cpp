#include "vigenere.hpp" 

void normalize(std::string& text) {

    for(size_t i = 0; i < text.length(); i++) {
        if(text[i] >= 'A' && text[i] <= 'Z') {
            text[i] += 32;
        }
    }
}


std::string vigenere(std::string message, std::string key, bool encrypt) {

    if(message.length() == 0) return message;

    if(key.length() == 0) return message;

    std::string extended_key = key;
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

    std::string cipher = "";
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
