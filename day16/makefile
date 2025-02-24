CXXFLAGS+=-O3
%: %.cpp
	$(CXX)  $(CXXFLAGS)  $^  -o $@.out
clean:
	rm *.out
	rm *.o