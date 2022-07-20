#include <iostream>

using namespace std;

#define ENCRYPT 1
#define DECRYPT 0

string vigenere(string message, string key, bool encrypt) {

    string extended_key = key;
    int pos = 0;

    for(size_t i = key.length(); i < message.length(); i++) {

        if(message[i] == ' ') {
            extended_key += ' ';
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

            if(message[i] == ' ') {
                cipher += ' ';
                continue;
            }
            cipher += ((message[i]-97 + key[i]-97) % 26) + 97;
        }
    }

    else {
            
            for (size_t i = 0; i < message.length(); i++) {
    
                if(message[i] == ' ') {
                    cipher += ' ';
                    continue;
                }
                cipher += (( (message[i]-97) - (key[i]-97) + 26) % 26) + 97;
            }
    }

    return cipher;
}

int main(int argc, char const *argv[])
{
    
    string msessgae = "atacar base sul";

    string key = "limao";

    cout << vigenere(msessgae, key, ENCRYPT) << endl;
    cout << vigenere(vigenere(msessgae, key, ENCRYPT), key, DECRYPT) << endl;

    return 0;
}
