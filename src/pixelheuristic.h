#ifndef _PIXELHEURISTIC_H_
#define _PIXELHEURISTIC_H_

#include "heap.h"
#include "board.h"

/*Heuristica que calcula probabilidade de um pixel ser preto ou branco*/


bool updatePixels(Board &b, int x, int y, vector<struct HeapItem> &heap){
	//atualiza pixels da mesma linha ou coluna
	vector<int> blackPosRow;
	vector<int> whitePosRow;

	blackPosRow.resize(b.m+1);
	whitePosRow.resize(b.m+1);

	//linha
	for(int i=1; i<=b.m; i++){
		if(b.mat[x][i]==NONE){
			b.mat[x][i]=BLACK;
			blackPosRow[i] = solve_row(b, x)*solve_col(b, i);
			b.mat[x][i]=WHITE;
			whitePosRow[i] = solve_row(b, x)*solve_col(b, i);
			b.mat[x][i]=NONE;
			if(blackPosRow[i]+whitePosRow[i]==0)
				return false;

			//printf("%d %d: Black %d White %d\n", x, i, blackPosRow[i], whitePosRow[i]);
		}
	}

	//coluna
	vector<int> blackPosCol;
	vector<int> whitePosCol;

	blackPosCol.resize(b.n+1);
	whitePosCol.resize(b.n+1);

	//coluna
	for(int i=1; i<=b.n; i++){
		if(b.mat[i][y]==NONE){
			b.mat[i][y]=BLACK;
			blackPosCol[i] = solve_row(b, i)*solve_col(b, y);
			b.mat[i][y]=WHITE;
			whitePosCol[i] = solve_row(b, i)*solve_col(b, y);
			b.mat[i][y]=NONE;
			if(blackPosCol[i]+whitePosCol[i]==0)
				return false;
				//printf("%d %d: Black %d White %d\n", x, i, blackPosCol[i], whitePosCol[i]);
		}

	}

	for(int k=0; k<(int)heap.size(); k++){
		if(heap[k].x==x || heap[k].y==y){
			if(b.mat[heap[k].x][heap[k].y]==NONE){
				if(heap[k].x==x){ //mesma linha
					if(heap[k].color==BLACK){
						heap[k].key=(float)blackPosRow[heap[k].y]/(blackPosRow[heap[k].y]+whitePosRow[heap[k].y]);
						
					}else{
						heap[k].key=(float)whitePosRow[heap[k].y]/(blackPosRow[heap[k].y]+whitePosRow[heap[k].y]);
					}
				}else{ //mesma coluna
					if(heap[k].color==BLACK){
						heap[k].key=(float)blackPosCol[heap[k].x]/(blackPosCol[heap[k].x]+whitePosCol[heap[k].x]);
					}else{
						heap[k].key=(float)whitePosCol[heap[k].x]/(blackPosCol[heap[k].x]+whitePosCol[heap[k].x]);
					}

				}
			}
		}
	}
	//se impossivel, tentar outra cor
	make_heap(heap.begin(), heap.end(), CompareMax());
	return true;
}


bool solvePixelRec(Board &b, int x, int y, vector<struct HeapItem> &heap, int color){
	
	//printf("proximo pixel %d %d: cor %c\n", x, y, color);

	//se pixel ja estiver preenchido, remove item e chama proximo
	if(b.mat[x][y]!=NONE){
			//remove primeiro item da heap
		//heap vazia, sucesso
		if(heap.size()==0)
			return true;	


		int i=heap[0].x;
		int j=heap[0].y;

		if(heap[0].key==0 && b.mat[i][j]==NONE) //proximo pixel nao preenchido e probabilidade==0
			return false;

		int color=heap[0].color;
		pop_heap(heap.begin(), heap.end(), CompareMax());
		heap.pop_back();
		//printHeap(heap);	
		return solvePixelRec(b, i, j, heap, color);
	}


	
	//probabilidade 0 e pixel nao preenchido
		//return false


	//tente cor indicada pela probabilidade
	b.mat[x][y]=color;
	//print_board(b);

	//atualiza probabilidade dos pixels na mesma linha ou coluna
	bool success;
	if(updatePixels(b, x, y, heap)){
		//chama proximo pixel
		int i=heap[0].x;
		int j=heap[0].y;

		if(heap[0].key==0 && b.mat[i][j]==NONE) //proximo pixel nao preenchido e probabilidade==0
			success = false;
		else{
			int color=heap[0].color;
			pop_heap(heap.begin(), heap.end(), CompareMax());
			heap.pop_back();
			//printHeap(heap);
			success= solvePixelRec(b, i, j, heap, color);
		}

		
	}else{
		success=false;
	}

	if(!success){ //tenta outra cor!
		if(color==BLACK)
			b.mat[x][y]=WHITE;
		else
			b.mat[x][y]=BLACK;
		//print_board(b);
		if(updatePixels(b, x, y, heap)){
			//chama proximo pixel
			int i=heap[0].x;
			int j=heap[0].y;
			//printHeap(heap);
			if(heap[0].key==0 && b.mat[i][j]==NONE) //proximo pixel nao preenchido e probabilidade==0
				success = false;
			else{
				int color=heap[0].color;
				pop_heap(heap.begin(), heap.end(), CompareMax());
				heap.pop_back();
				success= solvePixelRec(b, i, j, heap, color);
			}
		
		}

	}

	//fail? 
	//reset e volte
	if(!success){
		//coloca pixel novamente na heap
		int blackPos, whitePos;
		//calcula possibilidade preto
		b.mat[x][y]=BLACK;
		blackPos = solve_row(b, x)*solve_col(b, y);
		//calcula possibilidade branco
		b.mat[x][y]=WHITE;
		whitePos = solve_row(b, x)*solve_col(b, y);
		b.mat[x][y]=NONE;

		HeapItem h;

		if(blackPos!=0){
			h = HeapItem(x, y, (float)blackPos/(blackPos+whitePos), BLACK);
			heap.push_back(h);
		}


		if(whitePos!=0){
			h = HeapItem(x, y, (float)whitePos/(blackPos+whitePos), WHITE);
			heap.push_back(h);
		}
	
		updatePixels(b, x, y, heap);

	}

	

	

	return success;
}




bool solveByPixel(Board &b){
	int i, j, k=0;
	int blackPos, whitePos;
	//cria heap - chave eh o n. de possibilidades para cada linha/coluna
	vector<struct HeapItem> heap;
	heap.resize(2*b.m*b.n);

	//insere linhas na heap
	for(i=1; i<=b.n; i++){
		for(j=1; j<=b.m; j++){
			//calcula possibilidade preto
			b.mat[i][j]=BLACK;
			blackPos = solve_row(b, i)*solve_col(b, j);
		
			//calcula possibilidade branco
			b.mat[i][j]=WHITE;
			whitePos = solve_row(b, i)*solve_col(b, j);
			b.mat[i][j]=NONE;

			//se os dois sao 0, fail
			if(blackPos==0 && whitePos==0)
				return false;

			//insere se !=0
			if(blackPos!=0){
				heap[k] = HeapItem(i, j, (float)blackPos/(blackPos+whitePos), BLACK);
				k++;
			}

			if(whitePos!=0){
				heap[k] = HeapItem(i, j, (float)whitePos/(blackPos+whitePos), WHITE);
				k++;
			}
				
		}
	};

	heap.resize(k);

	make_heap(heap.begin(), heap.end(), CompareMax());

	int color;

	//preenche inicialmente todos os pixels de probabilidade 1
	while(heap.size()>0 && heap[0].key==1){
		//remove primeiro item da heap
		i=heap[0].x;
		j=heap[0].y;
		b.mat[i][j]=heap[0].color;
		pop_heap(heap.begin(), heap.end(), CompareMax());
		heap.pop_back();
	}
	if(heap.size()==0) //verificando se puzzle nao foi preenchido pelo loop acima (por precaucao)
		return true;


	i=heap[0].x;
	j=heap[0].y;
	color=heap[0].color;
	pop_heap(heap.begin(), heap.end(), CompareMax());
	heap.pop_back();
	//chama resolucao para primeiro pixel
	return solvePixelRec(b, i, j, heap, color);


}

#endif