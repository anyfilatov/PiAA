#include <iostream>

struct Writer
{
	int x;
	int y;
	int w;
};

class Square
{
  private:
	char *mas;
	int size;
	int kol;
	Writer *qwe;

	void insert(int x, int y, int size);
	void insert(int x, int y, int size, Writer &w);
	void remove(int x, int y, int size);
	bool Check(int x, int y, int size);

	void B_T_S(int x, int y, int size);
	bool B_T(const int &x0, const int &y0, const int &size, const int &kol, int r, int x, int y);

  public:
	Square(int size);
	~Square();

	void print();
	void GO();
};

Square::Square(int size)
{
	mas = new char[(size + 1) * (size + 1)]();
	this->kol = 0;
	this->size = size;
}

Square::~Square()
{
	delete mas;
}

void Square::print()
{
	std::cout << kol;
	for (int i = 0; i < kol; i++)
		std::cout << std::endl
				  << qwe[i].x + 1 << " " << qwe[i].y + 1 << " " << qwe[i].w;
}

void Square::insert(int x, int y, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mas[(i + y) * this->size + (j + x)] = size;
	kol++;
}

void Square::insert(int x, int y, int size, Writer &w)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mas[(i + y) * this->size + (j + x)] = size;

	w.x = x;
	w.y = y;
	w.w = size;
	kol++;
}

bool Square::Check(int x, int y, int size)
{
	if ((size > this->size - x) ||
		(size > this->size - y))
		return 0;

	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			if (mas[(i + y) * this->size + (j + x)])
				return 0;

	return 1;
}

void Square::remove(int x, int y, int size)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size; j++)
			mas[(i + y) * this->size + (j + x)] = 0;

	kol--;
}

void Square::GO()
{
	if (!(size % 2))
	{
		qwe = new Writer[4];
		insert(0, 0, size / 2, qwe[0]);
		insert(0, size / 2, size / 2, qwe[1]);
		insert(size / 2, 0, size / 2, qwe[2]);
		insert(size / 2, size / 2, size / 2, qwe[3]);

		return;
	}
	if (!(size % 3))
	{
		qwe = new Writer[6];
		insert(0, 0, size * 2 / 3, qwe[0]);
		insert(0, size * 2 / 3, size / 3, qwe[1]);
		insert(size * 2 / 3, 0, size / 3, qwe[2]);
		insert(size / 3, size * 2 / 3, size / 3, qwe[3]);
		insert(size * 2 / 3, size / 3, size / 3, qwe[4]);
		insert(size * 2 / 3, size * 2 / 3, size / 3, qwe[5]);

		return;
	}
	if (!(size % 5))
	{
		qwe = new Writer[8];
		insert(0, 0, size * 3 / 5, qwe[0]);
		insert(size * 3 / 5, size / 5, size * 2 / 5, qwe[1]);
		insert(size / 5, size * 3 / 5, size * 2 / 5, qwe[2]);
		insert(size * 3 / 5, size * 3 / 5, size * 2 / 5, qwe[3]);

		B_T(0, 0, size, 8, 4, size * 3 / 5, 0);
		return;
	}

	int m = size / 2 + 1;
	insert(0, 0, m);
	insert(0, m, m - 1);
	insert(m, 0, m - 1);

	B_T_S(m - 1, m - 1, m);

	qwe[kol - 3].x = 0;
	qwe[kol - 3].y = 0;
	qwe[kol - 3].w = m;
	qwe[kol - 2].x = 0;
	qwe[kol - 2].y = m;
	qwe[kol - 2].w = m - 1;
	qwe[kol - 1].x = m;
	qwe[kol - 1].y = 0;
	qwe[kol - 1].w = m - 1;
}

void Square::B_T_S(int x, int y, int size)
{

	for (int i = 2; i < size * size; i++)
	{
		qwe = new Writer[i + 3];
		if (B_T(x, y, size, i, 0, 0, 0))
			break;
		delete qwe;
	}
}

bool Square::B_T(const int &x0, const int &y0, const int &size, const int &kol, int r, int x, int y)
{
	if (kol == r)
	{
		for (int i = 0; i < size; i++)
		{
			if (!(mas[(y0 + i) * this->size + (x0 + size - 1)] &&
				  mas[(y0 + size - 1) * this->size + (x0 + i)]))
				return false;
		}

		return true;
	}

	while ((mas[(y0 + y) * this->size + (x0 + x)]) || (x == size))
	{
		if (x >= size)
		{
			if (y == size)
				return true;
			y++;
			x = 0;
		}
		else
			x++;
	}

	for (int i = 1; i < size; i++)
	{
		if (Check(x0 + x, y0 + y, i))
		{
			insert(x0 + x, y0 + y, i, qwe[r]);
			r++;

			if (B_T(x0, y0, size, kol, r, x + i, y))
				return true;

			remove(x0 + x, y0 + y, i);
			r--;
		}
	}

	return false;
}

int main()
{
	int N;
	std::cin >> N;
	Square a(N);
	a.GO();

	a.print();
	return 0;
}