#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <stack>
using namespace std;
vector<double> g,f;
vector<char> from_vec;
double h(char start, char end)          //эвристическая функция
{
	return abs((double)start - (double)end);
}

typedef struct Graph
{
	char from;        //parent
	char to;	  //son
	double weight;	  //edge
}Graph;

char min_f(vector<char> &open)      //функция для нахождения вершины с минимальным
{				    //значением f
	char ret;
	double min = numeric_limits<double>::max();
	for(auto elem: open)
	{
		if(f[(int)elem - 97] < min)
		{
			min = f[(int)elem - 97];
			ret = elem;
		}
	}
	return ret;
}

void remove(char curr, vector<char> &open)    //удаление элемента
{
	for(int i = 0; i < open.size(); i++)
	{
		if(open[i] == curr)
			open.erase(open.begin()+i);
	}
}

bool IsIn(char symbol, vector<char> &list)   //содержится ли символ в векторе
{
	for(int i=0;i<list.size();i++)
	{
		if(symbol == list[i])
			return true;
	}
	return false;
}

void UnclosedNeighbours(char curr, vector<Graph> &graph, vector<char> &unclosed, vector<char> &closed)
{
	for(auto elem: graph)
	{
		if(elem.from == curr && !IsIn(elem.to, closed)) //если curr - родитель и сын не в closed
			unclosed.push_back(elem.to);     // добавить сына в unclosed
	}
}

double dist(char curr, char neighbour, vector<Graph> &graph)  //возвращает вес ребра
{
	for(auto elem: graph)
	{
		if(elem.from == curr && elem.to == neighbour)    
			return elem.weight;
	}
}

int setWay(stack<char> &result, char start, char end)
{
	result.push(end);
	if(start == end) 
	{
		return 0;
	}
    setWay(result, start, from_vec[(int)end - 97]);//вызвать функцию, в качестве end передать
}						   //предпоследнюю вершину пути

int main()
{
	vector<char> unclosed; 
	vector<Graph> graph; //граф
	Graph tmp;
	double resWeight = 0;  //будет записан суммарный вес пути
	vector<char> closed;   //вектор закрытых вершин
	char start, finish, curr; 
	cout << "Введите стартовую вершину(только английские буквы) "<<endl;
	for(;;)	
	{	
		cin >> start;
		if(abs((int)start - 97) >= 26)
			{ cout << "Неверный символ, попытайтесь еще!"<<endl; continue;}
		else break;
	}
	cout << "Введите конечную вершину(только английские буквы) "<<endl;
	do	
	{	
		cin >> finish;
		if(abs((int)finish - 97) >= 26)
			cout << "Неверный символ, попытайтесь еще!"<<endl;
	}while(abs((int)finish - 97) >= 26);
	for(int i = 0; i < 26; i++)
	{
		g.push_back(numeric_limits<double>::max()-27); //заполняем вектор g
	}
    for(int i = 0; i < 26; i++)
	{
		f.push_back(g[i] + h((char)(i+97), finish));//заполняем вектор f
	}
	for(int i = 0;i < 26; i++)
	{				//заполняем from_vec значениями '!', если вместо '!'
		from_vec.push_back('!');//записана буква, значит в эту вершину мы можем попасть 
	}				//из вершины, соответствующей индексу 
 
	vector<char> open; //список открытых вершин
	open.push_back(start);
	stack<char> result;//стэк с резульирующим путем
	g[(int)start - 97] = 0;
	f[(int)start - 97] = g[(int)start - 97] + h(start, finish);
	char start1, finish1;
	double weight, temp_g;
	cout <<"Введите граф "<<endl;
	while(cin >> start1)
	{
		cin >> finish1 >> weight;
		tmp.from = start1;
		tmp.to = finish1;
		tmp.weight = weight;
		graph.push_back(tmp);
	}
	cout << "Начало работы алгоритма" << endl;
	while(open.size()!=0)
	{
		cout << open.size() << " элементов в Open" << endl;
		curr = min_f(open);
		cout <<" Вершина с минимальным значением функции F из Open = "<<curr<<" рассматриваем её"<<endl;
		if(curr == finish)
		{
			cout<<"  Рассматриваемая вершина - конечная, следовательно кратчайший путь из начальной вершины в конечную: ";
			setWay(result, start, finish);
			while(!result.empty())
			{
				cout << result.top();
				result.pop();
			}
			cout << ", weight: " <<resWeight<<endl;
			return 0; //ADD 
		}
		remove(curr, open);
		cout <<"  "<< curr << " удалён из Open" << endl;
		closed.push_back(curr);
		cout <<"   "<< curr <<" добавлен в Closed" << endl;
		UnclosedNeighbours(curr, graph, unclosed, closed);
		cout <<"    Сыновья для вершины "<<curr << ", которые не находятся в Closed: ";
		for(auto neighbour: unclosed)
			cout << neighbour << ' ';
		cout << endl;
		for(auto neighbour: unclosed)
		{
			cout<<"     Рассматриваем сына "<<neighbour<<endl;
			temp_g = g[(int)curr - 97] + dist(curr, neighbour, graph);
			cout << "      Временное значение G("<<neighbour<<") = G("<<curr<<") + weight("<<curr<<", "<<neighbour<<") = " << g[(int)curr - 97]<<" + "<<dist(curr,neighbour,graph)<<" = "<<temp_g<<endl; 
			if(!IsIn(neighbour, open) || temp_g < g[(int)neighbour - 97])
			{
				if(!IsIn(neighbour, open))
					cout <<"       " << neighbour << " не находится в Open"<<endl;
				else if(temp_g < g[(int)neighbour-97])
					cout <<"       Временное значение G("<<neighbour<<") < G("<<neighbour<<")"<<endl;
				from_vec[(int)neighbour-97] = curr;
				cout <<"        From("<<neighbour<<") = " << curr <<endl;
				g[(int)neighbour-97] = temp_g;
				cout<< "         G("<<neighbour<<") = " << temp_g << endl;
				f[(int)neighbour-97] = g[(int)neighbour-97] + h(neighbour, finish);
				cout<< "          F("<<neighbour<<") = G("<<neighbour<<") + H("<<neighbour<<", "<<finish<<") = "<<g[(int)neighbour-97]<<" + " << h(neighbour, finish) << " = " <<f[(int)neighbour-97]<<endl;
			}
			if(!IsIn(neighbour, open))
			{
				cout<< "           Добавляем "<<neighbour<<" в Оpen, потому что в Open нет такого элемента"<<endl;
				open.push_back(neighbour);
			}
			resWeight = f[(int)neighbour-97];
				
		}
		unclosed.erase(unclosed.begin(), unclosed.end());
		
		
		

	}
	cout << "           Нет пути из вершины "<<start<<" к вершине "<<finish<<endl;
	return 1;
	
}
