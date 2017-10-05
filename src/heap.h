#ifndef _HEAP_H_
#define _HEAP_H_
#include <vector>

using namespace std;
struct HeapItem{
	int x;
	int y;
	float key;
	int color;

	HeapItem(){

	}

	HeapItem(int x, int y, float key){
		this->x = x;
		this->y = y;
		this->key = key;
	}

	HeapItem(int x, int y, float key, int color){
		this->x = x;
		this->y = y;
		this->key = key;
		this->color=color;
	}
};

struct Compare{
		inline bool operator()(const HeapItem &a, const HeapItem &b){
			return a.key > b.key;
		}
};

struct CompareMax{
		inline bool operator()(const HeapItem &a, const HeapItem &b){
			return a.key < b.key;
		}
};

#endif