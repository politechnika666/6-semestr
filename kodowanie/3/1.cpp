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
map<unsigned char, string> codes;
map<string, char> codes1;

void ReadFile(char *name);
void printArr(int arr[], int n);
void printCodes(struct MinHeapNode* root, int arr[], int top);
string huffmanDecode(string huffmanCode);
void writeFile(string encoded);
 
// A Huffman tree node
struct MinHeapNode
{
    char data;  // One of the input characters
    unsigned freq;  // Frequency of the character
    struct MinHeapNode *left, *right; // Left and right child of this node
};

MinHeapNode* root;
 
// A Min Heap:  Collection of min heap (or Hufmman tree) nodes
struct MinHeap
{
    unsigned size;    // Current size of min heap
    unsigned capacity;   // capacity of min heap
    struct MinHeapNode **array;  // Attay of minheap node pointers
};
 
// A utility function allocate a new min heap node with given character
// and frequency of the character
struct MinHeapNode* newNode(char data, unsigned freq)
{
    struct MinHeapNode* temp =
          (struct MinHeapNode*) malloc(sizeof(struct MinHeapNode));
    temp->left = temp->right = NULL;
    temp->data = data;
    temp->freq = freq;
    return temp;
}
 
// A utility function to create a min heap of given capacity
struct MinHeap* createMinHeap(unsigned capacity)
{
    struct MinHeap* minHeap =
         (struct MinHeap*) malloc(sizeof(struct MinHeap));
    minHeap->size = 0;  // current size is 0
    minHeap->capacity = capacity;
    minHeap->array =
     (struct MinHeapNode**)malloc(minHeap->capacity * sizeof(struct MinHeapNode*));
    return minHeap;
}
 
// A utility function to swap two min heap nodes
void swapMinHeapNode(struct MinHeapNode** a, struct MinHeapNode** b)
{
    struct MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}
 
// The standard minHeapify function.
void minHeapify(struct MinHeap* minHeap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
 
    if (left < minHeap->size &&
        minHeap->array[left]->freq < minHeap->array[smallest]->freq)
      smallest = left;
 
    if (right < minHeap->size &&
        minHeap->array[right]->freq < minHeap->array[smallest]->freq)
      smallest = right;
 
    if (smallest != idx)
    {
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}
 
// A utility function to check if size of heap is 1 or not
int isSizeOne(struct MinHeap* minHeap)
{
    return (minHeap->size == 1);
}
 
// A standard function to extract minimum value node from heap
MinHeapNode* extractMin(MinHeap* minHeap)
{
    struct MinHeapNode* temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}
 
// A utility function to insert a new node to Min Heap
void insertMinHeap(struct MinHeap* minHeap, struct MinHeapNode* minHeapNode)
{
    ++minHeap->size;
    int i = minHeap->size - 1;
    while (i && minHeapNode->freq < minHeap->array[(i - 1)/2]->freq)
    {
        minHeap->array[i] = minHeap->array[(i - 1)/2];
        i = (i - 1)/2;
    }
    minHeap->array[i] = minHeapNode;
}
 
// A standard funvtion to build min heap
void buildMinHeap(struct MinHeap* minHeap)
{
    int n = minHeap->size - 1;
    int i;
    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}
 
// Utility function to check if this node is leaf
int isLeaf(struct MinHeapNode* root)
{
    return !(root->left) && !(root->right) ;
}
 
// Creates a min heap of capacity equal to size and inserts all character of
// data[] in min heap. Initially size of min heap is equal to capacity
struct MinHeap* createAndBuildMinHeap(unsigned char data[], int freq[], int size)
{
    struct MinHeap* minHeap = createMinHeap(size);
    for (int i = 0; i < size; ++i)
        minHeap->array[i] = newNode(data[i], freq[i]);
    minHeap->size = size;
    buildMinHeap(minHeap);
    return minHeap;
}
 
// The main function that builds Huffman tree
struct MinHeapNode* buildHuffmanTree(unsigned char data[], int freq[], int size)
{
    struct MinHeapNode *left, *right, *top;
 
    // Step 1: Create a min heap of capacity equal to size.  Initially, there are
    // modes equal to size.
    struct MinHeap* minHeap = createAndBuildMinHeap(data, freq, size);
 
    // Iterate while size of heap doesn't become 1
    while (!isSizeOne(minHeap))
    {
        // Step 2: Extract the two minimum freq items from min heap
        left = extractMin(minHeap);
        right = extractMin(minHeap);
 
        // Step 3:  Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted node as
        // left and right children of this new node. Add this node to the min heap
        // '$' is a special value for internal nodes, not used
        top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }
 
    // Step 4: The remaining node is the root node and the tree is complete.
    return extractMin(minHeap);
}


 
// The main function that builds a Huffman Tree and print codes by traversing
// the built Huffman Tree
void HuffmanCodes(unsigned char data[], int freq[], int size)
{
   //  Construct Huffman Tree
   root = buildHuffmanTree(data, freq, size);
   //cout << "OKroot: " << root->data << " " << root->freq << " " << root->left << endl;
    
   // Print Huffman codes using the Huffman tree built above
   int arr[length], top = 0;
   printCodes(root, arr, top);
}
 
// Driver program to test above functions
int main(int args, char* argv[])
{
	ReadFile(argv[1]);
	int numberOf[256] = {0};
	int diffBytes = 0;
	for(int i = 0; i<length; i++) {
		numberOf[(int)buffer[i]]++;
	}

    for(int i = 0; i<256; ++i) {
        //cout << numberOf[i] << " ";
    }

	cout << endl << endl;
	for(int i = 0; i<256; i++) {
		if(numberOf[i] > 0) {
			diffBytes++;
		}
	}

    unsigned char arr[diffBytes];
    int freq[diffBytes];

    for(int i = 0; i<diffBytes; i++)
        freq[i] = 0;

    int j = 0;
    for(int i = 0; i<256; i++) {
		if(numberOf[i] != 0) {
			arr[j] = (char)i;
			freq[j] = numberOf[i];
			//printf(" %c: %d\n",arr[j],freq[j]);
            j++;
		}
	}

    int size = sizeof(arr)/sizeof(arr[0]);
    HuffmanCodes(arr, freq, size);

    for(map<unsigned char,string>::iterator it = codes.begin(); it!=codes.end(); ++it)
        cout << (int)it->first << " => " << it->second << '\n';
    /*for(int i = 0; i<length; ++i)
        cout << buffer[i] << " ";*/
    cout << endl;
    string encoded = "";
    double averageLen = 0.0;
    for(int i = 0; i<length; ++i) {
        //cout << codes[buffer[i]];
        encoded += codes[buffer[i]];
    }
    map<unsigned char,string>::iterator it = codes.begin();
    string temp;
    for(int i = 0; i<diffBytes; ++i) {
        temp = it->second;
        //cout << temp;
        averageLen += (double)(freq[i]*temp.size())/length;
        it++;
    }
    /*unsigned char* toDecode = new unsigned char[encoded.length() + 1];
    char* temp1 = new char[encoded.length() + 1];
    strcpy(temp1, encoded.c_str());

    for(int i = 0; i<encoded.length(); ++i) {
        toDecode[i] = temp1[i];    
    }
    00000000000000000000000001000010 01100001 00000001 0 01100001 00000001 1
    00000000000000000000000001000010 01100001 00000001 0 01100010 00000001 1
    */
    stringstream ss;
    temp = "";
    string dictionary = "";
    int len;
    string binary_outputInformations = "";
    for(map<unsigned char,string>::iterator it = codes.begin(); it!=codes.end(); ++it) {
        //cout << it->first << " => " << it->second << endl;
        ss.clear();
        ss << it->first;
        ss >> temp;
        for(int i = 0; i < temp.length(); ++i) {
            bitset<8> b(temp.c_str()[i]);
            binary_outputInformations+= b.to_string();
        }
        len = it->second.length();
        bitset<8> b(len);
        binary_outputInformations+= b.to_string();
        binary_outputInformations+= it->second;

        //cout << binary_outputInformations << endl;
    }
    len = binary_outputInformations.length()+16;
    bitset<32> b(len);
    dictionary += b.to_string();
    dictionary += binary_outputInformations;
    //cout << "DICTIONARY: " << dictionary << endl;
    cout << "DIC length: " << b.to_string() << endl;
    temp = encoded;
    encoded = dictionary;
    encoded += temp;
    while(encoded.length() % 8 != 0)
        encoded += "0";
    encoded += "00000000";

    double compressionDeg = (double)encoded.size()/(length*8);
    
    //string decodes = huffmanDecode(encoded);
    //cout << count << endl;
    //cout << "Encoded: " << encoded << endl << endl;
    cout << "Dlugosc wejscia: " << length*8 << endl;
    cout << "Dlugosc po zakodowaniu: " << encoded.size() << endl;
    cout << "Srednia dlugosc kodowania: " << averageLen << endl;
    cout << "Stopień kompresji: " << compressionDeg << endl; 
    //cout << "root: " << root->data << " " << root->freq << " " << root->left << endl;    

    writeFile(encoded);

    free(buffer);
    //free(toDecode);
    return 0;
}

// Write 
void writeFile(string encoded) { 
    FILE* pFile;
    unsigned char* a = new unsigned char[encoded.length()/8+1];
    //strcpy(a, encoded.c_str());
    string* temp = new string[(encoded.length()/8+1)];
    unsigned long decimal;
    int j = 0;
    /*for(int i = 0; i<encoded.length(); ++i) {
        if(temp[j].length() == 8) {
            decimal = bitset<8>(temp[j]).to_ulong();
            //cout << bitset<8>(temp[j]) << endl;
            a[j] = decimal;
        }
        if((i+1) % 8 == 0) {
            j++;
        }
        temp[j] += encoded.at(i);
    }*/
    for(int i = 0; i<encoded.length(); ++i) {
        if((i+1) % 8 == 0) {
            j++;
        }
        temp[j] += encoded.at(i);
    }
    temp[0] += "0";
    for(j = 0; j<encoded.length()/8; ++j) { //encoded.length()/8
        decimal = bitset<8>(temp[j]).to_ulong();
        a[j] = decimal;
        //cout << (int)a[j] << endl;
    }
    cout << endl;


    //string binary = bitset<8>(128).to_string(); //to binary
    //cout<<binary<<"\n";

    //unsigned long decimal = bitset<8>(encoded[0]).to_ulong();
    //cout<<decimal<<"\n";

    /*for(int i = 0; i<encoded.length(); ++i)
        cout << a[i] << " ";*/
    int size = encoded.length();
    pFile = fopen("/home/afro/Dokumenty/kodowanie/3/encoded", "w+");
    cout << "ENCODED LEN: " << encoded.length() << endl;

    fwrite(a, encoded.length()/8, 1, pFile);
    free(a);
}


// Reading binary files function

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


// A utility function to print an array of size n

void printArr(int arr[], int n, unsigned char key)
{
    string temp = "";
    char* str = (char *)malloc(sizeof(char));
    for(int i = 0; i < n; ++i) {
        //printf("%d", arr[i]);
        sprintf(str, "%d", arr[i]);
        temp += string(str);
    }
    codes1.insert(pair<string, char>(temp,key));
    codes.insert(pair<unsigned char, string>(key,temp));
    free(str);
    //printf("\n");
}



// Prints huffman codes from the root of Huffman Tree.  It uses arr[] to
// store codes
void printCodes(struct MinHeapNode* root, int arr[], int top)
{
    // Assign 0 to left edge and recur
    if (root->left)
    {
        arr[top] = 0;
        printCodes(root->left, arr, top + 1);
    }
 
    // Assign 1 to right edge and recur
    if (root->right)
    {
        arr[top] = 1;
        printCodes(root->right, arr, top + 1);
    }
 
    // If this is a leaf node, then it contains one of the input
    // characters, print the character and its code from arr[]
    if (isLeaf(root))
    {
        //printf(" %c: ", root->data);
        printArr(arr, top, root->data);
    }
}

string huffmanDecode(string huffmanCode){
    string t = "";
    string ans = "";
    map <string, char> :: iterator it;
    char c;
    for(int i = 0; i < huffmanCode.size(); i++ ) {
        t += " ";
        t[t.size()-1] = huffmanCode[i];
        it = codes1.find(t);
        if( it != codes1.end() ) {
            c = codes1[t];
            ans += " ";
            ans[ans.size()-1] = c;
            t = "";
        }
    }
    return ans;
}