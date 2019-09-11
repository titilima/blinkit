CrawlerSrc = $(BkRoot)src/blinkit
CrawlerFlags = -I$(CrawlerSrc) -DBLINKIT_EXPORTS $(CrFlags)
CrawlerObjects = app_constants.o \
	curl_request.o request_impl.o response_impl.o	\
	buffer.o controller.o

app_constants.o: $(CrawlerSrc)/app/app_constants.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
curl_request.o: $(CrawlerSrc)/http/curl_request.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
request_impl.o: $(CrawlerSrc)/http/request_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
response_impl.o: $(CrawlerSrc)/http/response_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
buffer.o: $(CrawlerSrc)/misc/buffer.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
controller.o: $(CrawlerSrc)/misc/controller.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
