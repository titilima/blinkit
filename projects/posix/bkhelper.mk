BkHelperSrc = $(BkRoot)src/bkhelper
BkHelperFlags = -I$(BkHelperSrc) $(CrFlags) -include _pc.h
BkHelperObjects = http_request_task.o https_request_task.o request_task_base.o response_task.o	\
	posix_proxy.o proxy_globals.o proxy_impl.o socket_wrapper.o	\
	certificate.o private_key_impl.o rsa_key.o sign_request.o ssl_pair.o

http_request_task.o: $(BkHelperSrc)/proxy/tasks/http_request_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
https_request_task.o: $(BkHelperSrc)/proxy/tasks/https_request_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
request_task_base.o: $(BkHelperSrc)/proxy/tasks/request_task_base.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
response_task.o: $(BkHelperSrc)/proxy/tasks/response_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@

posix_proxy.o: $(BkHelperSrc)/proxy/posix_proxy.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
proxy_globals.o: $(BkHelperSrc)/proxy/proxy_globals.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
proxy_impl.o: $(BkHelperSrc)/proxy/proxy_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
socket_wrapper.o: $(BkHelperSrc)/proxy/socket_wrapper.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@

certificate.o: $(BkHelperSrc)/ssl/certificate.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
private_key_impl.o: $(BkHelperSrc)/ssl/private_key_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
rsa_key.o: $(BkHelperSrc)/ssl/rsa_key.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
sign_request.o: $(BkHelperSrc)/ssl/sign_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
ssl_pair.o: $(BkHelperSrc)/ssl/ssl_pair.cpp
	$(CXX) -c $(CXXFLAGS) $(BkHelperFlags) $< -o $@
