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
	vector<vector<double>> a = { {2, 1, 1}, {1, 1, 0}, {0, 0.5, 2} };
	vector<vector<double>> simplex_table;
	vector <double> b = { 3, 6, 3 };
	vector <int> fict;
	double F_val = 0;
	vector<string> symb = { "Si0", "y1", "y2" , "y3" , "y4" , "y5" , "y6", "Ф" };

	transform(c.begin(), c.end(), c.begin(), [](int c) {return -c; });
	transform(b.begin(), b.end(), b.begin(), [](int b) {return -b; });
	for(auto &el : a)
		transform(el.begin(), el.end(), el.begin(), [](double el) {return -el; });
	for (int i = 0; i < b.size(); i++)
		a[i].push_back(b[i]);
	c.push_back(F_val);
	simplex_table.push_back(c);
	for (auto el : a)
		simplex_table.push_back(el);

	print_table(simplex_table, symb);
	while (!check(simplex_table))
		jordan_ex(simplex_table, symb);
}

void print_table(vector<vector<double>>& simplex_table, vector<string>& symb)
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
	vector<double> Si0_temp;
	vector<double> rats;
	for (int i = 0; i < simplex_table[0].size() - 1; i++)
		Si0_temp.push_back(simplex_table[0][i]);
	supp.second = distance(Si0_temp.begin(), min_element(Si0_temp.begin(), Si0_temp.end()));
	rats.push_back(INFINITY);
	for (int i = 1; i < simplex_table.size(); i++)
	{
		rats.push_back(simplex_table[i][supp.second]);
	}
	supp.first = distance(rats.begin(), min_element(rats.begin(), rats.end()));
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
			if (i != r && j != k)
				jordan_table[j][i] = simplex_table[j][i] -
				(simplex_table[k][i] * simplex_table[j][r]) / simplex_table[k][r];
		}
	}
	simplex_table = jordan_table;

	string temp_str = symb[r + 4];
	symb[r + 4] = symb[k];
	symb[k] = temp_str;
	print_table(simplex_table, symb);
}

bool check(vector<vector<double>>& simplex_table)
{
	for (auto el : simplex_table[0])
	{
		if (el < 0) { return false; }
	}
	return true;
}
