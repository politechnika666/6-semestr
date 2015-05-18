#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <vector>
#include <bitset>

using namespace std;

void writeFile(unsigned char *a);
void ReadFile(char *name);
void initialize(map<vector<unsigned char>,short> *dictionary);
void getFibbo(short number,vector<string> *output);

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
	vector <string> temp;

	cout << "START" << endl;
	/*for(int i = 0; i<length; ++i) {
		cout << (short)buffer[i] << " ";
	}*/
	cout << endl;

	for(int i = 0; i<length; ++i) {
		getFibbo(buffer[i],&temp);
	}
	cout << endl;

	unsigned char output[temp.size()/8+1];
	outLen = temp.size()/8+1;
	int j = 0;
	string temp1 = "";

	cout << endl;
	for(unsigned int i = 0; i<temp.size(); ++i) {
		if(temp1.size() == 8) {
			bitset<8> conv (temp1);
			output[j] = conv.to_ulong();
			j++;
			temp1 = "";
		} 
		temp1 += temp[i];
	}
	if(temp.size() % 8 != 0) {
		temp1 = "";
		for(unsigned int rest = temp.size() - temp.size()%8; rest<temp.size(); ++rest) {
			temp1 += temp[rest];
		}
		for(int i = temp1.size(); i<8; ++i)
			temp1 += "0";
		//cout << endl << temp1 << endl;
		bitset<8> conv (temp1);
		output[j] = conv.to_ulong();
	}


	/*for(unsigned int i = 0; i<temp.size(); ++i)
		cout << temp[i];*/
	cout << endl;
	/*for(unsigned int i = 0; i<outLen; ++i)
		cout << (int)output[i] << " ";*/
	cout << endl;
	writeFile(output);
	cout << outLen << endl;
	cout << "KONIEC" << endl;
	return 0;
}

void getFibbo(short number,vector<string> *output) {
	short fibb[22];
	vector <string> temp;

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

	int max = 0;
	for(int i = 0; i< 22; ++i) {
		if(fibb[i] <= number)
			max = i;
		else
			break;
	}

	for(int i = max; i >= 0; i--) {
    	if(number >= fibb[i]) {
      		temp.insert(temp.begin(),"1");
      		number -= fibb[i];
    	}
    	else 
    		temp.insert(temp.begin(),"0");   	
  	}
	temp.insert(temp.end(),"1");
  	for(unsigned int i = 0; i<temp.size(); ++i)
  		(*output).push_back(temp[i]);
}
void writeFile(unsigned char *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/7/fibboEncoded", "w+");
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
	