NetSrc = $(BkRoot)src/chromium/net
NetFlags = -I$(BkRoot)src/net $(CrFlags) -include _pc.h
NetObjects = registry_controlled_domain.o net_util.o \
	canonical_cookie.o cookie_constants.o cookie_options.o cookie_util.o parsed_cookie.o \
	http_util.o

registry_controlled_domain.o: $(NetSrc)/base/registry_controlled_domains/registry_controlled_domain.cpp
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
net_util.o: $(NetSrc)/base/net_util.cpp
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@

canonical_cookie.o: $(NetSrc)/cookies/canonical_cookie.cc
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
cookie_constants.o: $(NetSrc)/cookies/cookie_constants.cc
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
cookie_options.o: $(NetSrc)/cookies/cookie_options.cc
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
cookie_util.o: $(NetSrc)/cookies/cookie_util.cc
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
parsed_cookie.o: $(NetSrc)/cookies/parsed_cookie.cc
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@

http_util.o: $(NetSrc)/http/http_util.cpp
	$(CXX) -c $(CXXFLAGS) $(NetFlags) $< -o $@
