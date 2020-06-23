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
	@echo '    make blinkit                # Build BlinKit for debugging'
	@echo '    make blinkit config=release # Build BlinKit for release'
	@echo '    make bklogin                # Build BkLogin for debugging'
	@echo '    make bklogin config=release # Build BkLogin for release'
	@echo '    make clean                  # Cleanup all object files'
	@echo '    make test                   # Build test program using BkTest.cpp'

include base.mk blink.mk duktape.mk net.mk stub.mk url.mk bkbase.mk bkcommon.mk bklogin.mk BlinKit.mk

BlinKitObjects = $(BaseObjects) $(BlinkObjects) $(DuktapeObjects) $(NetObjects) $(StubObjects) $(URLObjects) \
	$(BkBaseObjects) $(BkCommonObjects) $(CrawlerObjects)
AllObjects = $(BlinKitObjects) $(BkLoginObjects)

blinkit: $(BlinKitObjects)
	ar -rcs libBlinKit.a $(BlinKitObjects)
bklogin: $(BkLoginObjects)
	ar -rcs libBkLogin.a $(BkLoginObjects)
test: BkTest.cpp
	$(CXX) -g -std=c++17 -stdlib=libc++ -I$(BkRoot)sdk/include BkTest.cpp -L . -lBlinKit -lcurl -lpthread -lz -o BkTest
clean:
	rm -f $(AllObjects)
