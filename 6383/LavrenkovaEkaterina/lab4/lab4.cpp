#include <iostream>
# include <string>
# include <vector>

using namespace std;


void KMP(string pattern, string text, vector<int> &indexes)
{
	int pl = pattern.length();
	int tl = text.length();
	string str = pattern + '#' + text;
	vector<int> p(str.length());
	p[0] = 0;
	int k = 0;
	for (int i = 1; i<str.length(); i++)
	{
		k = p[i - 1];
		while ((k>0) && (str[i] != str[k]))
			k = p[k - 1];
		if (str[i] == str[k])
			k++;
		p[i] = k;
	}
	for (int i = 0; i<tl; i++)
		if (p[pl + i + 1] == pl)
			indexes.push_back(i + 1 - pl);

}

int main()
{
	string pattern, text;
	getline(cin, pattern);
	getline(cin, text);
	vector<int> indexes;
	KMP(pattern, text, indexes);
	if (indexes.size() == 0)
	{
		cout << -1 << endl;; return 0;
	}
	for (int i = 0; i<indexes.size(); i++)
	{
		cout << indexes[i];
		if (i != indexes.size() - 1)
			cout << ',';
	}
	cout << endl;
	system("pause>nul");
}

