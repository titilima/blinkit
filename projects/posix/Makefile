config=debug

CC = clang
CCFLAGS = -fPIC
CXX = clang++
CXXFLAGS = -std=c++17 -stdlib=libc++
ifeq ($(config), release)
CCFLAGS += -DNDEBUG
CXXFLAGS += -DNDEBUG
else
CCFLAGS += -g
CXXFLAGS += -g
endif

BkRoot = ../../
CrFlags = -I$(BkRoot)sdk/include -I$(BkRoot)src -I$(BkRoot)src/chromium

.PHONY: clean help

help:
	@echo Usage:
	@echo '    make all                    # Build BlinKit (BkCrawler + BkHelper) for debugging'
	@echo '    make all config=release     # Build BlinKit (BkCrawler + BkHelper) for release'
	@echo '    make crawler                # Build BkCrawler for debugging'
	@echo '    make crawler config=release # Build BkCrawler for release'
	@echo '    make clean                  # Cleanup all object files'
	@echo '    make test                   # Build test program using BkTest.cpp'

include base.mk blink.mk duktape.mk net.mk stub.mk url.mk bkbase.mk bkcommon.mk bkhelper.mk BlinKit.mk

BkCrawlerObjects = $(BaseObjects) $(BlinkObjects) $(DuktapeObjects) $(NetObjects) $(StubObjects) $(URLObjects) \
	$(BkBaseObjects) $(BkCommonObjects) $(CrawlerObjects)
AllObjects = $(BkCrawlerObjects) $(BkHelperObjects)

all: $(AllObjects)
	ar -rcs ../../sdk/posix/libBlinKit.a $(AllObjects)
crawler: $(BkCrawlerObjects)
	ar -rcs ../../sdk/posix/libBkCrawler.a $(BkCrawlerObjects)
test: BkTest.cpp
	$(CXX) -g -std=c++17 -stdlib=libc++ -I$(BkRoot)sdk/include BkTest.cpp -L . -lBlinKit -lcurl -lpthread -lz -o BkTest
clean:
	rm -f $(AllObjects)
