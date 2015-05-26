<<<<<<< HEAD
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
=======
all: lzwCoder lzwDecoder fibboCoder fibboDecoder runAll

fibboCoder: fibboCoder.cpp
	g++ -Wall -o fibboCoder fibboCoder.cpp

fibboDecoder: fibboDecoder.cpp
	g++ -Wall -o fibboDecoder fibboDecoder.cpp

lzwCoder: lzwCoder.cpp
	g++ -Wall -o lzwCoder lzwCoder.cpp

lzwDecoder: lzwDecoder.cpp
	g++ -Wall -o lzwDecoder lzwDecoder.cpp

runAll: lzwCoder lzwDecoder fibboCoder fibboDecoder
	./lzwCoder test3
	./fibboCoder lzwEncoded
	./fibboDecoder fibboEncoded
	./lzwDecoder fibboDecoded

>>>>>>> c948fe257010a3f004783e84cc96d37e08be9626
