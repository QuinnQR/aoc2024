CXXFLAGS+=-O3
%: %.cpp
	$(CXX)  $(CXXFLAGS)  $^ -lgmp -lgmpxx -o $@.out
clean:
	rm *.out
	rm *.o