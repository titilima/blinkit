BaseSrc = $(BkRoot)src/chromium/base
BaseFlags = -I$(BkRoot)src/base -I$(BaseSrc) $(CrFlags) -include _pc.h
BaseObjects = base_location.o logging_posix.o task_runner.o \
	string_number_conversions.o stringprintf.o string_split.o string_util.o string_util_constants.o utf_string_conversion_utils.o \
	thread_local_storage_posix.o thread_local_storage.o \
	time_exploded_posix.o time_posix.o base_time.o

base_location.o: $(BaseSrc)/location.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
logging_posix.o: $(BaseSrc)/logging_posix.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
task_runner.o: $(BaseSrc)/task_runner.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_number_conversions.o: $(BaseSrc)/strings/string_number_conversions.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
stringprintf.o: $(BaseSrc)/strings/stringprintf.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_split.o: $(BaseSrc)/strings/string_split.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_util.o: $(BaseSrc)/strings/string_util.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
string_util_constants.o: $(BaseSrc)/strings/string_util_constants.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
utf_string_conversion_utils.o: $(BaseSrc)/strings/utf_string_conversion_utils.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
thread_local_storage_posix.o: $(BaseSrc)/threading/thread_local_storage_posix.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
thread_local_storage.o: $(BaseSrc)/threading/thread_local_storage.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
time_exploded_posix.o: $(BaseSrc)/time/time_exploded_posix.cc
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
time_posix.o: $(BaseSrc)/time/time_posix.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
base_time.o: $(BaseSrc)/time/time.cpp
	$(CXX) -c $(CXXFLAGS) $(BaseFlags) $< -o $@
