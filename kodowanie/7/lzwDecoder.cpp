#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace std;

void writeFile(unsigned char *a);
void ReadFile(char *name);
void initialize(map<int,vector<unsigned char> > *dictionary);
void decode(map<int,vector<unsigned char> > *dictionary, vector<unsigned char> *output);

long length;
int *buffer;
long buffPtr = 0;
//long outPtr = 0;
long outLen = 0;
int idPtr = 257;
int outputPtr = 0;
int lenA;



int main(int args, char* argv[]) {
    ReadFile(argv[1]);
    map<int,vector<unsigned char> > dictionary;
    vector<unsigned char> output;

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

void initialize(map<int,vector<unsigned char> > *dictionary) {
    for(int i = 0; i<256; ++i) {
        (*dictionary)[i+1] = vector <unsigned char> (1, i);
    }
    idPtr = 257;
}

void decode(map<int,vector<unsigned char> > *dictionary, vector<unsigned char> *output) {
    int OLD, NEW;
    vector <unsigned char> temp,entry;
    unsigned char C;
    OLD = buffer[buffPtr];
    temp = (*dictionary)[OLD];
    (*output).push_back(temp[0]);

    while(buffPtr <= length) {
        NEW = buffer[buffPtr++];

        if(!(*dictionary).count(NEW)) {
            entry = (*dictionary)[OLD];
            entry.push_back(C);
        } else {
            entry = (*dictionary)[NEW];
        }
<<<<<<< HEAD
        w.push_back(entry[0]);
        (*dictionary)[idPtr++] = w;
        w = entry;
        buffPtr++;
        if(idPtr >= 32766) {
            (*dictionary).clear();
            initialize(dictionary);
=======

        for(unsigned int i = 0; i < entry.size(); ++i) {
            (*output).push_back(entry[i]);
>>>>>>> c948fe257010a3f004783e84cc96d37e08be9626
        }
        C = entry[0];
        OLD.push_back(C);
        (*dictionary)[++idPtr] = OLD;
        OLD = NEW;
    }
}

void writeFile(unsigned char *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/7/lzwDecoded", "w+");
    fwrite(a, outLen, sizeof(*a), pFile);
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
    buffer=(int *)malloc(fileLen+1);
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
    