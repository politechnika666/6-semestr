#include <iostream>
#include <fstream> 
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>

using namespace std; 

int main(int argc, char* argv[]) {
    map<char, int> calc;
    map<char, map<char, int> > prefixes;
    map<char, double> proba;
    
    ifstream is (argv[1], ifstream::binary);
    if (is) {
        is.seekg (0, is.end);
        int length = is.tellg();
        is.seekg (0, is.beg);

        char * buffer = new char [length];

        cout << "Reading " << length << " characters...\n";
        is.read (buffer,length);
        int bytesNum = 0;
        char* bytes = new char[length];
        if (is)
          cout << endl << "\nAll characters read successfully.\n" << endl;
        else
          cout << "error: only " << is.gcount() << " could be read";
        is.close();

        map<char, int> pref;

        map<char, int>::iterator calcIt;
        map<char, int>::iterator prefIt;
        map<char, double>::iterator probaIt;

        for(int i = 0; i<length; i++) {
            calcIt = calc.find(buffer[i]);
            if(calcIt == calc.end()) {
                bytes[bytesNum] = buffer[i];
                bytesNum++;
                calc.insert(make_pair(buffer[i],1));
            }
            calcIt->second++;
        }
        for(int i = 0; i<bytesNum; i++) {
            for(int j = 0; j<bytesNum; j++) {
                pref.insert(make_pair(bytes[j],0));
            }
            prefixes.insert(make_pair(bytes[i],pref));
        }

        for(int i = 0; i<length; i++) {
            prefIt = prefixes[buffer[i-1]].find(buffer[i]);
            prefIt->second++;
        }
        delete[] buffer;
        
        int i = 0;

        double temp;
        for(map<char, int>::const_iterator it = calc.begin(); it!=calc.end();it++) {
            temp = it->second;
            proba[bytes[i]] = temp/length;
            i++;
        }

        double entropia = 0;
        double entropiaWar = 0;
        double HY = 0;
        double temp1;
        double pxy;
        
        cout << endl << "  ";
        for(int i = 0; i<bytesNum; i++) {
            cout << bytes[i] << " ";
        }
        cout << endl;
        i = 0;
        for(map<char, int>::const_iterator it1 = calc.begin(); it1!=calc.end();it1++) {
            temp1 = it1->second;
            entropia += proba[it1->first]*log2(1/proba[it1->first]);
            printf("%c ",it1->first );
            for(map<char, int>::const_iterator it = prefixes[bytes[i]].begin(); it!=prefixes[bytes[i]].end();it++) {// KOLUMNY
                temp = it->second;
                pxy = temp/temp1;
                cout << pxy << " "; 
                
                if(pxy != 0) {
                    HY += (pxy)*(-log2(pxy));
                }
            }
            cout << endl;
            entropiaWar += proba[it1->first]*HY;
            HY = 0;
            i++;
        }
        cout << endl;
        for(int i = 0; i<bytesNum; i++) {
            cout << bytes[i] << " ";
        }
        cout << endl;
        for(map<char, int>::const_iterator it = calc.begin(); it!=calc.end();it++) {
            cout << it->second << " ";
        }
        cout<< endl;
        for(int i = 0; i<bytesNum; i++) {
            cout << proba[bytes[i]] << " ";
        }
        cout << endl << endl << "  ";
        for(int i = 0; i<bytesNum; i++) {
            cout << bytes[i] << " ";
        }
        cout << endl;
        for(int i = 0; i<bytesNum; i++) {
            printf("%c ",bytes[i]);
            for(map<char, int>::const_iterator it = prefixes[bytes[i]].begin(); it!=prefixes[bytes[i]].end();it++) {
                cout << it->second << " ";
            }
            cout << endl;
        }

        cout << endl;
        //cout << "Ilosc: " << ilosc << endl;
        //cout << "Num of bytes:" << bytesNum << endl;
        //cout << "Length: " << length << endl;
        //cout << "Pstwo prefiksow: " << licznik << endl;
        cout << "Entropia: " << entropia << endl;
        cout << "Entropia warunkowa:" << entropiaWar << endl;
        cout << "Entropia - entropia warunkowa: " << entropia-entropiaWar << endl << endl;
        
        calc.clear();
        prefixes.clear();
        pref.clear();
    }
    return 0;
}