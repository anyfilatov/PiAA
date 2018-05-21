#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <utility>
#include <vector>

using namespace std;

//вывод графа в консоль в виде списка смежности
 void print_graph(const map<string, multimap<double, string>>& graph) {
  for (const auto &iter1: graph ) {
    cout << iter1.first << ' ';
    for (const auto &iter2: iter1.second) {
      cout << "(" << iter2.second << ", "<< iter2.first << ") " ;
    }
    cout << endl;
  }
}
//-----------------------------------------------------------------
//считывание графа
void read_graph(map<string, multimap<double, string>>& graph) {
  cout << "Вводите: <первая вершина> <вторая вершина> <вес>" <<endl;
  cout << "нажмите <Ctrl> + <D> чтобы завершить ввод" << endl;
  string first_vertex, second_vertex;
  double weight;
  pair<double, string> incident_vertex;
  while (cin >> first_vertex) {
    cin >> second_vertex >> weight;
    incident_vertex.first = weight;
    incident_vertex.second = second_vertex;
    graph[first_vertex].insert(incident_vertex);
    graph[second_vertex];
  }
}

void read_graph(ifstream& fin, map<string, multimap<double, string>>& graph) {
  string first_vertex, second_vertex;
  double weight;
  pair<double, string> incident_vertex;
  while (fin >> first_vertex) {
    fin >> second_vertex >> weight;
    incident_vertex.first = weight;
    incident_vertex.second = second_vertex;
    graph[first_vertex].insert(incident_vertex);
    graph[second_vertex];
  }
}

//-----------------------------------------------------------------
//удаление из графа ребер, ведущих в данную вершину, чтобы избежать циклов в пути
void exclude_incident_edges(map<string, multimap<double, string>>& graph, string deleting_vertex) {
 // cout << "исключаем вершины чтобы избежать циклов на пути" << endl;
 //cout << "исключаем края: ";
for (auto iter1 = graph.begin(); iter1 != graph.end(); iter1++) {  //для каждой вершины
for (auto iter2 = iter1->second.begin(); iter2 != iter1->second.end(); iter2++) { //просматриваем смежные ей вершины
    if (iter2->second == deleting_vertex) {   //если ребро ведет в необходимую вершину
      cout << "(" << iter1->first << ", " << iter2->second << ", " << iter2-> first << ")";
      iter1->second.erase(iter2);            //то убираем ребро из графа
    }
  }
}
cout << endl;
}

//-----------------------------------------------------------------
//поиск минимального пути в графе
void find_minimal_path(map<string, multimap<double, string>> &graph, vector<string> &result, string current_vertex, const string finish) {
cout << "Текущая вершина: " << current_vertex << endl;

result.push_back(current_vertex); //добавляем текущую вершину в результат
cout << "Текущий результат: ";
for(const auto &elem: result) {
  cout << elem;
}
cout << endl;

exclude_incident_edges(graph, current_vertex);
cout << "Текущее состояние:" << endl;
print_graph(graph);
if (current_vertex == finish) {                     //если текущая вершина конечная
  cout << "Эта вершина конечная" << endl;       //то конец алгоритма
  cout << "Конец работы алгоритма" << endl;
	return;
}
else if (graph[current_vertex].empty()) {           //если у текущей вершины нет смежных вершин
  cout << "Нету смежных вершин" << endl;  //то возвращаемся на предыдущий этап рекурсии
  cout << "Возвращаемся обратно и иещм другой путь" << endl;
	return;
}
else {
	string next_vertex;

	   for (auto elem: graph[current_vertex]) { //просматриваем смежные вершины для данной
		 next_vertex = elem.second; //вершины отсортированы в порядке возрастания веса, поэтому просматриваем просто в цикле for each
     cout << "Инцидентная вершина с минимальным весом -  (" << next_vertex << ", " << elem.first << ")" << endl;
     cout << "----------------------------------------" << endl;
     cout <<  "\033[1;32m"  << "Рекурсивный вызов" << "\033[0m" << endl;
find_minimal_path(graph, result, next_vertex, finish); 
//рекурсивный вызов для следующей вершины, образующей с текущей ребро минимального веса
if (result.back() == finish) {  
//если при возврате из рекурсии последняя вершина в результате - конечная
			 return;  //то конец алгоритма, возвращаемся из рекурсии
		 }
		 else {  //иначе убираем из результата последнюю добавленную вершину
		result.pop_back();//и продолжаем просматривать смежные вершины для текущей
      		 }
	 }
 }
}

//-----------------------------------------------------------------
void menu() {
cout << "Выберите действие:" << endl;
cout << "0 - Входные данные из консоли" << endl;
cout << "1 - Входные данные из файла" << endl;
}
//-----------------------------------------------------------------
int main() {

  cout << "Жадный алгоритм." << endl;
  cout << "Поиск минимального пути между вершинами." << endl;
  map<string, multimap<double, string>> graph;
  string start, finish;

  menu();
  int cmd;
  cout << "Ваш выбор: ";
  cin >> cmd;

  if(cmd == 0) {
    cout << "Первая вершина: ";
    cin >> start;
    cout << "Вторая вершина: ";
    cin >> finish;
    read_graph(graph);
  }
  else if (cmd == 1) {
    ifstream fin("input.txt");
    fin >> start >> finish;
    read_graph(fin, graph);
  }
  else {
    cout << "Неправильный ввод!" << endl;
}
  cout << "Начальное состояние графа: " << endl;
  print_graph(graph);
  cout << endl;

  if (start == finish) {
    cout << "Начальная и конечная вершина одна и та же" << endl;
  }
  else if (graph.find(start) == graph.end()) {
    cout << "Первая вершина не существует" << endl;
  }
  else if (graph.find(finish) == graph.end()) {
    cout << "Вторая вершина не существует" << endl;
  }
  else {
    vector<string> result_path;

    
    find_minimal_path(graph, result_path, start, finish);
   
    cout << "----------------------------------------" << endl;
    cout <<  "\033[1;32m"  << "result" << "\033[0m" << endl;
    cout << "Минимальный путь между вершиной " << start << " и вершиной " << finish << ": ";
    for(const auto &elem: result_path) {
      cout << elem;
    }
    cout << endl;
  }
  return 0;
}

