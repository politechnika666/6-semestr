#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>

using namespace std;

void writeFile(short *a);
void ReadFile(char *name);
void initialize(map<vector<unsigned char>,short> *dictionary);
void encode(map<vector<unsigned char>,short> *dictionary, vector<int> *output);

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
	map<vector<unsigned char>,short> dictionary;
	vector <int> output;

	cout << "START" << endl;
	initialize(&dictionary);
	encode(&dictionary,&output);

	/*map<vector<unsigned char>,short>::iterator it = dictionary.begin();
		for (it=dictionary.begin(); it!=dictionary.end(); ++it) {
			for(vector<unsigned char>::const_iterator i = it->first.begin(); i != it->first.end(); ++i)
    			cout << (short)*i << ' ';
    		cout << it->second << '\n';
    	}*/
    short a[outLen];
    cout << endl;
    for(int i = 0; i<outLen; ++i)
    	a[i] = output[i];

    /*for(int i = 0; i<outLen; ++i)
    	cout << (short)a[i] << " ";*/

	writeFile(a);
	cout << endl << "KONIEC" << endl;
	cout << outLen << endl;
	return 0;
}

void initialize(map<vector<unsigned char>,short> *dictionary) {
	for(int i = 0; i<256; ++i) {
		vector<unsigned char> temp(1,i);
		(*dictionary)[temp] = i+1;
	}
	idPtr = 257;
}

void encode(map<vector<unsigned char>,short> *dictionary, vector<int> *output) {
	vector<unsigned char> word;
	while(buffPtr <= length) {
		word.push_back(buffer[buffPtr]);
		if((*dictionary).count(word)) {
			buffPtr++;
		} else {
			(*dictionary)[word] = idPtr;
			idPtr++;
			word.pop_back();
			(*output).push_back((*dictionary)[word]);
			outLen++;
			word.clear();
		}

		if(idPtr == 32766) {
			(*dictionary).clear();
			initialize(dictionary);
		}
	}
}

void writeFile(short *a) { 
    FILE* pFile;
    pFile = fopen("/home/afro/Dokumenty/repo/6-semestr/kodowanie/7/lzwEncoded", "w+");
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
    fread(buffer, fileLen, 1, file);
}
	