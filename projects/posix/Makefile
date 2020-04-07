config=debug

CXX = clang++
CXXFLAGS = -fPIC -fvisibility=hidden -std=c++17 -stdlib=libc++ -include _pc.h
ifeq ($(config), release)
CXXFLAGS += -DNDEBUG
else
CXXFLAGS += -g
endif

BkRoot = ../../
CrFlags = -I$(BkRoot)sdk/include -I$(BkRoot)src -I$(BkRoot)src/chromium

.PHONY: clean help

help:
	@echo Usage:
	@echo '    make all                # Build BlinKit for debugging'
	@echo '    make all config=release # Build BlinKit for release'
	@echo '    make clean              # Cleanup all object files'

include base.mk blink.mk url.mk BlinKit.mk

AllObjects = $(BaseObjects) $(BlinkObjects) $(URLObjects) $(CrawlerObjects)

all: $(AllObjects)
	$(CXX) -fPIC -shared $(AllObjects) -lcurl -o libBlinKit.so
test: BkTest.cpp
	$(CXX) -g -std=c++17 -stdlib=libc++ -I$(BkRoot)sdk/include BkTest.cpp -L . -lBlinKit -o BkTest
clean:
	rm -f $(AllObjects)
