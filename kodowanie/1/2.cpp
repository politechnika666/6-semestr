#include <iostream>
#include <fstream> 
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std; 

int main(int argc, char* argv[]) {
    
    vector<char> bytes;
    vector<int> count;

    ifstream is (argv[1], ifstream::binary);
    if (is) {
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        cout << "Reading " << length << " characters...\n";
        is.read (buffer,length);
        int bytesNum = 0;
        unsigned char* bytes = new unsigned char[length];

        for(int i = 0; i<length; i++) {
            if(calc.find(buffer[i]) != calc.end())
                calc[buffer[i]]++;
            else {
                
                bytes[bytesNum] = buffer[i];
                bytesNum++;
                calc.insert(calc.end(),pair<char,int>(buffer[i],1));
            }
            if(prefixes.find(buffer[i]) != prefixes.end()) {
                if(prefixes[buffer[i]].find(buffer[i-1]) != prefixes[buffer[i]].end()) {
                    prefixes[buffer[i]][buffer[i-1]]++;
                }
                else {
                    prefixes[buffer[i]].insert(make_pair(buffer[i-1],1));
                }
            }
            else {
                if(i != 0) {
                    pref.insert(make_pair(buffer[i-1],1));
                    prefixes.insert(make_pair(buffer[i],pref));
                }
                else {
                    pref.insert(make_pair('0',1));
                    prefixes.insert(make_pair(buffer[i],pref));
                }
            }


        }
 
        if (is)
          cout << "\nAll characters read successfully.\n";
        else
          cout << "error: only " << is.gcount() << " could be read";
        is.close();
    }
    return 0;
}