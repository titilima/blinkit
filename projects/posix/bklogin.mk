BkLoginSrc = $(BkRoot)src/bklogin
BkLoginFlags = -I$(BkLoginSrc) $(CrFlags) -include _pc.h
BkLoginObjects = http_request_task.o https_request_task.o request_task_base.o response_task.o	\
	login_globals.o login_proxy_impl.o posix_login_proxy.o private_key_impl.o socket_wrapper.o	\
	certificate.o rsa_key.o sign_request.o ssl_pair.o

http_request_task.o: $(BkLoginSrc)/login/tasks/http_request_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
https_request_task.o: $(BkLoginSrc)/login/tasks/https_request_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
request_task_base.o: $(BkLoginSrc)/login/tasks/request_task_base.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
response_task.o: $(BkLoginSrc)/login/tasks/response_task.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@

login_globals.o: $(BkLoginSrc)/login/login_globals.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
login_proxy_impl.o: $(BkLoginSrc)/login/login_proxy_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
posix_login_proxy.o: $(BkLoginSrc)/login/posix_login_proxy.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
private_key_impl.o: $(BkLoginSrc)/login/private_key_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
socket_wrapper.o: $(BkLoginSrc)/login/socket_wrapper.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@

certificate.o: $(BkLoginSrc)/ssl/certificate.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
rsa_key.o: $(BkLoginSrc)/ssl/rsa_key.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
sign_request.o: $(BkLoginSrc)/ssl/sign_request.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
ssl_pair.o: $(BkLoginSrc)/ssl/ssl_pair.cpp
	$(CXX) -c $(CXXFLAGS) $(BkLoginFlags) $< -o $@
