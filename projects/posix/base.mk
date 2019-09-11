BaseSrc = $(BkRoot)src/chromium/base
BaseFlags = -I$(BkRoot)src/base -I$(BaseSrc) $(CrFlags)
BaseObjects = string_split.o string_util.o string_util_constants.o utf_string_conversion_utils.o

string_split.o: $(BaseSrc)/strings/string_split.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_util.o: $(BaseSrc)/strings/string_util.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_util_constants.o: $(BaseSrc)/strings/string_util_constants.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
utf_string_conversion_utils.o: $(BaseSrc)/strings/utf_string_conversion_utils.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
