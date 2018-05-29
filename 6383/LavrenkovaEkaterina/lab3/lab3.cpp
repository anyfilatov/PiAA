#include <iostream>
#include <algorithm>

#define SIZE 300
#define noPath -1
#define inPath 0

using  namespace std;

int c[SIZE][SIZE] = { 0 };
int prevs[SIZE] = { 0 };
bool visited[SIZE] = { 0 };
int flow[SIZE][SIZE] = { 0 };
int istock, stock;

void initPrevs() {
	for (int i = 0; i<SIZE; i++)
		prevs[i] = noPath;
}

void bf(int v) {
	visited[v] = true;
	for (int i = 0; i<SIZE; i++)
		if (!visited[i] && (c[v][i] - flow[v][i] > 0 && c[v][i] != 0 || flow[v][i] < 0 && c[i][v] != 0)) {
			prevs[i] = v;
			bf(i);
		}
}

bool getPath(int v) {
	bf(v);
	for (int i = 0; i<SIZE; i++)
		visited[i] = false;
	return (prevs[stock] != noPath);
}

int _ff() {
	int maxFlow = 0;
	while (getPath(istock)) {
		int tmp = 10000000;
		for (int v = stock; 0 <= prevs[v]; v = prevs[v])
			tmp = min(tmp, c[prevs[v]][v] - flow[prevs[v]][v]);
		for (int v = stock; 0 <= prevs[v]; v = prevs[v]) {
			flow[prevs[v]][v] += tmp;
			flow[v][prevs[v]] -= tmp;
		}
		maxFlow += tmp;
		initPrevs();
	}
	return maxFlow;
}

int main() {
	try {
		initPrevs();
		int numb_edges = 0, cost = 0;
		char  left, rigth;
		cin >> numb_edges;
		cin >> left >> rigth;
		istock = left - '0';
		stock = rigth - '0';

		for (int k = 0; k < numb_edges; k++) {
			cin >> left >> rigth >> cost;
			int i = left - '0';
			int j = rigth - '0';
			c[i][j] = cost;
		}
		cout << _ff() << endl;
		for (int i = 0; i < SIZE; i++)
			for (int j = 0; j < SIZE; j++) {
				if (flow[i][j] != 0 && flow[i][j] < 0)
					flow[i][j] = 0;
				if (c[i][j] > 0)
					cout << (char)(i + '0') << " " << (char)(j + '0') << " " << flow[i][j] << endl;
			}
	}
	catch (int y) {
		return 0;
	}

	system("pause>nul");
}