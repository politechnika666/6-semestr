#include <iostream>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <bitset>
#include <map>

using namespace std;

void ReadFile(char *name);

map<unsigned char, string> codes;

unsigned char* buffer;
unsigned long length;

int main(int argc, char* argv[]) {

	ReadFile(argv[1]);
	//for(int i = 0; i<length; ++i)
		//cout << (int)buffer[i] << " ";

	string temp = "";
	string decimal = "";
	string encoded = "";
	for(int i = 0; i<length; ++i) {
		encoded += bitset<8>(buffer[i]).to_string();
	}
	encoded.erase(encoded.end()-1);
	encoded += "0";
	cout << encoded << endl;

	string toDecode = "";

	decimal = "";
	for(int i = 0; i<=32; ++i) {
		decimal += encoded.at(i);
	}

	cout << "DECIMAL " << decimal << endl;
	unsigned long dictionaryLen = bitset<33>(decimal).to_ulong();
	cout << "Dictionary length: " << dictionaryLen << endl;
	for(int i = dictionaryLen; i<length; ++i) {
		toDecode += encoded.at(i);
	}
	
	//cout << "toDecode = " << toDecode << endl;

	temp = "";
	string temp2 = "";
	unsigned char key;
	int temp1,j;
	decimal = "";
	for(int i = 32; i<dictionaryLen; ++i) {
		if(temp.length() < 8) {
			temp += encoded.at(i);
			//cout << "TEMP: " << temp << endl;
		}
		else {
			temp1 = bitset<8>(temp).to_ulong();
			key = (int)temp1;
			//cout << key;
			temp = "";
			for(int k = i+1; k<i+8; ++k) {
				temp += encoded.at(k);
			}
			i += 8;
			temp1 = bitset<8>(temp).to_ulong();
			temp1 += i;
			temp = "";
			for(int j = i; j<temp1; ++j) {
				temp += encoded.at(j);
			}
			codes.insert(pair<unsigned char, string>(key,temp));
			/*cout << "KEY: " << key;
			cout << " LENGTH: " << temp1-i;
			cout << " VALUE: " << temp << endl;*/
			//i += temp1;
			temp = "";
			temp2 = "";
		}
		//cout <<" i = " << i << endl;
	}


	for(map<unsigned char,string>::iterator it = codes.begin(); it!=codes.end(); ++it) {
        cout << (int)it->first << " => " << it->second << '\n';
    }
    //cout << encoded << endl;
	return 0;
}


void ReadFile(char *name)
{
	FILE *file;
	
	unsigned long fileLen;

	//Open file
	file = fopen(name, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s", name);
		return;
	}
	
	//Get file length
	fseek(file, 0, SEEK_END);
	fileLen=ftell(file);
	fseek(file, 0, SEEK_SET);

	//Allocate memory
	buffer=(unsigned char *)malloc(fileLen+1);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!");
                                fclose(file);
		return;
	}

	//Read file contents into buffer
	length = fileLen;
	fread(buffer, fileLen, 1, file);
	fclose(file);
}

//2  0000000000000000000000000100010000000001000010000010000001110101001101000000111110110000100000011110011001000000001110001101101000000100110110011001001110000000
//1  0000000000000000000000000100010000000001000010000010000001110101001101000000111110110000100000011110011001000000001110001101101000000100110110011001001110000000