URLSrc = $(BkRoot)src/chromium/url
URLFlags = -I$(BkRoot)src/url -I$(URLSrc) $(CrFlags)
URLObjects = gurl.o url_canon.o url_canon_etc.o url_canon_filesystemurl.o url_canon_fileurl.o url_canon_host.o	\
	url_canon_internal.o url_canon_ip.o url_canon_mailtourl.o url_canon_path.o url_canon_pathurl.o	\
	url_canon_query.o url_canon_relative.o url_canon_stdstring.o url_canon_stdurl.o url_constants.o url_parse.o	\
	url_parse_file.o url_util.o

gurl.o: $(URLSrc)/gurl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon.o: $(BkRoot)src/url/url_canon.cpp
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_etc.o: $(URLSrc)/url_canon_etc.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_filesystemurl.o: $(URLSrc)/url_canon_filesystemurl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_fileurl.o: $(URLSrc)/url_canon_fileurl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_host.o: $(URLSrc)/url_canon_host.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_internal.o: $(URLSrc)/url_canon_internal.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_ip.o: $(URLSrc)/url_canon_ip.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_mailtourl.o: $(URLSrc)/url_canon_mailtourl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_path.o: $(URLSrc)/url_canon_path.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_pathurl.o: $(URLSrc)/url_canon_pathurl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_query.o: $(URLSrc)/url_canon_query.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_relative.o: $(URLSrc)/url_canon_relative.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_stdstring.o: $(URLSrc)/url_canon_stdstring.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_canon_stdurl.o: $(URLSrc)/url_canon_stdurl.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_constants.o: $(URLSrc)/url_constants.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_parse.o: $(URLSrc)/third_party/mozilla/url_parse.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_parse_file.o: $(URLSrc)/url_parse_file.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
url_util.o: $(URLSrc)/url_util.cc
	$(CXX) -c $(CXXFLAGS) $(URLFlags) $< -o $@
