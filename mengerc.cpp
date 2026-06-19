#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

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

vector<vector<int>> expand(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	int fl=matrix.size();
	int cl=matrix[0].size();
	vector<vector<int>> nueva_matrix(fl*3, vector<int>(cl*3, 1));
	for(int i=0; i<fl; i++) {
		for(int j=0; j<cl; j++) {
			if(matrix[i][j]==0) {
				//if(find(queque.begin(), queque.end(), make_pair(i,j))== queque.end()) {
					queque.push_back({i,j});
				//}
				for (int row = i*3; row < (i+1)*3; row++) {
					for (int col = j*3; col < (j+1)*3; col++) {
						nueva_matrix[row][col] = 0;
					}
				}
			}
		}
	}
	cout << queque.size() << endl;
	//for (const auto& cell : queque) {
	//	cout << "Queque: (" << cell.first << ", " << cell.second << ")" << endl;
	//}
	return nueva_matrix;
}

vector<vector<int>> remove(vector<vector<int>> matrix, vector<pair<int,int>> queque) {
	for (const auto& cell : queque) {
		int row = cell.first * 3 + 1;
		int col = cell.second * 3 + 1;
		matrix[row][col] = 0;
	}
	return matrix;
}

int main() {
	int f = 3, c = 3;
	int ciclos = 2;
	pair<vector<vector<int>>, vector<pair<int,int>>> lista= crear_lista(f, c);
	vector<vector<int>> matrix = lista.first;
	vector<pair<int,int>> queque = lista.second;
	//vector<vector<int>> nueva_lista = expand(matrix, queque, f, c);
	//vector<vector<int>> lista_final = remove(nueva_lista, queque);
	vector<vector<int>> lista_final = matrix;

	for (auto& fila : lista_final) {
			for (auto& valor : fila) {
				cout << valor << " ";
			}
			cout << endl;
		}

	for (int i=0; i<ciclos; i++) {
		lista_final = expand(lista_final, queque);
		lista_final = remove(lista_final, queque);
		for (auto& fila : lista_final) {
			for (auto& valor : fila) {
				cout << valor << " ";
			}
			cout << endl;
		}
	}

}

