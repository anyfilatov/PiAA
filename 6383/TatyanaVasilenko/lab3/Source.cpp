#include <iostream>
#include <fstream>
#include <limits>

int compare(const void * x1, const void * x2);

struct IN_OUT
{
	char a;		// Исток
	char b;		// Сток
	int  c;		// Поток
};


class Graph
{
private:
	int size;			// Размер
	IN_OUT *c;			// Пропускная способность

	bool way(IN_OUT * f, IN_OUT * cf, char beg, char end);
	bool way_to_end(char * b, IN_OUT * cf, char beg, char end, int &min);

	void print(IN_OUT *c, char beg);

public:
	Graph(int size, std::istream &in);
	~Graph();

	void print();
	

	void Ford(char beg, char end);

};


// Конструктор
Graph::Graph(int size, std::istream &in) : size(size), c(size ? new IN_OUT[size] : NULL)
{
	for (int i = 0; i < size; i++)
	{
		in >> c[i].a >> c[i].b >> c[i].c;
	}

	qsort(c, size, sizeof(IN_OUT), compare);
}

// Деструктор
Graph::~Graph()
{
	delete[] c;
}




// Пуск Форда-Фалкрисона
void Graph::Ford(char beg, char end)
{
	IN_OUT * f = new IN_OUT[size];		// Потоки
	IN_OUT *cf = new IN_OUT[size];		// Остаточная сеть

	// Инициализируем поток и остаотчную сеть
	for (int i = 0; i < size; i++)
	{
		f[i].a = cf[i].a = c[i].a;
		f[i].b = cf[i].b = c[i].b;

		 f[i].c = 0;
		cf[i].c = c[i].c;
	}

	// Повторять пока не закончаца пути в сf
	while (way(f, cf, beg, end));


	print(f, beg);

	delete[] f;
	delete[] cf;
}


// Нахождение пути в остаточной сет
bool Graph::way(IN_OUT * f, IN_OUT * cf, char beg, char end)
{
	// Какие рёбра задействованы в найденном пути
	char *b = new char[size]();

	int r;
	for (r = 0; r < size, cf[r].a != beg; r++);		// Доходим до ребер из beg

	// Помечаем все ребра из beg как ребра из истока
	for (int i = r; i < size, cf[i].a == beg; i++)
	{
		b[i] = 2;
	}

	// Минимальная пропускная способность найденного пути
	int min = INT_MAX;

	// Если путь не найден - значит их больше нет
	if (!way_to_end(b, cf, beg, end, min)) 
	{ 
		delete[] b; 
		return false; 
	}

	// Пробегаем по всем рёбрам
	for (int i = 0; i < size; i++)
	{
		// Если ребро принадлежит пути - изменяем поток по этому ребру
		if (b[i] == 1)
		{
			f[i].c += min;
			cf[i].c = c[i].c - f[i].c;
		}
	}

	delete[] b;
	return true;
}


// Нахождение пути от beg до end в остаточной сети
bool Graph::way_to_end(char * b, IN_OUT * cf, char beg, char end, int &min)
{
	if (beg == end) return true;

	// "Адрес" начала пака ребер с истоком beg
	int r;
	for (r = 0; r < size; r++)
	{
		if (cf[r].a == beg) break;
	}

	// На случай если из вершины beg можем сразу попасть в end
	for (int i = r; i < size, cf[i].a == beg; i++)
	{
		if (cf[i].b == end && cf[i].c)
		{
			b[i] = 1;

			min = cf[i].c < min ? cf[i].c : min;
			return true;
		}
	}

	// Проверяем данный "пак"
	for (int i = r; i < size, cf[i].a == beg; i++)
	{
		// Если сквозь остаточное ребро ещё можно что-то пустить
		if ( cf[i].c && b[i] >= 0 )
		{
			// Помечаем, по каким ребрам нельзя ходить ввиду посещенности вершины b
			for (int j = 0; j < size; j++)
			{
				if (cf[j].b == cf[i].a && !b[j])
					b[j] = -1;
			}
			

			// Если путь с использование этого ребра найден
			if (way_to_end(b, cf, cf[i].b, end, min))
			{
				b[i] = 1;
				min = cf[i].c < min ? cf[i].c : min;
				return true;
			}
			b[i] = -1;
		}
	}
	
	return false;
}


///////////////////////////
int main()
{
	system("color 0a");
	std::ifstream fin("IN.txt");

	int c;
	char a, b;

	fin >> c >> a >> b;

	Graph qw(c, fin);


	qw.Ford(a, b);


	system("pause");
	return 0;
}

/////////////////////////////

int compare(const void * x1, const void * x2)   // функция сравнения элементов массива
{
	int c = (*(IN_OUT*)x1).a - (*(IN_OUT*)x2).a;

	if (c)
		return c;

	return (*(IN_OUT*)x1).b - (*(IN_OUT*)x2).b;
}


///////////////////////////////
void Graph::print()
{
	for (int i = 0; i < size; i++)
	{
		std::cout << c[i].a << " " << c[i].b << " " << c[i].c << "\n";
	}
}



void Graph::print(IN_OUT *c, char beg)
{
	int qq = 0;
	int r;
	for (r = 0; r < size; r++)
	{
		if (c[r].a == beg) break;
	}


	for (int i = r; i < size; i++)
	{
		if (c[i].a != beg) break;

		qq += c[i].c;
	}

	std::cout << qq;

	for (int i = 0; i < size; i++)
	{
			std::cout << "\n" << c[i].a << " " << c[i].b << " " << c[i].c;
	}
}
