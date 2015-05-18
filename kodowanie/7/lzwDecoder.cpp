#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace std;

void writeFile(unsigned char *a);
void ReadFile(char *name);
void initialize(map<short,vector<unsigned char> > *dictionary);
void decode(map<short,vector<unsigned char> > *dictionary, vector<unsigned char> *output);

long length;
short *buffer;
long buffPtr = 0;
//long outPtr = 0;
long outLen = 0;
short idPtr = 257;
short outputPtr = 0;
int lenA;

int main(int args, char* argv[]) {
    ReadFile(argv[1]);
    map<short,vector<unsigned char> > dictionary;
    vector<unsigned char> output;

    /*for(int i = 0; i<length; ++i)
        cout << buffer[i] << " ";*/

    cout << "START" << endl;
    initialize(&dictionary);
    decode(&dictionary,&output);

    /*map<short,vector<unsigned char> >::iterator it = dictionary.begin();
    for(it=dictionary.begin(); it!=dictionary.end(); ++it) {
        cout << "FIRST: " << it->first;
        for(unsigned int i = 0; i<(it->second).size(); ++i)
            cout << "  SECOND: " << (short)it->second[i] << endl;
    }*/
    unsigned char a[outLen];
    cout << endl;
    
    for(int i = 0; i<outLen; ++i)
        a[i] = output[i];
    cout << endl;
    
    writeFile(a);
    free(buffer);
    cout << "KONIEC" << endl;
    return 0;
}

void initialize(map<short,vector<unsigned char> > *dictionary) {
    for(short i = 0; i<256; ++i) {
        (*dictionary)[i+1] = vector <unsigned char> (1, i);
    }
    idPtr = 257;
}

void decode(map<short,vector<unsigned char> > *dictionary, vector<unsigned char> *output) {
    vector<unsigned char> w;   
    vector<unsigned char> entry;
    map<short,vector<unsigned char> >::iterator it = (*dictionary).find(buffer[buffPtr]);
    w = it->second;
    (*output).push_back(w[0]);
    buffPtr++;
    outLen++;
    
    while(buffPtr != length) {
        if((*dictionary).count(buffer[buffPtr])) {
            entry = (*dictionary)[buffer[buffPtr]];
        } else if(buffer[buffPtr] == idPtr) {
            for(unsigned int i = 0; i<w.size(); ++i)
                entry.push_back(w[i]);
        } else
            throw "COS NIE SMIGA";

        for(unsigned int i = 0; i<entry.size(); ++i) {
            outLen++;
            (*output).push_back(entry[i]);
        }
        w.push_back(entry[0]);
        (*dictionary)[idPtr++] = w;
        w = entry;
        buffPtr++;
        if(idPtr >= 32767) {
            (*dictionary).clear();
            initialize(dictionary);
        }
    }
}

void writeFile(unsigned char *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/7/lzwDecoded", "w+");
    fwrite(a, outLen, 1, pFile);
}

// Reading binary files function
void ReadFile(char *name) {
    FILE *file;
    unsigned long fileLen;
    //Open file
    file = fopen(name, "rb");
    if (!file) {
        fprintf(stderr, "Unable to open file %s", name);
        return;
    }   
    //Get file length
    fseek(file, 0, SEEK_END);
    fileLen=ftell(file);
    fseek(file, 0, SEEK_SET);
    //Allocate memory
    buffer=(short *)malloc(fileLen+1);
    if (!buffer) {
        fprintf(stderr, "Memory error!");
        fclose(file);
        return;
    }
    //Read file contents into buffer
    //length = fileLen;
    length = fread(buffer, sizeof(*buffer), fileLen, file);
    cout << "LENGTH: " << length << endl;
}
    