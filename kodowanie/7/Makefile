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
	./lzwCoder tadeusz
	./fibboCoder lzwEncoded
	./fibboDecoder fibboEncoded
	./lzwDecoder fibboDecoded

