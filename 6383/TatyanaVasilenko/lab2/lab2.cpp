#include <fstream>
#include <iostream>
#include <cfloat>

typedef double uz;

class Graph
{
private:
	uz *mas;			// Граф
	uz *dist;			// Дистанции от нач. вершины к такой-то вершине
	char *prew;			// Предыдущая вершина на пути следования ннач->конеч
	char *name;			// Вершины, исп.-е в графе
	char size;			// Количество вершин в графе
	bool *check;

	char found_min(char th, char end);		// до какой из нерасмотренных вершин ближе от th
	void print_way(char beg, char end);		// Напечатать существующий путь от beg к end
	char accordance(char h);				// Номер вершины в таблице имен

	char f(char end, char *h);

	bool GREEDY(char th, char end);			// Основной механизм жадного алгоритма

public:
	Graph(std::istream &in);				// Конструктор 
	~Graph();								// Деструктор


	void Greed(char beg, char end);			// Пусковая устоновка Жадности
	void A_star(char beg, char end);		// Запуск Звёздочки
	
	void print();							// Пишем основную инфу о графе
};


// Конструктор
Graph::Graph(std::istream &in)
{
	mas = new uz[28*28];
	for (int i = 0; i < 28 * 28; i++)
		mas[i] = -1;
	size = 0;

	while (!in.eof())
	{
		char a, b;
		uz c;
		in >> a >> b >> c;

		a -= 'a';
		b -= 'a';

		mas[(int)a * 28 + (int)b] = c;

		if (mas[(int)a * 28 + (int)a] == -1) { mas[(int)a * 28 + (int)a]--; size++; }
		if (mas[(int)b * 28 + (int)b] == -1) { mas[(int)b * 28 + (int)b]--; size++; }
	}

	dist = new uz[size];
	name = new char[size];
	prew = new char[size];
	check = new bool[size]();


	int i;	// Гуляем по графу
	i = 0;
	for (int k = size; k > 0; k--)
	{
		while (mas[i * 28 + i] == -1) i++;

		dist[size - k] = DBL_MAX;
		name[size - k] = (char)i;

		mas[name[size - k] * 28 + name[size - k]] = -1;

		i++;
	}
}


// Деструктор
Graph::~Graph()
{
	delete[] mas;
	delete[] dist;
	delete[] prew;
	delete[] name;
	delete[] check;
}



// Ищем соответсвие вершины графа месту в таблице имён
char Graph::accordance(char h)
{
	h -= 'a';
	for (int i = 0; i < size; i++)
	{
		if (h == name[i]) return i;
	}
	throw " ";
}


// Распечатать основную инфу о графе
void Graph::print()
{
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j < size; j++)
			std::cout << mas[name[i] * 28 + name[j]] << "	";

		std::cout << "\n";
	}
	std::cout << "\n\n";

	for (int i = 0; i < size; i++)
		std::cout << dist[i] << " ";

	std::cout << "\n";

	for (int i = 0; i < size; i++)
		std::cout << (char)(name[i] + 'a') << " ";

	std::cout << "\n";

	for (int i = 0; i < size; i++)
		std::cout << (char)(prew[i] + 'a') << " ";

	std::cout << "\n";
}


// Нарисовать путь от beg до end
void Graph::print_way(char beg, char end)
{
	if (end != beg)
	{
		print_way(beg, prew[end]);
	}

	std::cout << (char)(name[end] + 'a');
}
//////////////////////////////////////////////////



// Пусковая устоновка Жадности
void Graph::Greed(char beg, char end)
{
	beg = accordance(beg);
	end = accordance(end);


	if (GREEDY(beg, end))
	{
		print_way(beg, end);
	}
}


// ЖАДНОСТЬ
bool Graph::GREEDY(char th, char end)
{
	if (th == end) return true;


	check[th] = true;

	char r;

	while ((r = found_min(th, end)) != -1)
	{
		if (GREEDY(r, end))
		{
			prew[r] = th;
			return true;
		}
	}
	return false;
}


// До какой из нерасмотренных вершин ближе от th
char Graph::found_min(char th, char end)
{
	if (mas[name[th] * 28 + end] != -1) return end;

	char ret = -1;
	uz d = DBL_MAX;

	for (char i = 0; i < size; i++)
	{
		if ((!check[i]) && (mas[name[th] * 28 + name[i]] != -1) && (mas[name[th] * 28 + name[i]] < d))
		{
			ret = i;
			d = mas[name[th] * 28 + name[i]];
		}
	}
	return ret;
}

/////////////////////////////////////////////////

// Запуск Звёздочки
void Graph::A_star(char beg, char end)
{
	beg = accordance(beg);
	end = accordance(end);

	char *h = new char[size];
	for (int i = 0; i < size; i++)
	{
		h[i] = abs(name[end] - name[i]);
	}

	check[beg] = true;

	if (beg != end)
	{
		dist[beg] = 0;

		char s = 0;
		for (char i = 0; i < size; i++)		// Цикл "пока не най"
			if (mas[name[beg] * 28 + name[i]] != -1)
			{
				dist[i] = mas[name[beg] * 28 + name[i]];
				prew[i] = beg;
			}


		while ((s = f( end, h)) != end)
		{
			check[s] = true;

			for (int i = 0; i < size; i++)
			{
				double d = mas[name[s] * 28 + name[i]];

				if ((d != -1) && !check[i] && (d + dist[s] < dist[i]))
				{
					prew[i] = s;
					dist[i] = d + dist[s];
				}
			}
		}
	}

	print_way(beg, end);
}


// Эвристическая функция для звёздочки
char Graph::f( char end, char *h)
{
	char ret = -1;
	uz d = DBL_MAX;


	for (char i = 0; i < size; i++)
	{
		if (!check[i] && dist[i] + h[i] <= d)
		{
			if (ret == i)
				ret = h[ret] < h[i] ? ret : i;

			ret = i;
			d = dist[i] + h[i];
		}
	}
	return ret;
}


///////////////////////////////////
///////////////////////////////////

int main()
{
	char a, b;

	std::cin >> a >> b;

	Graph qw(std::cin);

	qw.A_star(a, b);

	return 0;
}