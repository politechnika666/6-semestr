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

void writeFile(string encoded);
void ReadFile(char *name);
short getPrefix(short *dictionary, short *coder, short * k);
void shiftCoder(short *coder);
void shiftDictionary(short *dictionary,short *coder);
void initialize(short *dictionary,short *coder);
 
int main(int args, char* argv[])
{
	ReadFile(argv[1]);
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
    string temp = "";
    short temp1;
    string decoded = "";

    while(coder[1] != -1) {
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
        if(coder[1] == 0) {
            shiftCoder(coder);
            shiftDictionary(dictionary,coder);
            decoded += (unsigned char)coder[1];
            shiftCoder(coder);
        }
        else {
            k = coder[1];
            shiftCoder(coder);
            l = coder[1];
            shiftCoder(coder);
            //cout << endl << k << "  " << l << endl;
            //cout << endl << dictionary[k] << "  " << dictionary[l] << endl;
            for(int i = k; i<l+k; ++i) {
                temp += (unsigned char)dictionary[i];
                temp1 = dictionary[i];
                //cout << "temp1: " << temp1 << endl;
                shiftDictionary(dictionary,coder);
                dictionary[1] = temp1;
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


void writeFile(string encoded) { 
    FILE* pFile;
    unsigned char* a = new unsigned char[encoded.length()+1];
    strcpy((char*)a, encoded.c_str());

    pFile = fopen("/home/afro/Dokumenty/kodowanie/5/decoded", "w+");
    //cout << "ENCODED LEN: " << encoded.length() << endl;

    fwrite(a, encoded.length(), 1, pFile);
    free(a);
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

void shiftDictionary(short *dictionary,short *coder) {
    for(int i = 256; i>1; --i) {
        
        dictionary[i] = dictionary[i-1];
        //cout << dictionary[i] << " ";
    }
    //cout << endl;
    dictionary[1] = coder[1];
}

void shiftCoder(short *coder) {
    for(int i = 1; i<257; ++i) {
        coder[i] = coder[i+1];
    }
    if(actPos == length) {
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