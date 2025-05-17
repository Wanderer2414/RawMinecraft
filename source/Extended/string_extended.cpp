#include "string_extended.h"

void filter(string& string, const char& c) {
    for (int i = string.size(); i>=0; i--) 
        if (string[i] == c) string.erase(string.begin() + i);
}
void to_lower(string& string) {
    for (int i = 0; i<string.size(); i++) 
        if (string[i]>='A' && string[i]<='Z') {
            string[i] = string[i]-'A'+'a';
        }
}