CFLAG = -c -O2 -std=c++11 -Wall -Wextra

bin/classifier: obj/main.o obj/naive_bayes.o | bin
	g++ -o $@ $^

obj/main.o: src/main.cpp src/naive_bayes.h | obj
	g++ $(CFLAG) -o $@ $<

obj/naive_bayes.o: src/naive_bayes.cpp src/naive_bayes.h src/filter.h | obj
	g++ $(CFLAG) -o $@ $<

# obj/filter.o: src/filter.cpp src/filter.h | obj
# 	g++ $(CFLAG) -o $@ $<

bin:
	mkdir $@

obj:
	mkdir $@

clean:
	-rm obj/*.o
