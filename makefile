rbtree_test: main.cpp rbtree.h
	g++ main.cpp -g -o rbtree_test -std=gnu++11

clean:
	rm rbtree_test
