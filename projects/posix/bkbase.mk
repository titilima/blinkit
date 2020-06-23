BkBaseSrc = $(BkRoot)src/bkbase
BkBaseFlags = -I$(BkBaseSrc) $(CrFlags) -include _pc.h
BkBaseObjects = http_constants.o http_response.o request_impl.o curl_request.o	\
	buffer.o controller.o

http_constants.o: $(BkBaseSrc)/http/http_constants.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
http_response.o: $(BkBaseSrc)/http/http_response.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
request_impl.o: $(BkBaseSrc)/http/request_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
curl_request.o: $(BkBaseSrc)/http/curl_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
buffer.o: $(BkBaseSrc)/misc/buffer.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
controller.o: $(BkBaseSrc)/misc/controller.cpp
	$(CXX) -c $(CXXFLAGS) $(BkBaseFlags) $< -o $@
