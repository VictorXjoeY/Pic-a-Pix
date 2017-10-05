#ifndef _HEAP_H_
#define _HEAP_H_
#include <vector>

using namespace std;
struct HeapItem{
	int x; //rowheuristic: coluna ou linha || pixelheuristic: coordenada x do pixel
	int y; //rowheuristic: indice da coluna ou linha || pixelheuristic: coordenada y do pixel
	float key;
	int color; //usado apenas para pixelheuristic

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

//usado para rowheuristic para uma min heap
struct Compare{
		inline bool operator()(const HeapItem &a, const HeapItem &b){
			return a.key > b.key;
		}
};

//usado para pixelheuristic para uma max heap
struct CompareMax{
		inline bool operator()(const HeapItem &a, const HeapItem &b){
			return a.key < b.key;
		}
};

#endif