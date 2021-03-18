#pragma once
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
	bool is_dual;
public:
	Symplex(deque<deque<double>> a, deque <double> b, deque <double> c)
	{
		symb = { "x1", "x2" , "x3" , "x4" , "x5" , "x6", "x7", "x8", "x9", "b", "F" };
		this->a = a; this->b = b; this->c = c;
		symplex_table = a;
		for (int i = 0; i < a.size(); i++)
			symplex_table[i].push_back(b[i]);
		symplex_table.push_back(c);
		symplex_table[symplex_table.size() - 1].push_back(0);
		this->is_dual = false;
	}

	void transposition(deque<deque<double>>& A)
	{
		deque<deque<double>> rez;
		deque<double>temp;
		for (int j = 0; j < A[0].size(); j++)
		{
			for (int i = 0; i < A.size(); i++)
				temp.push_back(A[i][j]);
			rez.push_back(temp);
			temp.clear();
		}
		A = rez;
	}

	Symplex(deque<deque<double>> a, deque <double> b, deque <double> c, bool is_dual)
	{
		symb = { "y1", "y2" , "y3" , "y4" , "y5" , "y6", "y7", "y8", "y9", "b", "F" };
		this->a = a; this->b = b; this->c = c;
		symplex_table = a; transposition(symplex_table);
		for (int i = 0; i < symplex_table.size(); i++)
			symplex_table[i].push_back(c[i]);
		for (auto& el : symplex_table)
			transform(el.begin(), el.end(), el.begin(), [](double el) {return -el; });
		symplex_table.push_back(b);
		symplex_table[symplex_table.size() - 1].push_back(0);
		this->is_dual = true;
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
				cout << endl << symb[i - 1] << "\t"; i++;
			for (int k = 0; k < symplex_table[0].size(); k++)
				cout << symplex_table[j][k] << "\t\t";
			cout << endl;
		}
		if(is_dual)
			cout << endl << "==============================================================================" << endl;
		else
			cout << endl << "==============================================================================================" << endl;
	}

	bool check()
	{
		if (!is_dual)
		{
			for (auto& el : symplex_table.back())
			{
				if (el >= 0) { return false; }
			}
			return true;
		}
		else
		{
			for (int i=0; i<symplex_table.size(); i++)
			{
				if (symplex_table[i][symplex_table[i].size()-1] <= 0) 
				{ 
					return false;
				}
			}
			return true;
		}
	}

	pair<int, int> find_pos()
	{
		pair<int, int> supp;
		deque<double> rats;
		deque<double> temp;
		if (is_dual)
		{
			for (int i = 0; i < symplex_table.size() - 1; i++)
			{
				temp.push_back(symplex_table[i].back());
				rats.push_back(*min_element(symplex_table[i].begin(), symplex_table[i].end() - 1));
			}
			supp.first = distance(temp.begin(), min_element(temp.begin(), temp.end()));
			supp.second = distance(symplex_table[supp.first].begin(), min_element(symplex_table[supp.first].begin(), symplex_table[supp.first].end() - 1));
			temp.clear(); rats.clear();
		}
		else
		{
			supp.second = distance(symplex_table.back().begin(), max_element(symplex_table.back().begin(), symplex_table.back().end() - 1));
			for (int i = 0; i < symplex_table.front().size() - 1; i++)
			{
				if (symplex_table[i].back() / symplex_table[i][supp.second] <= 0)
					rats.push_back(INFINITY);
				else
					rats.push_back(symplex_table[i].back() / symplex_table[i][supp.first]);
			}
			supp.first = distance(rats.begin(), min_element(rats.begin(), rats.end()));
		}
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
		for (int i = 0; i < symplex_table.size(); i++)
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

	void inter_print()
	{
		if (is_dual)
		{
			cout << endl << "Оптимальное решение: " << endl;
			cout << "u1 = " << symplex_table[0].back() << " u2 = " << symplex_table[2].back() << endl;
			cout << "W = " << symplex_table.back().back() << " g = " << 1 / (symplex_table.back().back());
			cout << endl << "Оптимальные стратегии: " << endl;
			cout << "x1 = " << symplex_table[0].back()* 1 / (symplex_table.back().back()) <<
			" x2 = " << symplex_table[2].back()* 1 / (symplex_table.back().back()) <<  endl;
		}
		else
		{
			cout << endl << "Оптимальное решение: " << endl;
			cout << "v1 = " << symplex_table[0].back() << " v3 = " << symplex_table[1].back() << endl;
			cout << "Z = " << -symplex_table.back().back() << " H = " << -1 / (symplex_table.back().back());
			cout << endl << "Оптимальные стратегии: " << endl;
			cout << "x1 = " << symplex_table[0].back() * -1 / (symplex_table.back().back()) <<
			" x3 = " << symplex_table[1].back() * -1 / (symplex_table.back().back()) << endl;
		}
	}

	deque<deque<double>> get_symplex() { return symplex_table; }
};
