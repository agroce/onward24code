all: test_binsearch fuzz_binsearch cov_binsearch

test_binsearch: binsearch.h binsearch.c binsearch_deepstate.cpp
	clang++ -o test_binsearch binsearch_deepstate.cpp binsearch.c -ldeepstate

fuzz_binsearch: binsearch.h binsearch.c binsearch_deepstate.cpp
	afl-clang++ -o test_binsearch binsearch_deepstate.cpp binsearch.c -ldeepstate_AFL

cov_binsearch: binsearch.h binsearch.c binsearch_deepstate.cpp
	clang++ -o test_binsearch binsearch_deepstate.cpp binsearch.c -ldeepstate --coverage

clean:
	rm -rf test_binsearch fuzz_binsearch cov_binsearch

