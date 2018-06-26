#include <iostream>
#include <vector>

using namespace std;


class Square
{
	int size;
	vector< vector<int> > square;
	int numberOfSquares;

public:

	Square(int size) : size(size)
	{
		numberOfSquares = 0;
		square.resize(size);
		for (unsigned i = 0; i < square.size(); i++)
			square[i].resize(size);
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				square[i][j] = 0;

		if (size != 0) {
			int tmp = size / 2;
			putIn(0, 0, tmp + 1);
			putIn(0, tmp + 1, tmp);
			putIn(tmp + 1, 0, tmp);
		}
	}

	int getSize()
	{
		return size;
	}

	int getNumberOfSquares()
	{
		return numberOfSquares;
	}

	bool isFreeCell(int i, int j)
	{
		for (int l = i; l < size; l++)
			for (int m = 0; m < size; m++)
				if (square[l][m] == 0)
					return true;
		return false;

	}

	int getNextFreeI(int i, int j)
	{
		for (int l = i; l < size; l++)
			for (int m = 0; m < size; m++)
				if (square[l][m] == 0)
					return l;
	}

	int getNextFreeJ(int i, int j)
	{
		for (int l = i; l < size; l++)
			for (int m = 0; m < size; m++)
				if (square[l][m] == 0)
					return m;
	}

	bool isPossibleToPut(int i, int j, int k)
	{
		if ((i + k) > size || (j + k) > size)
			return false;
		for (int l = i; l < i + k; l++)
			for (int m = j; m < j + k; m++)
				if (square[l][m] != 0)
					return false;
		return true;
	}

	void putIn(int i, int j, int k)// k - size of inserted square
	{
		for (int l = i; l < i + k; l++)
			for (int m = j; m < j + k; m++)
				square[l][m] = k;
		//printSquare();
		//cout << endl;
		numberOfSquares++;
	}

	void printSquare()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				cout << square[i][j];
			cout << endl;
		}
	}

	void zero(int i, int j)
	{
		int k = square[i][j];
		for (int l = i; l < i + k; l++)
			for (int m = j; m < j + k; m++)
				square[l][m] = 0;
	}
	void printAllSquares()
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
				if (square[i][j] != 0)
				{
					cout << i + 1 << " " << j + 1 << " " << square[i][j] << endl;
					zero(i, j);
				}
		}
	}
};

int minNumberOfSquares = 0;
Square best(0);

Square backTracking(Square square, int i, int j)
{
	// k - size of inserted square
	for (int k = square.getSize() / 2; k > 0; k--)
	{
		Square condition = square;//fix condition before going next
		if (condition.isPossibleToPut(i, j, k))
		{
			condition.putIn(i, j, k);// if it's possible square will be inserted into cells
			if (condition.isFreeCell(i, j))//check presence of free cells
				condition = backTracking(condition, condition.getNextFreeI(i, j), condition.getNextFreeJ(i, j));//go to free cell
			else if (condition.getNumberOfSquares() <= minNumberOfSquares)
			{
				minNumberOfSquares = condition.getNumberOfSquares();
				best = condition;
			}
		}

	}
	return square;
}

int main()
{
	int n;
	cin >> n;
	if (n % 2 == 0) {
		int temp = n / 2;
		cout << "4" << endl;
		cout << "1 1 " << temp << endl;
		cout << temp + 1 << " 1 " << temp << endl;
		cout << "1 " << temp + 1 << " " << temp << endl;
		cout << temp + 1 << " " << temp + 1 << " " << temp << endl;
		system("pause");
		return 0;
	}
	if (n % 3 == 0) {
		int temp = n / 3;
		cout << "6" << endl;
		cout << "1" << " 1 " << temp * 2 << endl;
		cout << "1 " << 1 + temp * 2 << " " << temp << endl;
		cout << 1 + temp << " " << 1 + temp * 2 << " " << temp << endl;
		cout << 1 + temp * 2 << " 1 " << temp << endl;
		cout << 1 + temp * 2 << " " << 1 + temp << " " << temp << endl;
		cout << 1 + temp * 2 << " " << 1 + temp * 2 << " " << temp << endl;
		system("pause");
		return 0;
	}

	Square square(n);
	minNumberOfSquares = n*n;
	square = backTracking(square, square.getNextFreeI(0, 0), square.getNextFreeJ(0, 0));
	cout << minNumberOfSquares << endl;
	//best.printSquare();
	best.printAllSquares();
	system("pause");
	return 0;
}

