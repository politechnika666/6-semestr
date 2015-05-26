all: Compile runAll

Compile: lzw.cpp
	g++ -Wall -o lzw lzw.cpp

runAll: lzw
	./lzw enc fibbonacci test3 fibboEncoded
	./lzw enc gamma test3 gammaEncoded
	./lzw enc delta test3 deltaEncoded
	./lzw enc omega test3 omegaEncoded
	
	./lzw dec fibbonacci fibboEncoded fibboDecoded
	./lzw dec gamma gammaEncoded gammaDecoded
	./lzw dec delta deltaEncoded deltaDecoded
	./lzw dec omega omegaEncoded omegaDecoded