CXXFLAGS+=-O3
%: %.cpp
	$(CXX)  $(CXXFLAGS)  $^  -o $@.out
debug: CXXFLAGS += -g
debug: day16
clean:
	rm *.out
	rm *.o