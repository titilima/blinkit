BkBaseSrc = $(BkRoot)src/bkbase
BkBaseFlags = -I$(BkBaseSrc) $(CrFlags) -include _pc.h
BkBaseObjects = http_constants.o http_response.o posix_request.o request_impl.o	\
	buffer.o controller.o

http_constants.o: $(BkBaseSrc)/http/http_constants.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
http_response.o: $(BkBaseSrc)/http/http_response.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
posix_request.o: $(BkBaseSrc)/http/posix_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
request_impl.o: $(BkBaseSrc)/http/request_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
buffer.o: $(BkBaseSrc)/misc/buffer.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
controller.o: $(BkBaseSrc)/misc/controller.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
