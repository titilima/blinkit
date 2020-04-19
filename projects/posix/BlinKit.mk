CrawlerSrc = $(BkRoot)src/blinkit
CrawlerFlags = -I$(CrawlerSrc) -I$(BkRoot)src/stub/icu -DBLINKIT_CRAWLER_ONLY $(CrFlags) -include _pc.h
CrawlerObjects = app_constants.o app_impl.o posix_app.o \
	local_frame_client_impl.o posix_task_runner.o posix_thread.o thread_impl.o url_loader_impl.o \
	bk_http_header_map.o bk_url.o \
	crawler_document.o crawler_element.o crawler_impl.o crawler_script_element.o \
	curl_request.o request_impl.o response_impl.o \
	context_impl.o js_value_impl.o \
	http_loader_task.o loader_task.o \
	buffer.o controller.o

app_constants.o: $(CrawlerSrc)/app/app_constants.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
app_impl.o: $(CrawlerSrc)/app/app_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
posix_app.o: $(CrawlerSrc)/app/posix_app.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

local_frame_client_impl.o: $(CrawlerSrc)/blink_impl/local_frame_client_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
posix_task_runner.o: $(CrawlerSrc)/blink_impl/posix_task_runner.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
posix_thread.o: $(CrawlerSrc)/blink_impl/posix_thread.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
thread_impl.o: $(CrawlerSrc)/blink_impl/thread_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
url_loader_impl.o: $(CrawlerSrc)/blink_impl/url_loader_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

bk_http_header_map.o: $(CrawlerSrc)/common/bk_http_header_map.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
bk_url.o: $(CrawlerSrc)/common/bk_url.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

crawler_document.o: $(CrawlerSrc)/crawler/crawler_document.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_element.o: $(CrawlerSrc)/crawler/crawler_element.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_impl.o: $(CrawlerSrc)/crawler/crawler_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_script_element.o: $(CrawlerSrc)/crawler/crawler_script_element.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

curl_request.o: $(CrawlerSrc)/http/curl_request.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
request_impl.o: $(CrawlerSrc)/http/request_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
response_impl.o: $(CrawlerSrc)/http/response_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

context_impl.o: $(CrawlerSrc)/js/context_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
js_value_impl.o: $(CrawlerSrc)/js/js_value_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

http_loader_task.o: $(CrawlerSrc)/loader_tasks/http_loader_task.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
loader_task.o: $(CrawlerSrc)/loader_tasks/loader_task.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

buffer.o: $(CrawlerSrc)/misc/buffer.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
controller.o: $(CrawlerSrc)/misc/controller.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
