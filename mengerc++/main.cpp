#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

int mostrar(vector<vector<int>> matrix) {
	cout << endl;
	for (const auto& fila : matrix) {
		for (const auto& valor : fila) {
			cout << valor << " ";
		}
		cout << endl;
	}
	cout << endl;
	return 0;
}

pair<vector<vector<int>>, vector<pair<int,int>>> crear_lista(int f, int c) {
	vector<vector<int>> lista(f, vector<int>(c, 1));
	if (f > 1 && c > 1) lista[1][1] = 0;
	vector<pair<int,int>> queque = {
		{0,0}, {0,1}, {0,2},
		{1,0}, {1,2},
		{2,0}, {2,1}, {2,2}
	};
	return {lista, queque};
}

pair<vector<vector<int>>, vector<pair<int,int>>> expand(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	int fl=matrix.size();
	int cl=matrix[0].size();
	vector<vector<int>> nueva_matrix(fl*3, vector<int>(cl*3, 0));
	for(int i=0; i<fl; i++) {
		for(int j=0; j<cl; j++) {
			if(matrix[i][j]==1) {
				if(find(queque.begin(), queque.end(), make_pair(i,j))== queque.end()) {
					queque.push_back({i,j});
				}
				for (int row = i*3; row < (i+1)*3; row++) {
					for (int col = j*3; col < (j+1)*3; col++) {
						nueva_matrix[row][col] = 1;
					}
				}
			}
		}
	}
	return {nueva_matrix, queque};
}

vector<vector<int>> remove(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	for (const auto& cell : queque) {
		int row = cell.first * 3 + 1;
		int col = cell.second * 3 + 1;
		matrix[row][col] = 0;
	}
	return matrix;
}

vector<vector<int>> computar2d(vector<vector<int>> matrix, int ciclos, vector<pair<int,int>> queque) {
	vector<vector<int>> lista_final = matrix;
	pair<vector<vector<int>>, vector<pair<int,int>>> result;

	for (int i=0; i<ciclos; i++) {
		result = expand(lista_final, queque);
		lista_final = result.first;
		queque = result.second;
		lista_final = remove(lista_final, queque);
		mostrar(lista_final);
	}
	return lista_final;
}

vector<vector<vector<int>>> computar3d(vector<vector<int>> matrix2d) {
	int tm = matrix2d.size();
	vector<vector<vector<int>>> matrix3d(tm, vector<vector<int>>(tm, vector<int>(tm, 1)));
	for (int i=0; i<tm; i++) {
		for (int j=0; j<tm; j++) {
			for (int k=0; k<tm; k++) {
				if (matrix2d[i][j] == 0) {
					matrix3d[i][j][k] = 0;
					matrix3d[i][k][j] = 0;
					matrix3d[k][i][j] = 0;
				}
			}
		}
	}
	return matrix3d;
}

int main() {

	int f = 3, c = 3;
	int ciclos = 3;
	vector<vector<vector<int>>> matrix3d;
	vector<vector<int>> matrix2d;
	vector<pair<int,int>> queque;
	
	tie(matrix2d, queque) = crear_lista(f, c);
	
	mostrar(matrix2d);

	computar2d(matrix2d, ciclos, queque);
	
	matrix3d = computar3d(matrix2d);
	
}

