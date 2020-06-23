BkCommonSrc = $(BkRoot)src/bkcommon
BkCommonFlags = -I$(BkCommonSrc) $(CrFlags) -include _pc.h
BkCommonObjects = bk_file.o bk_http_header_map.o bk_strings.o response_impl.o

bk_file.o: $(BkCommonSrc)/bk_file.cpp
	$(CXX) -c $(CXXFLAGS) $(BkCommonFlags) $< -o $@
bk_http_header_map.o: $(BkCommonSrc)/bk_http_header_map.cpp
	$(CXX) -c $(CXXFLAGS) $(BkCommonFlags) $< -o $@
bk_strings.o: $(BkCommonSrc)/bk_strings.cpp
	$(CXX) -c $(CXXFLAGS) $(BkCommonFlags) $< -o $@
response_impl.o: $(BkCommonSrc)/response_impl.cpp
	$(CXX) -c $(CXXFLAGS) $(BkCommonFlags) $< -o $@
