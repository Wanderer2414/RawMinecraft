#include "file_extended.h"

void connect(fstream& stream, const string& file) {
    while (!stream.is_open()) {
        stream.open(file, ios::in | ios::out | ios::binary);
        if (!stream.is_open()) {
            cout << "Connect " + file + " failed!" << endl;
            cout << "Creating " + file + " file..." << endl;
            stream.close();
            stream.open(file, ios::out);
            stream.close();
            cout << "Reconnect..." << endl;
        }
    }
    cout << "Connect " + file + " successfully!" << endl;
}

void close(fstream& stream, const string& file){
    stream.close();
    cout << "Disconnect file " + file + "!" << endl;
}