#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstring>
#define alph 52

using namespace std;

struct Bohr {
	int next_vrtx[alph];//íîìåð âåðøèíû, â êîòîðóþ ìû ïðèäåì ïî ñèìâîëó ñ íîìåðîì i â àëôàâèòå
	vector<int> pattern_num;//íîìåð ñòðîêè-îáðàçöà, îáîçíà÷àåìîãî ýòîé âåðøèíîé
	bool flag;//áèò, óêàçûâàþùèé íà òî, ÿâëÿåòñÿ ëè íàøà âåðøèíà èñõîäíîé ñòðîêîé
	int suff_link;//ñóôôèêñíàÿ ññûëêà
	int auto_move[alph];//çàïîìèíàíèå ïåðåõîäà àâòîìàòà
	int par; //âåðøèíà-îòåö â äåðåâå
	char symb; //ñèìâîë íà ðåáðå îò par ê ýòîé âåðøèíå

};


vector<Bohr> bohr;

/*Ôóíêöèè ñîçäàíèÿ íîâîé âåðøèíû è èíèöèàëèçàöèè áîðà*/

Bohr make_bohr(int p, char c) {//ïåðåäàåì íîìåð îòöà è ñèìâîë íà ðåáðå â áîðå
	Bohr v;
	memset(v.next_vrtx, 255, sizeof(v.next_vrtx));//çàïîëíÿåò v.next_vrtx áàéòîâ áëîêà ïàìÿòè, ÷åðåç óêàçàòåëü v.next_vrtx
	v.flag = false;
	v.suff_link = -1;
	memset(v.auto_move, 255, sizeof(v.auto_move));
	v.par = p;
	v.symb = c;
	return v;
}

void init_bohr() {
	bohr.push_back(make_bohr(0, 255));//äîáàâëÿåì åäèíñòâåííóþ âåðøèíó - êîðåíü
}

//Ïðîöåäóðà äîáàâëåíèå ñòðîêè-îáðàçöà â áîð
void add_pattern_to_bohr(const string& s, vector<string>& pattern) {
	int num = 0; //íà÷èíàåì ñ êîðíÿ
	for (int i = 0; i<s.length(); i++) {
		char ch = s[i] - 'A'; //ïîëó÷àåì íîìåð â àëôàâèòå
		if (bohr[num].next_vrtx[ch] == -1) { //-1 - ïðèçíàê îòñóòñòâèÿ ðåáðà
			bohr.push_back(make_bohr(num, ch));
			bohr[num].next_vrtx[ch] = bohr.size() - 1;
		}
		num = bohr[num].next_vrtx[ch];
	}
	bohr[num].flag = true;
	pattern.push_back(s);
	bohr[num].pattern_num.push_back(pattern.size() - 1);
}

int get_auto_move(int v, char ch);

int get_suff_link(int v) {
	if (bohr[v].suff_link == -1) //åñëè åùå íå ñ÷èòàëè
		if (v == 0 || bohr[v].par == 0) //åñëè v - êîðåíü èëè ïðåäîê v - êîðåíü
			bohr[v].suff_link = 0;
		else
			bohr[v].suff_link = get_auto_move(get_suff_link(bohr[v].par), bohr[v].symb);
	return bohr[v].suff_link;
}

int get_auto_move(int v, char ch) {
	if (bohr[v].auto_move[ch] == -1)
		if (bohr[v].next_vrtx[ch] != -1)
			bohr[v].auto_move[ch] = bohr[v].next_vrtx[ch];
		else
			if (v == 0)
				bohr[v].auto_move[ch] = 0;
			else
				bohr[v].auto_move[ch] = get_auto_move(get_suff_link(v), ch);
	return bohr[v].auto_move[ch];
}

void check(int v, int i, vector<int>& count, const vector<int>& lenght) {
	for (int u = v; u != 0; u = get_suff_link(u)) {
		if (bohr[u].flag) {
			for (const auto& it : bohr[u].pattern_num) {
				if (i - lenght[it]<count.size()) {
					count[i - lenght[it]]++;
				}
			}

		}
	}
}

void find_all_pos(const string& s, vector<int>& count, const vector<int>& lenght) {
	int u = 0;
	for (size_t i = 0; i < s.length(); i++) {
		u = get_auto_move(u, s[i] - 'A');
		check(u, i + 1, count, lenght);
	}
}

vector<int> split_pattern(stringstream& str_pat, char joker, vector<string>& pattern) {
	vector<int> lenght;
	int len = 0;
	string tmp;
	while (getline(str_pat, tmp, joker)) {
		if (tmp.size()>0) {
			len += tmp.size();
			lenght.push_back(len);
			add_pattern_to_bohr(tmp, pattern);
		}
		len++;
	}
	return lenght;
}

void print(const vector<int>& count, int text_size, int pattern_size) {
	for (int i = 0; i<text_size; i++) {
		if (count[i] == pattern_size)
			cout << i + 1 << "\n";
	}
}

int main()
{
	init_bohr();
	string text;
	string pattern;
	char joker;
	cin >> text >> pattern >> joker;
	stringstream str_pat(pattern);
	vector<int> count(text.size(), 0);
	vector<string> patt;
	vector<int> lenght = split_pattern(str_pat, joker, patt);
	find_all_pos(text, count, lenght);
	print(count, text.size(), patt.size());
	return 0;
}