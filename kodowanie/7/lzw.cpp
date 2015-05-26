#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <math.h>
#include <string>
#include <map>
using namespace std;

int pos = 7;
char lsbOne = 0x01;
char msbChar = 0x80;
vector<char>result;
char encByte = 0x00;
bool notFinished;


static vector<char> ReadFile(const char* filename) {
	ifstream ifs(filename, ios::binary|ios::ate);
    ifstream::pos_type pos = ifs.tellg();

    std::vector<char>  result(pos);

    ifs.seekg(0, ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

void sendBit(bool x) {
	if(x) {
		if(pos > 0) {
			encByte = encByte | (lsbOne << pos);
			pos--;
		}
		else {
			encByte = encByte | lsbOne;
			result.push_back(encByte);
			encByte = 0x00;
			pos = 7;
		}
	}
	else {
		if(pos > 0) {
			pos--;
		}
		else {
			result.push_back(encByte);
			encByte = 0x00;
			pos = 7;
		}
	}
}

void send(string x) {
	for(unsigned int i = 0; i < x.size(); i++) {
		if(x[i] == '0')
			sendBit(false);
		else
			sendBit(true);
	}
}

string bytesToString(vector<char>bytes, int offset) {
	int end = 8;
	string result ="";
	for(unsigned int i = 1; i < bytes.size(); i++) {
		if(i == bytes.size()-1)
			end = offset;
		char byte = bytes.at(i);
		for(int j = 0; j < end; j++) {
			if(j == 7) {
				if((lsbOne & byte) > 0)
					result +="1";
				else
					result += "0";
			}
			else {
				if((byte & (lsbOne << (7-j))) > 0)
					result += "1";
				else	
					result += "0";
			}
		}

	}
	return result;
}

int stringToInt(string x) {
	int result = 0;
	int temp = 1;
	for(int i = x.size()-1; i >= 0; i--) {
		if(x.at(i) == '1')
			result += temp;
		temp *= 2;
	}
	return result;
}

string intToString(int x) {
	if(x == 0)
		return "0";
	else {
		string result = "";
		int temp = 1;
		while(temp <= x)
			temp *= 2;
		temp /= 2;
		while(temp > 0) {
			if(temp <= x) {
				result.push_back('1');
				x -= temp;
			}
			else {
				result.push_back('0');			
			}
			temp /= 2;
		}
		return result;
	}
}
int main(int argc, char* argv[]) {

	if(argc != 5) {
		cout << "Call example: ./lzw enc inputfile outputfile" << endl;
		return 0;
	}
	bool encode;
	if(string(argv[1]) == "enc") { 
		encode = true;
	}
	else if(string(argv[1]) == "dec") {
		encode = false;
	}
	else {
		cout << "Type enc for encoding and dec for decoding as first argument" << endl;
		return 0;		
	}
	int fib [30] = {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025, 121393, 196418, 317811, 514229, 832040, 1346269};
	int fibnum = 30;
	const char * inputFile = argv[3];
	const char * outputFile = argv[4];
	vector<char> bytes = ReadFile(inputFile);
	string n,k, prefix,encoded,temp;
	int k1, x1;
	if(encode) {
			typedef map<string,int> dictionary_t;
			dictionary_t dictionary;
			for(unsigned int chars = 0; chars < 256;chars++)
				dictionary.insert(pair<string,int>(string(1,chars),chars+1));
			
			unsigned int maxsize;
			string current,x;
			int ctr = 257;
		for(unsigned int i = 0; i < bytes.size(); i++) {	
			current = string(1,bytes.at(i));
			maxsize = 1;
			while(dictionary.count(current) > 0 && i+maxsize < bytes.size()) {
				current += string(1,bytes.at(i+maxsize));
				maxsize++;			
			}
			if(maxsize > 1) {
				current.erase(current.size()-1);
				maxsize--;
			}
			if(maxsize > 0) {			// jest prefix			
				if(string(argv[2]) == "gamma") { 
					x = intToString(dictionary[current]);
					n = string (x.size()-1,'0');
					encoded = n+x;
				}
				if(string(argv[2]) == "delta") {
					x = intToString(dictionary[current]);
					n = intToString(x.size());
					k = string (n.size()-1,'0');
					encoded = k + n + x.substr(1);
				}
				if(string(argv[2]) == "fibbonacci") {
					x1 = dictionary[current];
					int it = fibnum-1;
					encoded = "1";
					while(fib[it] > x1)
						it--;
					while(x1 > 0) {
						for(int j = it; j >= 0; j--) {
							if(fib[j] <= x1) {
								x1 -= fib[j];
								encoded = "1" + encoded;
							}			
							else {
								encoded = "0" + encoded;
							}					
						}
					}
				}
				if(string(argv[2]) == "omega") {
					x1 = dictionary[current];
					encoded = "0";
					k1 = x1;
					while (k1 > 1) {
						k = intToString(k1);
						k1 = k.size() - 1;
						encoded = k + encoded;	
					}
				}
				send(encoded);
				//cout <<current+string(1,bytes[i+maxsize]) << endl;
				dictionary.insert(pair<string,int>(current+string(1,bytes[i+maxsize]),ctr));
				ctr++;
				i += (maxsize-1);
			}
			else {						// nie ma
				cout << "pszypau" << endl;

			}
			
		}
		
		if(pos < 7) { // offset
			result.push_back(encByte);
			encByte = 0x00;
			for(; pos <= 6; pos++) {
				encByte = encByte | msbChar;
				msbChar = msbChar >> 1;
			}
			result.insert(result.begin(),encByte);
			encByte = 0x00;
			msbChar = 0x80; 
		}
		else if(pos == 7) {
			result.insert(result.begin(),0xFF);
		}
		else {
			cout << "Something wrong happened" << endl;
		}

		char arr[result.size()];
		std::copy(result.begin(), result.end(), arr);
		std::ofstream file(outputFile, std::ios::binary);
	 	file.write(arr, result.size());
	}
	else {
		char decByte = bytes.at(0);	
		unsigned int offset = 0;
		while(decByte != 0x00) {
			offset++;
			decByte = decByte << 1;
		}
		map<int,string> dictionary;
		for(unsigned int chars = 0; chars < 256;chars++)
			dictionary.insert(pair<int,string>(chars+1,string(1,chars)));
		string current ="";
		string newWord, temp;
		bool hasNewWord = false;
		int ctr = 257;
		int knum = 0;
		int nnum = 0;
		int decode, n1;
		unsigned int i = 0;
		
		string data = bytesToString(bytes, offset);		// bez pierwszego bajtu
//cout << data.size() << endl;
		while(i < data.size()) {
			if(string(argv[2]) == "gamma") { 
				if(data.at(i) == '0') {
					nnum++;
					i++;
				}		
				else {
					current = "";
					for(int j = 0; j <= nnum;j++) {
						
						current += data.at(i+j);
					}
					i++;
					i += nnum;
					nnum = 0;
					decode = stringToInt(current);
					current = "";
					if(decode < ctr) {		// jest w slowniku
						for(unsigned int m = 0; m < dictionary[decode].size();m++) {
							result.push_back(dictionary[decode].at(m));
						}		
						if(hasNewWord) {		// jest jakis newWord
							newWord += dictionary[decode].at(0);
							dictionary.insert(pair<int,string>(ctr,newWord));
							ctr++;
						}	
						newWord = dictionary[decode];	
						hasNewWord = true;	
					}
					else {
						newWord += newWord.at(0);
						for(unsigned int m = 0; m < newWord.size();m++) {
							result.push_back(newWord.at(m));
						}
						dictionary.insert(pair<int,string>(ctr,newWord));
						ctr++;
					}
				}			
			}
			if(string(argv[2]) == "delta") {
				if(data.at(i) == '0') {
					knum++;
					i++;
				}		
				else {
					n = "";
					for(int j = 0; j <= knum;j++) {	
						n += data.at(i+j);
					}
					i++;
					i += knum;	
					nnum = stringToInt(n);
					knum = 0;
					current = "1";
					for(int j = 0; j < nnum-1; j++)	
						current += data.at(i+j);
					i += nnum-1;
					decode = stringToInt(current);
					if(decode < ctr) {		// jest w slowniku
						for(unsigned int m = 0; m < dictionary[decode].size();m++) {
							result.push_back(dictionary[decode].at(m));
						}		
						if(hasNewWord) {		// jest jakis newWord
							newWord += dictionary[decode].at(0);
							dictionary.insert(pair<int,string>(ctr,newWord));
							ctr++;
						}	
						newWord = dictionary[decode];	
						hasNewWord = true;	
					}
					else {
						newWord += newWord.at(0);
						for(unsigned int m = 0; m < newWord.size();m++) {
							result.push_back(newWord.at(m));
						}
						dictionary.insert(pair<int,string>(ctr,newWord));
						ctr++;
					}
				}	
			}
			if(string(argv[2]) == "fibbonacci") {
				decode = 0; 
				int it = 0;
				while(!(data.at(i) == '1' && data.at(i+1) == '1')) {
					if(data.at(i) == '1')
						decode += fib[it];
					it++;
					i++;
				}		
				decode += fib[it];
				i += 2;	
				if(decode < ctr) {		// jest w slowniku
					for(unsigned int m = 0; m < dictionary[decode].size();m++) {
						result.push_back(dictionary[decode].at(m));
					}		
					if(hasNewWord) {		// jest jakis newWord
						newWord += dictionary[decode].at(0);
						dictionary.insert(pair<int,string>(ctr,newWord));
						ctr++;
					}	
					newWord = dictionary[decode];	
					hasNewWord = true;	
				}
				else {
					newWord += newWord.at(0);
					for(unsigned int m = 0; m < newWord.size();m++) {
						result.push_back(newWord.at(m));
					}
					dictionary.insert(pair<int,string>(ctr,newWord));
					ctr++;
				}
			}
			if(string(argv[2]) == "omega") {
				n1 = 1;
			//	cout << i << endl;
				while(data.at(i) != '0') {
					temp = "";
					for(int j = 0; j <= n1; j++) {
						temp += data.at(i+j);
					}
					i += n1+1;
					n1 = stringToInt(temp);
				}
				i++;
				decode = n1;
				current = "";
				if(decode < ctr) {		// jest w slowniku
					for(unsigned int m = 0; m < dictionary[decode].size();m++) {
						result.push_back(dictionary[decode].at(m));
					}		
					if(hasNewWord) {		// jest jakis newWord
						newWord += dictionary[decode].at(0);
						dictionary.insert(pair<int,string>(ctr,newWord));
						ctr++;
					}	
					newWord = dictionary[decode];	
					hasNewWord = true;	
				}
				else {
					newWord += newWord.at(0);
					for(unsigned int m = 0; m < newWord.size();m++) {
						result.push_back(newWord.at(m));
					}
					dictionary.insert(pair<int,string>(ctr,newWord));
					ctr++;
				}
			}
			
		}
		char arr[result.size()];
		std::copy(result.begin(), result.end(), arr);
		std::ofstream file(outputFile, std::ios::binary);
	 	file.write(arr, result.size());
	}
	return 0;
}
