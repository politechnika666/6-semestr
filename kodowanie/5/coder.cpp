#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <bitset>
#include <math.h>

using namespace std;

unsigned char *buffer;

long length;
unsigned int actPos = 0;
unsigned int ptrA = 0;
unsigned int lenA = 0;

void writeFile(unsigned char *a);
void ReadFile(char *name);
short getPrefix(short *dictionary, short *coder, short * k);
void shiftCoder(short *dictionary, short *coder);
void shiftDictionary(short *dictionary,short *coder);
void initialize(short *dictionary,short *coder);
 
int main(int args, char* argv[])
{
	ReadFile(argv[1]);
    unsigned char* a =  (unsigned char*)calloc(1,sizeof(*a));
    short dictionary[256];
    int quantity[256];
    short coder[257];    

    for(int i = 0; i<256; ++i) {
        dictionary[i] = -1;
        coder[i] = -1;
        quantity[i] = 0;
    }

    coder[256] = -1;

    initialize(dictionary,coder);

    short k = 0; // i from task list
    short l = 0; // j from task list
    short i = 0; // iterator

    while(coder[1] != -1) {
        k = 0;
        l = getPrefix(dictionary,coder, &k);

        if(k == 0) {
            a = (unsigned char*)realloc(a, (ptrA+1)*sizeof(*a));
            a[ptrA] = (unsigned char)k;
            ptrA++;
            a[ptrA] = (unsigned char)coder[1];
            ptrA++;
            lenA += 2;
            shiftDictionary(dictionary,coder);
            shiftCoder(dictionary,coder);
        }
        else {
            a = (unsigned char*)realloc(a, (ptrA+1)*sizeof(*a));
            a[ptrA] = (unsigned char)k;
            ptrA++;
            a[ptrA] = (unsigned char)l;
            ptrA++;
            lenA += 2;
            i = 0;
            do {
                shiftDictionary(dictionary,coder);
                shiftCoder(dictionary,coder);
                i++;
            }while(i<l);
        }
    }
    
    for(int i = 0; i<length; ++i) {
        quantity[(int)buffer[i]]++;
    }
    

    free(buffer);

    double proba = 0.0;
    double entrophy = 0.0;    

    for(int i = 0; i<256; ++i) {
        proba = (double)quantity[i]/(double)length;
        if(proba != 0)
            entrophy -= (double)proba*(double)log10(proba);
    }

    entrophy /= (double)log10(2);

    writeFile(a);
    free(a);
    double compressionDeg = (double)lenA/(length*8);
    cout << endl << "Długość wejścia: " << length << endl;
    cout << "Stopień kompresji: " << compressionDeg << endl;
    cout << "Entropia wejścia: " << entrophy << endl;
    cout << endl << "Koniec" << endl;

    return 0;
}

void writeFile(unsigned char *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/5/encoded", "w+");
    fwrite(a, lenA, 1, pFile);
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

short getPrefix(short *dictionary, short *coder, short *k) { // k = i from task list
    short tempI = 0;
    short tempJ = 0;
    short tempLen = 0;
    short j,g;
    for(short i = 1; i<256 && dictionary[i] != -1; ++i){
        g = 1;
        if(dictionary[i] == coder[g]) {
            j = i;
            tempLen = 0;
            while(dictionary[j] == coder[g] && j<256) {
                j++;
                tempLen++;
                g++;
            }
            if(tempLen > tempJ) {
                tempI = i;
                tempJ = tempLen;
            }
        }
    }
    *k = tempI;
    return tempJ; // tempJ = j from task list
}

void shiftDictionary(short *dictionary,short *coder) {
    for(int i = 255; i>1; --i) {      
        dictionary[i] = dictionary[i-1];
    }
    dictionary[1] = coder[1];
}

void shiftCoder(short *dictionary,short *coder) {
    for(int i = 1; i<257; ++i) {
        coder[i] = coder[i+1];
    }
    if(actPos >= length) {
        coder[256] = -1;
    }
    else {
        coder[256] = buffer[actPos];
        actPos++;
    }
}

void initialize(short *dictionary,short *coder) {
    int j = 0;
    for(int i = 1; i<257 && i<=length; ++i) {
        coder[i] = (short)buffer[j];
        actPos++;
        j++;
    }
}