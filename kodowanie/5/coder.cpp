#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <string>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <bitset>


using namespace std;

unsigned char *buffer;

long length;
unsigned int actPos = 0;
unsigned int ptrA = 0;
unsigned int lenA = 0;

void writeFile(unsigned char *a);
void ReadFile(char *name);
short getPrefix(short *dictionary, short *coder, short * k);
void shiftCoder(short *dictionary, short *coder, short numberOf);
void shiftDictionary(short *dictionary,short *coder, short numberOf);
void initialize(short *dictionary,short *coder);
 
int main(int args, char* argv[])
{
	ReadFile(argv[1]);
    unsigned char* a =  (unsigned char*)calloc(1,sizeof(*a));
    short dictionary[256];
    short coder[257];
    for(int i = 0; i<256; ++i) {
        dictionary[i] = -1;
        coder[i] = -1;
    }
    coder[256] = -1;

    initialize(dictionary,coder);

    short k = 0; // i from task list
    short l = 0; // j from task list
    short i = 0;

    while(coder[1] != -1) {
        k = 0;
        l = getPrefix(dictionary,coder, &k);
            //cout << "(" << k << "," << l << "," << coder[1] << ")" << endl;    
            /*cout << "Dictionary: ";
        for(i = 1; i<256; ++i) {
            if(dictionary[i] != -1)
               cout << dictionary[i] << " ";
        }
        cout << endl << "Coder: ";
        for(i = 1; i<257; ++i) {
            if(coder[i] != -1)
                cout << coder[i] << " ";
        }
        cout << endl << endl;*/
        if(l == 0) {
            a = (unsigned char*)realloc(a, (ptrA+3)*sizeof(*a));
            a[ptrA] = 0;
            ptrA++;
            a[ptrA] = (unsigned char)coder[1];
            ptrA++;
            lenA += 2;
        }
        else {
            a = (unsigned char*)realloc(a, (ptrA+3)*sizeof(*a));
            a[ptrA] = (unsigned char)k;
            ptrA++;
            a[ptrA] = (unsigned char)l;
            ptrA++;
            lenA += 2;

        }
        i = 0;
        do {
                shiftDictionary(dictionary,coder,0);
                shiftCoder(dictionary,coder,0);
                i++;
        }while(i<l);
    }

    cout << "Dlugosc wejscia: " << length*8 << endl;
    writeFile(a);
    cout << "Koniec" << endl;

    free(buffer);
    free(a);

    return 0;
}


void writeFile(unsigned char *a) { 
    FILE* pFile;
    
    //strcpy((char*)a, encoded.c_str());
    //for(int i = 0; i<lenA; ++i)
        //cout << (short)a[i] << " ";

    pFile = fopen("/home/afro/Dokumenty/kodowanie/5/encoded", "w+");
    //cout << "ENCODED LEN: " << encoded.length() << endl;

    fwrite(a, lenA, 1, pFile);
    //free(a);
}

// Reading binary files function

void ReadFile(char *name)
{
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
        //cout << "DICTIONARY:   " << i << "  " << dictionary[i] << "  " << coder[0] << endl;
        g = 1;
        if(dictionary[i] == coder[g]) {
            j = i;
            tempLen = 0;
            //cout << dictionary[j] << "   " << coder[j] << endl;
            while(dictionary[j] == coder[g] && j<256) {
                //cout << dictionary[j] << "  " << coder[j] << endl;
                j++;
                tempLen++;
                g++;
            }
            if(tempLen > tempJ) {
                tempI = i;
                tempJ = tempLen;
            }
            // optimization
            //if(dictionary[254] != -1 && tempj > 128)
                //break;
        }
    }
    *k = tempI;

    return tempJ; // tempJ = j from task list
}

void shiftDictionary(short *dictionary,short *coder, short numberOf) {
    for(int i = 256; i>1; --i) {
        
        dictionary[i] = dictionary[i-1];
        //cout << dictionary[i] << " ";
    }
    //cout << endl;
    dictionary[1] = coder[1];
}

void shiftCoder(short *dictionary,short *coder, short numberOf) {
    for(int i = 1; i<257; ++i) {
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

void initialize(short *dictionary,short *coder) {
    for(int i = 1; i<257 && i<length; ++i) {
        coder[i] = (short)buffer[i-1];
        actPos++;
        //dictionary[i] = coder[0];
    }
}