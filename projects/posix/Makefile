config=debug

CXX = clang++
CXXFLAGS = -fPIC -fvisibility=hidden -std=c++17 -stdlib=libc++ -include _pc.h
ifeq ($(config), release)
CXXFLAGS += -DNDEBUG
else
CXXFLAGS += -g
endif

BkRoot = ../../
CrFlags = -I$(BkRoot)src/chromium -I$(BkRoot)sdk/include

.PHONY: clean help

help:
	@echo Usage:
	@echo '    make all                # Build BlinKit for debugging'
	@echo '    make all config=release # Build BlinKit for release'
	@echo '    make clean              # Cleanup all object files'

include base.mk url.mk BlinKit.mk

AllObjects = $(BaseObjects) $(URLObjects) $(CrawlerObjects)

all: $(AllObjects)
	$(CXX) -fPIC -shared $(AllObjects) -lcurl -o libBlinKit.so
test: BkTest.cpp
	$(CXX) -g -std=c++17 -stdlib=libc++ -I$(BkRoot)sdk/include BkTest.cpp -L . -lBlinKit -o BkTest
clean:
	rm -f $(AllObjects)
