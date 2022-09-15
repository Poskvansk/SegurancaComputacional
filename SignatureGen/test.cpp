#include <iostream>
#include <vector>

using namespace std;

char ctob64(uint x) {

    if (x <= 25) return x + 65;
    if (x <= 51) return x + 71;
    if (x <= 61) return x - 4;
    if (x == 62) return 43;
    if (x == 63) return 47;

    return 0;
}

void base64(string data) {

    // vector<int> spaces_positions;
    // string aux = "";
    string result;

    // cout << data << endl;
    // for (size_t i = 0; i < data.length(); i++) {
    //     cout << data[i] << " ";
    //     if(data[i] == ' ') {
    //         spaces_positions.push_back(i);
    //     }
    //     else {
    //         cout << "entrou" << endl;
    //         aux += data[i];
    //     }
    // }

    // cout << "aux: " << aux << endl;
    

    for (int i = 0; i < data.length(); i+=3) {

        char c;
        string slice = data.substr(i, 3);
        cout << slice << endl;
    
        char encoded[4];
        encoded[0] = (slice[0] & 0xFC) >> 2;
        encoded[1] = ((slice[0] & 0x03) << 4) | ((slice[1] & 0xF0) >> 4);
        encoded[2] = ((slice[1] & 0x0F) << 2) | ((slice[2] & 0xC0) >> 6);
        encoded[3] = slice[2] & 0x3F;
        
        cout << ctob64(encoded[0]) << endl;
        cout << ctob64(encoded[1]) << endl;
        cout << ctob64(encoded[2]) << endl;
        cout << ctob64(encoded[3]) << endl;
        cout << "--------------------" << endl;

        result += ctob64(encoded[0]);
        result += ctob64(encoded[1]);
        result += ctob64(encoded[2]);
        result += ctob64(encoded[3]);
    }

    // for (int i = 0; i < spaces_positions.size(); i++) {
    //     result.insert(spaces_positions[i], " ");
    // }

    cout << result << endl;   

}

int main() {

    string data = "Man is distinguished, not only by his reason, but by this singular passion from other animals, which is a lust of the mind, that by a perseverance of delight in the continued and indefatigable generation of knowledge, exceeds the short vehemence of any carnal pleasure.";
    base64(data);
    return 0;
}