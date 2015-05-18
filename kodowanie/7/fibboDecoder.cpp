#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

void writeFile(short *a);
void ReadFile(char *name);
void initialize(map<vector<unsigned char>,short> *dictionary);
short decodeFibbo(string binStream);
bitset<8> ToBits(unsigned char byte);

long length;
unsigned char *buffer;
long buffPtr = 0;
//long outPtr = 0;
long outLen = 0;
short idPtr = 257;
short outputPtr = 0;
int lenA;

int main(int args, char* argv[]) {
	ReadFile(argv[1]);
	string binStream = "";
	vector <short> result;

	cout << "START" << endl;
	/*for(int i = 0; i< length; ++i)
		cout << (int)buffer[i] << " ";*/

	for(int i = 0; i<length; ++i) {
		bitset<8> my_bset=ToBits(buffer[i]);
		//out << endl << my_bset;
		binStream += my_bset.to_string();
	}
	//cout << endl << binStream << endl;
	string temp = "";
	for(unsigned int i = 0; i<binStream.size(); ++i) {
		if(binStream[i] == '1' && binStream[i+1] == '1') {
			temp += binStream[i];
			result.push_back(decodeFibbo(temp));
			temp = "";
			i++;
		} else {
			temp += binStream[i];
		}
	}

	//cout << endl << binStream << endl;


	short output[result.size()];
	for(unsigned int i = 0; i<result.size(); ++i)
		output[i] = result [i];

	/*for(unsigned int i = 0; i<result.size(); ++i) {
		cout << output[i] << " ";
	}*/
	outLen = result.size();

	writeFile(output);
	cout << endl << outLen << endl;
	cout << "KONIEC" << endl;
	return 0;
}


bitset<8> ToBits(unsigned char byte)
{
    return bitset<8>(byte);
}

short decodeFibbo(string binStream) {
	short fibb[22], result = 0;

	fibb[0] = 1;
    fibb[1] = 2;
    fibb[2] = 3;
    fibb[3] = 5;
    fibb[4] = 8;
    fibb[5] = 13;
    fibb[6] = 21;
	fibb[7] = 34;
	fibb[8] = 55;
	fibb[9] = 89;
	fibb[10] = 144;
	fibb[11] = 233;
	fibb[12] = 377;
	fibb[13] = 610;
	fibb[14] = 987;
	fibb[15] = 1597;
	fibb[16] = 2584;
	fibb[17] = 4181;
	fibb[18] = 6765;
	fibb[19] = 10946;
	fibb[20] = 17711;
	fibb[21] = 28657;
	//cout << endl << binStream << endl;
	for(unsigned int i = 0; i<binStream.size(); ++i) {
		if(binStream[i] == '1')
			result += fibb[i];
	}

	return result;
}
void writeFile(short *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/7/fibboDecoded", "w+");
    fwrite(a, sizeof(*a), outLen, pFile);
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
    fread(buffer, sizeof(*buffer), fileLen, file);
    cout << "LENGTH: " << length << endl;
}