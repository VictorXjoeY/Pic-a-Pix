#include <cstdio>
#include <vector>
#include <cassert>

using namespace std;

int main(){
	vector<vector<int> > mat;
	vector<vector<int> > row;
	vector<vector<int> > col;
	int cur, n, m, i, j;

	// Lendo a matriz.
	assert(scanf("%d%d", &n, &m) == 2);

	mat.resize(n + 1);
	row.resize(n + 1);
	col.resize(m + 1);

	for (i = 1; i <= n; i++){
		mat[i].resize(m + 1);

		for (j = 1; j <= m; j++){
			assert(scanf("%1d", &mat[i][j]) == 1);
		}
	}

	// Criando as restrições das linhas.
	for (i = 1; i <= n; i++){
		for (j = 1, cur = 0; j <= m; j++){
			if (mat[i][j] == 1){
				cur++;
			}
			else{
				if (cur > 0){
					row[i].push_back(cur);
					cur = 0;
				}
			}
		}

		if (cur > 0){
			row[i].push_back(cur);
			cur = 0;
		}
	}

	// Criando as restrições das colunas.
	for (j = 1; j <= m; j++){
		for (i = 1, cur = 0; i <= n; i++){
			if (mat[i][j] == 1){
				cur++;
			}
			else{
				if (cur > 0){
					col[j].push_back(cur);
					cur = 0;
				}
			}
		}

		if (cur > 0){
			col[j].push_back(cur);
			cur = 0;
		}
	}

	// Imprimindo as restrições
	printf("%d %d\n\n", n, m);

	for (i = 1; i <= n; i++){
		printf("%d", (int)row[i].size());

		for (j = 0; j < (int)row[i].size(); j++){
			printf(" %d", row[i][j]);
		}

		printf("\n");
	}

	printf("\n");

	for (j = 1; j <= m; j++){
		printf("%d", (int)col[j].size());

		for (i = 0; i < (int)col[j].size(); i++){
			printf(" %d", col[j][i]);
		}

		printf("\n");
	}

	return 0;
}