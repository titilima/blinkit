CrawlerSrc = $(BkRoot)src/blinkit
CrawlerFlags = -I$(CrawlerSrc) -I$(BkRoot)src/stub/icu -DBLINKIT_CRAWLER_ONLY $(CrFlags) -include _pc.h
CrawlerObjects = app_impl.o posix_app.o \
	local_frame_client_impl.o posix_task_runner.o posix_thread.o thread_impl.o url_loader_impl.o	\
	crawler_document.o crawler_element.o crawler_script_element.o cookie_jar_impl.o crawler_impl.o hijack_response.o	\
	context_impl.o js_value_impl.o \
	http_loader_task.o loader_task.o \
	task_loop.o

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

crawler_document.o: $(CrawlerSrc)/crawler/dom/crawler_document.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_element.o: $(CrawlerSrc)/crawler/dom/crawler_element.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_script_element.o: $(CrawlerSrc)/crawler/dom/crawler_script_element.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
cookie_jar_impl.o: $(CrawlerSrc)/crawler/cookie_jar_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
crawler_impl.o: $(CrawlerSrc)/crawler/crawler_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
hijack_response.o: $(CrawlerSrc)/crawler/hijack_response.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

context_impl.o: $(CrawlerSrc)/js/context_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
js_value_impl.o: $(CrawlerSrc)/js/js_value_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

http_loader_task.o: $(CrawlerSrc)/loader_tasks/http_loader_task.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
loader_task.o: $(CrawlerSrc)/loader_tasks/loader_task.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@

task_loop.o: $(CrawlerSrc)/posix/task_loop.cpp
	$(CXX) -c $(CXXFLAGS) $(CrawlerFlags) $< -o $@
