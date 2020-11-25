#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <cmath>

using namespace std;

void print_table(vector<vector<double>>& simplex_table, vector<string>& symb);
pair<int, int> find_pos(vector<vector<double>>& simplex_table);
void jordan_ex(vector<vector<double>>& simplex_table, vector<string>& symb);
bool check(vector<vector<double>>& simplex_table);

int main()
{
	setlocale(LC_ALL, "ru");
	vector <double> c = { 5, 3, 8 };
	vector<vector<double>> a = { {2, 1, 0}, {1, 1, 0.5}, {1, 0, 2} };
	vector<vector<double>> simplex_table;
	vector <double> b = { 3, 6, 3 };
	vector <int> fict;
	double F_val = 0;
	vector<string> symb = { "Si0", "x1", "x2" , "x3" , "x4" , "x5" , "x6", "F" };

	/*cout << "Исходная задача: с: [";
	for (auto el : c) { cout << el << " "; }
	cout << "]" << endl << "A: " << endl;
	for (int i = 0; i < a[0].size(); i++)
	{
		cout << "[ ";
		for (int j = 0; j < a.size(); j++)
			cout << a[j][i] << " ";
		cout << "]" << endl;
	}
	cout << "b: [";
	for (auto el : b) { cout << el << " "; }
	cout << "]" << endl << "=============================================================" << endl;*/
	for (int i = 0; i < c.size(); i++)
		a[i].push_back(c[i]);
	b.push_back(F_val);
	simplex_table.push_back(b);
	for (auto el : a)
		simplex_table.push_back(el);

	print_table(simplex_table, symb);
	while (!check(simplex_table))
		jordan_ex(simplex_table, symb);
}

void print_table(vector<vector<double>>& simplex_table, vector<string> &symb)
{
	cout.setf(ios::fixed);
	cout.precision(2);
	int i = 0, j = 0;
	for (cout << "\t"; i < simplex_table.size(); i++)
		cout << symb[i] << "\t\t";
	cout << endl;
	for (; j < simplex_table[0].size(); j++)
	{
		cout << endl << symb[i] << "\t"; i++;
		for (int k = 0; k < simplex_table.size(); k++)
			cout << simplex_table[k][j] << "\t\t";
		cout << endl;
	}
	cout << endl << "=============================================================" << endl;
}

pair<int, int> find_pos(vector<vector<double>>& simplex_table)
{
	pair<int, int> supp;
	vector<double> F_temp;
	vector<double> rats;
	for (int i = 0; i < simplex_table.size(); i++)
		F_temp.push_back(simplex_table[i][simplex_table[0].size() - 1]);
	supp.first = distance(F_temp.begin(), max_element(F_temp.begin(), F_temp.end()));
	for (int i = 0; i < simplex_table[0].size() - 1; i++)
	{
		if (simplex_table[0][i] / simplex_table[supp.first][i] <= 0)
			rats.push_back(INFINITY);
		else
			rats.push_back(simplex_table[0][i] / simplex_table[supp.first][i]);
	}
	supp.second = distance(rats.begin(), min_element(rats.begin(), rats.end()));
	return supp;
}

void jordan_ex(vector<vector<double>>& simplex_table, vector<string>& symb)
{
	vector<vector<double>> jordan_table = simplex_table;
	vector<double> temp;
	pair<int, int> supp = find_pos(simplex_table);
	int k = supp.first, r = supp.second;
	jordan_table[k][r] = 1 / simplex_table[k][r];
	for (int j = 0; j < simplex_table.size(); j++)
		if (j != k) { jordan_table[j][r] = simplex_table[j][r] / simplex_table[k][r]; }
	for (int i = 0; i < simplex_table[0].size(); i++)
		if (i != r) { jordan_table[k][i] = -simplex_table[k][i] / simplex_table[k][r]; }
	for (int i = 0; i < simplex_table[0].size(); i++)
	{
		for (int j = 0; j < simplex_table.size(); j++)
		{
			if(i != r && j != k)
				jordan_table[j][i] = simplex_table[j][i] -
				(simplex_table[k][i] * simplex_table[j][r]) / simplex_table[k][r];
		}
	}
	simplex_table = jordan_table;
	
	string temp_str = symb[r+4];
	symb[r + 4] = symb[k];
	symb[k] = temp_str;
	print_table(simplex_table, symb);
}

bool check(vector<vector<double>>& simplex_table)
{
	vector<double> F_temp;
	for (int i = 0; i < simplex_table.size(); i++)
		F_temp.push_back(simplex_table[i][simplex_table[0].size() - 1]);
	for (auto el : F_temp)
	{
		if (el > 0) { return false; }
	}
	return true;
}
