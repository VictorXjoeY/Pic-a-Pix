#ifndef _HEAP_H_
#define _HEAP_H_
#include <vector>

using namespace std;
struct HeapItem{
	int x;
	int y;
	int key;

	HeapItem(){

	}

	HeapItem(int x, int y, int key){
		this->x = x;
		this->y = y;
		this->key = key;
	}
};

struct Compare{
		inline bool operator()(const HeapItem &a, const HeapItem &b){
			return a.key > b.key;
		}
};

void update_heap(vector<HeapItem> &heap, int x, int y, int key){
	vector<HeapItem>::iterator i;

}

#endif