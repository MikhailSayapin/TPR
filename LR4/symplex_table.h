#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <math.h>

using namespace std;

class Symplex
{
private:
	deque<deque<double>> a; deque <double> b; deque <double> c;
	deque<string> symb;
	deque<deque<double>> symplex_table;
public:
	Symplex(deque<deque<double>> a, deque <double> b, deque <double> c)
	{
		symb = { "x1", "x2" , "x3" , "x4" , "x5" , "x6", "x7", "x8", "b", "F" };
		this->a = a; this->b = b; this->c = c;
		symplex_table = a;
		for (int i=0; i<a.size(); i++)
			symplex_table[i].push_back(b[i]);
		symplex_table.push_back(c);
		symplex_table[symplex_table.size() - 1].push_back(-15.75);
	}

	void print_table()
	{
		cout.setf(ios::fixed);
		cout.precision(2);
		int i = 0, j = 0;
		for (cout << "\t"; i < symplex_table[0].size(); i++)
		{
			if (i == symplex_table[0].size() - 1)
				cout << symb[symb.size() - 2];
			else
				cout << symb[i] << "\t\t";
		}
		cout << endl;
		for (; j < symplex_table.size(); j++)
		{
			if (j == symplex_table.size() - 1)
				cout << endl << symb[symb.size() - 1] << "\t";
			else
				cout << endl << symb[i-1] << "\t"; i++;
			for (int k = 0; k < symplex_table[0].size(); k++)
				cout << symplex_table[j][k] << "\t\t";
			cout << endl;
		}
		cout << endl << "=============================================================" << endl;
	}

	bool check()
	{
		for (auto &el : symplex_table.back())
		{
			if (el >= 0) { return false; }
		}
		return true;
	}

	pair<int, int> find_pos()
	{
		pair<int, int> supp;
		vector<double> rats;
		supp.second = distance(symplex_table.back().begin(), max_element(symplex_table.back().begin(), symplex_table.back().end()));
		for (int i = 0; i < symplex_table.size() - 1; i++)
		{
			if (symplex_table[i][symplex_table[i].size() - 1] / symplex_table[i][supp.second] <= 0)
				rats.push_back(INFINITY);
			else
				rats.push_back(symplex_table[i][symplex_table[i].size() - 1] / symplex_table[i][supp.second]);
		}
		supp.first = distance(rats.begin(), min_element(rats.begin(), rats.end()));
		return supp;
	}

	void jordan_ex()
	{
		deque<deque<double>> jordan_table = symplex_table;
		deque<double> temp;
		pair<int, int> supp = find_pos();
		int r = supp.first, k = supp.second;
		jordan_table[r][k] = 1 / symplex_table[r][k];
		for (int j = 0; j < symplex_table[0].size(); j++)
			if (j != k) { jordan_table[r][j] = symplex_table[r][j] / symplex_table[r][k]; }
		for (int i = 0; i < symplex_table/**/.size(); i++)
			if (i != r) { jordan_table[i][k] = -symplex_table[i][k] / symplex_table[r][k]; }
		for (int i = 0; i < symplex_table.size(); i++)
		{
			for (int j = 0; j < symplex_table[0].size(); j++)
			{
				if (i != r && j != k)
					jordan_table[i][j] = symplex_table[i][j] -
					(symplex_table[i][k] * symplex_table[r][j]) / symplex_table[r][k];
			}
		}
		symplex_table = jordan_table;

		swap(symb[r + symplex_table[0].size() - 1], symb[k]);
		print_table();
	}

	void add_str()
	{
		double integer;
		deque<double> temp_f = symplex_table.back();
		deque<double> temp_str; temp_str.assign(symplex_table[0].size(), 0);
		cout << "x1 = " << symplex_table[0][3] << " x3 = " << symplex_table[2][3] << endl;
		cout << "F = " << -symplex_table[symplex_table.size() - 1][symplex_table[0].size() - 1] << endl;

		for (int i=0; i<symplex_table[0].size(); i++)
		{
			if (symplex_table[0][i] < 0)
				temp_str[i] = -(symplex_table[0][i] - floor(symplex_table[0][i]));
			else
				temp_str[i] = -(modf(symplex_table[0][i], &integer));
		}

		symplex_table.back() = temp_str;
		symplex_table.push_back(c);
		/*for (int i = 0; i < symplex_table.back().size(); i++)
			if (i != 1) symplex_table.back()[i] = 0;*/
		symplex_table.back().push_back(0);
		print_table();
	}

	void inter_print()
	{
		cout << "x1 = " << symplex_table[3][1] << " x3 = " << symplex_table[3][1] << endl;
		cout << "x2 = " << symplex_table[0][3] << endl;
		cout << "F = " << -symplex_table[symplex_table.size() - 1][symplex_table[0].size() - 1] << endl;

	}

	deque<deque<double>> get_symplex() { return symplex_table; }
};
