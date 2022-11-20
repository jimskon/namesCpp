CXX = g++
CXXFLAGS = -O2 -std=c++11 -I.. -Wall -Wextra -pthread

PREFIX = /usr/local
#PREFIX = $(shell brew --prefix)

OPENSSL_DIR = $(PREFIX)/opt/openssl@1.1
#OPENSSL_DIR = $(PREFIX)/opt/openssl@3
OPENSSL_SUPPORT = -DCPPHTTPLIB_OPENSSL_SUPPORT -I$(OPENSSL_DIR)/include -L$(OPENSSL_DIR)/lib -lssl -lcrypto

ZLIB_SUPPORT = -DCPPHTTPLIB_ZLIB_SUPPORT -lz

BROTLI_DIR = $(PREFIX)/opt/brotli
BROTLI_SUPPORT = -DCPPHTTPLIB_BROTLI_SUPPORT -I$(BROTLI_DIR)/include -L$(BROTLI_DIR)/lib -lbrotlicommon -lbrotlienc -lbrotlidec

all: namesAPI PutHTML namesconsole

PutHTML:
	cp namelookup.html /var/www/html/namesCpp/
	cp namelookup.css /var/www/html/namesCpp/
	cp namelookup.js /var/www/html/namesCpp/

	echo "Current contents of your HTML directory: "
	ls -l /var/www/html/namesCpp/

test : test.cpp httplib.h Makefile
	$(CXX) -o test $(CXXFLAGS) test.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT)

NameEntry.o: NameEntry.cpp NameEntry.h
	$(CC) $(CFLAGS) NameEntry.cpp -c

NameMap.o: NameMap.cpp NameMap.h NameEntry.h
	$(CC) $(CFLAGS) NameMap.cpp -c

namesAPI : namesAPI.cpp httplib.h NameEntry.o NameMap.o
	$(CXX) -o namesAPI $(CXXFLAGS) namesAPI.cpp $(OPENSSL_SUPPORT) $(ZLIB_SUPPORT) $(BROTLI_SUPPORT) NameEntry.o NameMap.o

namesconsole : namesAPI.cpp NameEntry.o NameMap.o namesconsole.cpp
	$(CXX) -o namesconsole namesconsole.cpp NameEntry.o NameMap.o

pem:
	openssl genrsa 2048 > key.pem
	openssl req -new -key key.pem | openssl x509 -days 3650 -req -signkey key.pem > cert.pem

clean:
	rm *.o namesconsole namesAPI  *.pem
