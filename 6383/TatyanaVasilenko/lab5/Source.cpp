#include <iostream>
#include <fstream>
#include <vector>
#include <string>


// A =0; C = 1; G = 2; T = 3; N =4;
int lapsum(char c);

// Структура вершин автомата-бора
struct Pic
{
	int out[6];					// номер вершины, в которую мы придем по символу с номером i в алфавите
	int move[6];				// Запоминание перехода автомата
	int par;					// "отцовая вершина"
	std::vector<int> num_str;	// номера строки-образца, обозначаемого этой вершиной
	bool f;						// true - вершина - исходная строка, false - нет
	int suff;					// Ссылка на суффикс
	int suff_g;					// Ссылка на "хороший" суффикс
	char c;						// символ на ребре от par сюда 



	Pic(int par, char ch);
};

Pic::Pic(int par, char c) : par(par), f(false), suff(-1), suff_g(-1), c(c)
{
	for (int i = 0; i < 6; i++)
		out[i] = move[i] = -1;
}






///////////////////////////////////////////////////
///////////////////////////////////////////////////
class Bor
{
private:
	std::vector<Pic> bor;						// Бор
	std::vector< std::string > str;				// Шаблоны

	int get_suff(int v);						// Получение суффикса
	int get_move(int v, char ch);				// Куда пройти из v по ch
	int get_suff_g(int v);						// Получение хорошего суффикса

	std::vector<int> it;						// "Массив" начал подшаблонов
	int kol;									// Количество подшаблонов
	int lenght;									// Общая длина подшаблона с масками

	//!!!!!!!!!!!!!!!!!!!!!!!!!!!
	void way(int v, int i);						// Просматривает все "хорошие" суффиксы\ в случае совпадения с шаблоном-> результат
	void way_M(int v, int i, int *C, int p);	// Аналогично, но только для шаблона с маской



	void in_bor(std::string &, int);			// Добавление шаблона в бор

public:
	Bor(std::istream &in);						//Конструктор для нескольких шаблонов
	Bor(std::string &P, char mask);				//Конструктор для шаблона с маской

	void AKG(const std::string &s);				// Запуск Ахо-Корасик
	void AKG_M(const std::string &s);			// Запуск Ахо-Корасик для шаблона с маской

};

// Конструктор для нескольких шаблонов
Bor::Bor(std::istream &in)
{
	int kol;
	in >> kol;

	// Создаём первый элемент(первую вершину) бора
	bor.push_back(*(new Pic(-1, (char)0)));

	std::string dop;

	// Считываем все шаблоны
	for (int i = 0; i < kol; i++)
	{
		in >> dop;
		str.push_back(dop);	// добавляем в массив шаблонов

		// и вставляем считанный шаблон в бор
		in_bor(dop, i);
	}
}

// Конструктор для шаблона с маской
Bor::Bor(std::string &P, char mask)
{
	kol = 0;									// Сколько в шаблоне подшаблонов
	lenght = P.length();						// Сохраняем общую длину шаблона
	P += mask;
	int from = 0;								// Номер символа, просматриваемоо шаблона (помогает найти подшаблоныны)
	bor.push_back(*(new Pic(-1, (char)0)));		// Создаём первый элемент(первую вершину) бора

	// Поиск подшаблонов с добавлением в бор
	for (int i = 0; i < P.length(); i++)
	{
		// Если символ не джокер - игнорим и идём дальше
		if (P[i] != mask)
			continue;

		// Если джокер "подряд" - игнорим
		if (i - from == 0)
		{
			from = i + 1;
			continue;
		}


		str.push_back(P.substr(from, i - from));		// "Создаём" подшаблон
		it.push_back(from);								// Место, откуда в шаблоне начинается подшаблон

		in_bor(str[kol], kol);							// Добавляем в бор

		kol++;
		from = i + 1;
	}
}




// Просматривает все "хорошие" суффиксы\ в случае совпадения с шаблоном-> результат
void Bor::way(int v, int i)
{
	// Просматриваем все хорошие суффиксы
	for (int u = v; u != 0; u = get_suff_g(u))
	{
		if (bor[u].f)
		{
			std::cout << i - str[bor[u].num_str[0]].length() + 1 << " " << bor[u].num_str[0] + 1 << std::endl;
		}
	}
}

// Просматривает все "хорошие" суффиксы\ в случае совпадения с шаблоном-> результат
void Bor::way_M(int v, int i, int *C, int p)
{
	for (int u = v; u != 0; u = get_suff_g(u))
	{
		if (bor[u].f)
		{
			std::vector<int> a = bor[u].num_str;
			for (int j = 0; j < a.size(); j++)
			{
				int pq = i - str[a[j]].length() - it[a[j]];

				if ( (pq + lenght <= p) && p>=0)
					C[pq]++;
			}
		}
	}
}



//;;;;;;;;;;;;;
void Bor::AKG(const std::string &s)
{
	int u = 0;
	for (int i = 0; i < s.length(); i++)
	{
		u = get_move(u, s[i]);
		way(u, i + 1);
	}
}

void Bor::AKG_M(const std::string &s)
{
	int u = 0;
	int *C = new int[s.length()]();
	for (int i = 0; i < s.length(); i++)
	{
		u = get_move(u, s[i]);
		way_M(u, i + 1, C, s.length());
	}

	// Распечатываем итог
	for (int i = 0; i < s.length(); i++)
	{
		if (C[i] == kol)
			std::cout << i + 1 << std::endl;
	}

}


////////////////////////////////////////////////////////////////////////
int main()
{
	system("color 0a");

	std::ifstream fin("IN.txt");

	//std::string T;
	//fin >> T;
	//Bor qw(fin);
	//qw.AKG(T);
	// Для поиска шаблонов
	
	
	//std::string T, P;
	//char mask;
	//fin >> T >> P >> mask;
	//Bor qw(P, mask);
	//qw.AKG_M(T);
	// Для поиска шаблона с маской
	
	
	Bor qw(P, mask);
	qw.AKG_M(T);

	system("pause");
}


// Выдаёт соответствие символа алфавита его номеру
int lapsum(char c)
{
	switch (c)
	{
	case 'A': return 0;
	case 'C': return 1;
	case 'G': return 2;
	case 'T': return 3;
	case 'N': return 4;
	default: return 5;
	}
}

////////////////////////////////////////////////////
int Bor::get_suff(int v)
{
	if (bor[v].suff == -1)
	{
		if (v == 0 || bor[v].par == 0)
			bor[v].suff = 0;
		else
			bor[v].suff = get_move(get_suff(bor[v].par), bor[v].c);
	}
	return bor[v].suff;
}


int Bor::get_move(int v, char ch)
{
	int CH = lapsum(ch);
	if (bor[v].move[CH] == -1)
	{
		if (bor[v].out[CH] != -1)
			bor[v].move[CH] = bor[v].out[CH];
		else
		{
			if (!v)
				bor[v].move[CH] = 0;
			else
				bor[v].move[CH] = get_move(get_suff(v), ch);
		}
	}
	return bor[v].move[CH];
}


// Получение хорошего суффикса (который является шаблоном)
int Bor::get_suff_g(int v)
{
	if (bor[v].suff_g == -1);
	{
		int u = get_suff(v);
		if (!u)
			bor[v].suff_g = 0;
		else
			bor[v].suff_g = (bor[u].f) ? u : get_suff_g(u);
	}
	return bor[v].suff_g;
}



void Bor::in_bor(std::string &dop, int i)
{
	int num = 0;
	for (int j = 0; j < dop.length(); j++)
	{
		int ch = lapsum(dop[j]);

		if (bor[num].out[ch] == -1)
		{
			bor.push_back(*(new Pic(num, dop[j])));
			bor[num].out[ch] = bor.size() - 1;
		}
		num = bor[num].out[ch];
	}
	bor[num].f = true;
	bor[num].num_str.push_back(i);
}