#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

class Path
{
public:
	char start;
	char getStart() {return start;}

	char end;
	char getEnd() {return end;}
	
	double weight;
	double getWeight() {return weight;}
	
	Path(char start, char end, double weight) : start(start), end(end), weight(weight) {}
};

struct TopForList
{
	char top;
	double priority;
	TopForList(char top, double priority) : top(top), priority(priority) {}
};

class List
{
	vector<TopForList> priorities;
public:
	TopForList top() {return priorities[0];}
	bool empty() {return priorities.empty();}

	void push(TopForList p)
	{
		if (!priorities.empty())
		{
			for (unsigned int i = 0; i < priorities.size(); i++)
			{
				if (priorities[i].priority > p.priority)
				{
					priorities.insert(priorities.begin() + i, p);
					return;
				}
			}
		}
		priorities.push_back(p);
	}

	void pop(char top)
	{
		for (unsigned int i = 0; i < priorities.size(); i++)
		{
			if (priorities[i].top == top)
			{
				priorities.erase(priorities.begin() + i);
				return;
			}
		}
		priorities.erase(priorities.begin());
	}
};

bool getTop(vector<char> tops, char top)
{
	for (unsigned  int i = 0; i < tops.size(); i++)
	{
		if (tops[i] == top) return true;
	}
	return false;
}

int getIndex(vector<char> tops, char top)
{
	for (unsigned  int i = 0; i < tops.size(); i++)
	{
		if (tops[i] == top) return i;
	}
	return 1;
}

void printWay(vector<char> from, vector<char> to, char start, char end)
{
	if (end != start) printWay(from, to, start, from[getIndex(to, end)]);
	cout << end;
}

void findWay(vector<char> tops, vector<vector<Path> > paths, char start, char end)
{
	vector<char> to;
	vector<char> from;
	vector<double> allWaight;

	to.push_back(start);
	from.push_back(0);
	allWaight.push_back(0);
	
	List priorities;
	priorities.push(TopForList(start, 0));

	while (!priorities.empty())
	{
		TopForList PriorityNow = priorities.top();
		int nowPos = getIndex(tops, PriorityNow.top);
		for (unsigned  int i = 0; i < paths[nowPos].size(); i++)
		{
			Path nowPath = paths[nowPos][i];
			double weightNow = nowPath.getWeight() +  allWaight.at(getIndex(to, PriorityNow.top));
			if (getTop(to, nowPath.getEnd()))
			{
				if (weightNow < allWaight.at(getIndex(to, nowPath.getEnd())))
				{
					allWaight[getIndex(to, nowPath.getEnd())] = weightNow;
					from[getIndex(to, nowPath.getEnd())] = PriorityNow.top;
				}
			}
			else
			{
				to.push_back(nowPath.getEnd());
				from.push_back(PriorityNow.top);
				allWaight.push_back(weightNow);
			}
			double priority = weightNow + abs(end - nowPath.getEnd());
			priorities.push(TopForList(nowPath.getEnd(), priority));
		}
		priorities.pop(PriorityNow.top);
		if (PriorityNow.top == end)
		{
			printWay(from, to, start, PriorityNow.top);
			break;
		}
	}
}

int main()
{
	char start;
	char end;
	char get_start;
	char get_end;
	
	double get_weight;

	vector<char> tops;
	vector<vector<Path> > paths;

	cin >> start >> end;
	while (cin >> get_start && cin >> get_end && cin >> get_weight)
	{
		if (!getTop(tops, get_start))
		{
			tops.push_back(get_start);
			vector<Path> new_path;
			paths.push_back(new_path);
		}
		if (!getTop(tops, get_end))
		{
			tops.push_back(get_end);
			vector<Path> new_path;
			paths.push_back(new_path);
		}
		Path edge(get_start, get_end, get_weight);
		paths[getIndex(tops, get_start)].push_back(edge);
	}

	findWay(tops, paths, start, end);
	cout << endl;
	return 0;
}