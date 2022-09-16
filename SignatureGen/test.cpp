#include <iostream>
#include <vector>
#include <bitset>

using namespace std;

char ctob64(uint x) {

    if (x <= 25) return x + 65;
    if (x <= 51) return x + 71;
    if (x <= 61) return x - 4;
    if (x == 62) return 43;
    if (x == 63) return 47;

    return 0;
}

// CORRIGIR PARA NUMERO DE BITS
// COLOCAR = QUANDO FALTAR BITS
void base64_decoder() {
    return;
}

void base64_encoder(string data) {

    // if(data.length() % 3 != 0) {
    //     data = padding(data);
    // }

    string result;

    for (int i = 0; i < data.length(); i+=3) {

        char c;
        string slice = data.substr(i, 3);

        char encoded[4];
        encoded[0] = (slice[0] & 0xFC) >> 2;
        encoded[1] = ((slice[0] & 0x03) << 4) | ((slice[1] & 0xF0) >> 4);
        encoded[2] = ((slice[1] & 0x0F) << 2) | ((slice[2] & 0xC0) >> 6);
        encoded[3] = slice[2] & 0x3F;

        for (int j = 0; j < 4; j++) {
            encoded[j] = ctob64(encoded[j]);
        }
        
        if(slice.length() % 3 == 1) {
            encoded[2] = '=';
            encoded[3] = '=';
        }
        else if(slice.length() % 3 == 2) {
            encoded[3] = '=';
        }

        result += encoded[0];
        result += encoded[1];
        result += encoded[2];
        result += encoded[3];
    }

    cout << result << endl;   

}

int main() {

    string data = "Man is distinguished, not only by his reason";
    cout << data.length() << endl;
    base64_encoder(data);

    return 0;
}