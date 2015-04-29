#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string>
#include <cstring>


using namespace std;

unsigned char *buffer;
long length;
unsigned int actPos = 0;

void writeFile(string encoded);
void ReadFile(char *name);
short getPrefix(short *dictionary, short *coder, short * k);
void shiftCoder(short *coder);
void shiftDictionary(short *dictionary,short *coder);
void initialize(short *dictionary,short *coder,string *decoded);
 
int main(int args, char* argv[]) {
	ReadFile(argv[1]);
    short dictionary[256];
    short coder[257];
    string decoded = "";
    for(int i = 0; i<256; ++i) {
        dictionary[i] = -1;
        coder[i] = -1;
    }
    coder[256] = -1;

    initialize(dictionary,coder,&decoded);

    short k = 0; // i from task list
    short l = 0; // j from task list
    string temp = "";
    char* temp1 = new char[256];

    cout << "START" << endl;
    int j;
    
    while(coder[1] != -1) {
        j = 0;
        if(coder[1] == 0) {
            shiftCoder(coder);
            shiftDictionary(dictionary,coder);
            decoded += (unsigned char)coder[1];
            shiftCoder(coder);
        }
        else if(l != -1){
            k = coder[1];
            shiftCoder(coder);
            l = coder[1];
            shiftCoder(coder);
            for(int i = k; i<l+k; ++i) {
                temp += (unsigned char)dictionary[i];
                temp1[j] = dictionary[i];
                j++;
            }
            for(int i = 0; i<l; ++i) {
                shiftDictionary(dictionary,coder);
                dictionary[1] = (short)temp1[i];
            }

            decoded += temp;
            temp = "";
        }
    }
    writeFile(decoded);
    cout << "Koniec" << endl;

    free(buffer);

    return 0;
}


void writeFile(string decoded) { 
    FILE* pFile;
    unsigned char* a = new unsigned char[decoded.length()+1];
    strcpy((char*)a, decoded.c_str());
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/5/decoded", "w+");
    fwrite(a, decoded.length(), 1, pFile);
    free(a);
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
    buffer=(unsigned char *)malloc(fileLen+1);
	if (!buffer) {
		fprintf(stderr, "Memory error!");
        fclose(file);
		return;
	}
	//Read file contents into buffer
	length = fileLen;
    fread(buffer, fileLen, 1, file);

	fclose(file);
}

void shiftDictionary(short *dictionary,short *coder) {
    for(int i = 255; i>1; --i)
        dictionary[i] = dictionary[i-1];
    dictionary[1] = coder[1];
}

void shiftCoder(short *coder) {
    for(int i = 1; i<256; ++i) {
        coder[i] = coder[i+1];
    }
    if(actPos > length) {
        coder[256] = -1;
    }
    else {
        coder[256] = buffer[actPos];
        actPos++;
    }
}

void initialize(short *dictionary,short *coder, string* decoded) {
    for(int i = 1; i<257 && i<=length; ++i) {
        coder[i] = (short)buffer[actPos];
        actPos++;
    }
}