StubSrc = $(BkRoot)src/stub
StubObjects = ubidi_props.o uchar.o uprops.o

ubidi_props.o: $(StubSrc)/icu/ubidi_props.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
uchar.o: $(StubSrc)/icu/uchar.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
uprops.o: $(StubSrc)/icu/uprops.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@
