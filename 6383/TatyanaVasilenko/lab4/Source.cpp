#include <iostream>
#include <fstream>
#include <string>


class STR
{
private:
	std::string str;
	void KMP(const std::string &str, int *M);


public:
	STR(std::string &str);

	void Q_1(const std::string &temp);
	int  Q_2(const std::string &temp);
};


STR::STR(std::string &str) : str(str)
{}


void STR::KMP(const std::string &str, int *M)
{
	int m = str.length();

	for (int i = 1; i < m; i++)
	{
		int j = M[i - 1];

		while ((j > 0) && (str[i] != str[j]))
		{
			j = M[j - 1];
		}

		if (str[i] == str[j])
			j++;

		M[i] = j;
	}
}



void STR::Q_1(const std::string &temp)
{
	std::string s = temp;
	s += (char)0 + str;

	int *M = new int[s.length()]();

	KMP(s, M);

	bool f = false;
	for (int i = temp.length(); i < s.length(); i++)
	{
		if (M[i] == temp.length())
		{
			if (f)
				std::cout << ",";

			std::cout << i - temp.length() * 2;

			if (!f) f++;
		}
	}
	delete[] M;
	if (!f) std::cout << -1;
}


int STR::Q_2(const std::string &temp)
{
	int len = temp.length();

	if (len == str.length())
	{
		std::string s = temp;
		s += (char)0 + str;

		int d_len = len * 2 + 1;

		int *M = new int[d_len]();

		KMP(s, M);


		// Если строки совпали
		if (M[d_len - 1] == len)
		{
			delete[] M;
			return 0;
		}

		// Флаг "цикличности"
		bool f = true;

		// Сравниваем префикс строки и суффикс шаблона
		for (int i = 0; i < len - M[d_len - 1]; i++)
		{
			if (str[i] != temp[M[d_len - 1] + i])
			{
				f = false;
				break;
			}
		}

		// Если строка и шаблон "одинаковы"
		if (f)
		{
			int ret = (M[len - 1] >= len / 2 && !(len % (len - M[len - 1])) && !(M[len - 1] % (len - M[len - 1]))) 
				? len / (len - M[len - 1]) - (len - M[d_len - 1]) 
				: M[d_len - 1];
			
			delete[] M;

			return ret;
		}
	}
	return -1;
}

int main()
{
	system("color 0a");

	std::string templ;
	std::string   str;

	std::ifstream fin("IN.txt");

	std::getline(fin, templ);
	std::getline(fin,   str);

	STR qw(str);
	//qw.Q_1(templ);
	std::cout << qw.Q_2(templ);

	system("pause");
	return 0;
}