#include <iostream>
#include <vector>
#include <stack>
using namespace std;

bool found_way = false;

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

bool getTop(vector<char> tops, char top)
{
	for (int i = 0; i < tops.size(); i++)
    {
		if (tops[i] == top) return true;
    }
    return false;
}

int getIndex(vector<char> tops, char get_start)
{
	for (int i = 0; i < tops.size(); i++)
	{
		if (tops[i] == get_start) return i;
	}
}

void printStack(stack <char> stack)
{
	char st_top = stack.top();
	stack.pop();
	if (!stack.empty()) printStack(stack);
	cout << st_top;
}

void findWay(vector<char> tops, vector<vector<Path> > paths, int startIndex, char end, stack <char> stack)
{
	if (!paths[startIndex].empty())
    {
		char endLocal = paths[startIndex][0].getEnd();
		stack.push(endLocal);
		if (endLocal != end)
        {
			paths[startIndex].erase(paths[startIndex].begin());
			findWay(tops, paths, getIndex(tops, endLocal), end, stack);
			stack.pop();
			if (found_way == false) findWay(tops, paths, startIndex, end, stack);
		}
		else
        {
			printStack(stack);
			found_way = true;
		}
	}
}

void sortPaths(vector<vector<Path> > &paths)
{
	for (int i = 0; i < paths.size(); i++)
    {
	    if (1 < paths[i].size())
        {
			for (int j = 0; j < paths[i].size() - 1; j++)
            {
				for (int k = 0; k < paths[i].size(); k++)
                {
					if (paths[i][j].getWeight() > paths[i][j + 1].getWeight())
                    {
						Path change = paths[i][j];
						paths[i][j] = paths[i][j + 1];
						paths[i][j + 1] = change;
					}
				}
			}
	    }
    }
	paths.swap(paths);
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
		paths[getIndex(tops, get_start)].push_back(Path(get_start, get_end, get_weight));
	}
	stack <char> stack;
	stack.push(start);
	sortPaths(paths);
	findWay(tops, paths, getIndex(tops, start), end, stack);
	cout << endl;
	return 0;
}