#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

void print_brute_force(vector<int>& value)
{
	cout << "x1 = " << value[0] << ", x2 = " <<
		value[1] << ", x3 = " << value[2] << "; F = " << value[3] << endl;
}

void transposition(vector<vector<double>>& A)
{
	vector<vector<double>> rez;
	vector<double>temp;
	for (int j = 0; j < A[0].size(); j++)
	{
		for (int i = 0; i < A.size(); i++)
			temp.push_back(A[i][j]);
		rez.push_back(temp);
		temp.clear();
	}
	A = rez;
}

vector<int> brute_force(vector<vector<double>> A, vector<double> &b, vector<double> &c)
{
	transposition(A);
	vector<vector<int>> values;
	values.push_back({ 0, 0, 0, 0 }); 
	int i = 0; 
	vector<int> counters = { 0, 0 };
	while (true)
	{
		if (A[0][0] * values[i][0] + A[0][1] * values[i][1] + A[0][2] * values[i][2] <= b[0] &&
			A[1][0] * values[i][0] + A[1][1] * values[i][1] + A[1][2] * values[i][2] <= b[1] &&
			A[2][0] * values[i][0] + A[2][1] * values[i][1] + A[2][2] * values[i][2] <= b[2])
		{
			values[i][3] = c[0] * values[i][0] + c[1] * values[i][1] + c[2] * values[i][2];
			vector<int> temp = values.back();
			temp[2]++;
			values.push_back(temp);
			counters[0]++;
			i++;
		}
		else if(counters[0] != 0)
		{
			values.back()[2] = 0;
			values.back()[1]++;
			counters[0] = 0;
			counters[1]++;
		}
		else if(counters[1] != 0)
		{
			values.back()[2] = values.back()[1] = 0;
			values.back()[0]++;
			counters[0] = counters[1] = 0;
		}
		else
		{
			values.pop_back();
			for(auto &el : values)
				print_brute_force(el);
			sort(values.begin(), values.end(), [](vector<int> &a, vector<int> &b)
			{
				return a[3] > b[3];
			});
			break;
		}
	}
	return values.front();
}

void print_table(vector<vector<double>>& simplex_table, vector<string>& symb, vector<pair<string, double>> &node)
{
	cout.setf(ios::fixed);
	cout.precision(2);
	int i = 0, j = 0;
	for (cout << "\t"; i < simplex_table.size(); i++)
		cout << symb[i] << "\t\t";
	cout << endl;
	for (; j < simplex_table[0].size(); j++)
	{
		cout << endl << symb[i] << "\t"; 
		node.push_back({ symb[i], simplex_table[0][j] }); i++;
		for (int k = 0; k < simplex_table.size(); k++)
		{
			cout << simplex_table[k][j] << "\t\t";
		}
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

void jordan_ex(vector<vector<double>>& simplex_table, vector<string>& symb, vector<pair<string, double>> &node)
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
	print_table(simplex_table, symb, node);
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

void run(vector<vector<double>> &a, vector<double> &b, vector<double> &c, vector<string> &symb)
{
	vector<vector<double>> simplex_table;
	vector<pair<string, double>> node;
	double F_val = 0;
	for (int i = 0; i < c.size(); i++)
		a[i].push_back(c[i]);
	b.push_back(F_val);
	simplex_table.push_back(b);
	for (auto el : a)
		simplex_table.push_back(el);

	print_table(simplex_table, symb, node);
	while (!check(simplex_table))
	{
		node.clear();
		jordan_ex(simplex_table, symb, node);
	}
	sort(node.begin(), node.end(), [](pair<string, double>& a, pair<string, double>& b)
	{
		return a.first < b.first;
	});
	for (auto& el : node)
	{
		if (el.first == "x1" || el.first == "x2" || el.first == "x3" || el.first == "F")
			cout << el.first << " = " << el.second << endl;
	}
	node.erase(node.begin());
	b.back() = 1; b.push_back(0); b.push_back(F_val);
}

int main()
{
	setlocale(LC_ALL, "ru");
	vector<vector<double>> a = { {2, 1, 0}, {1, 1, 0.5}, {1, 0, 2} };
	vector<double> b = { 3, 6, 3};
	vector<double> c = { 5, 3, 8 };
	vector<string> symb = { "Si0", "x1", "x2" , "x3" , "x4" , "x5" , "x6", "F" };
	vector<string> symb2 = { "Si0", "x1", "x2" , "x3" , "x4" , "x5" , "x6", "x7","F" };
	vector<string> symb3 = { "Si0", "x1", "x2" , "x3" , "x4" , "x5" , "x6", "x7", "x8","F" };
	vector<int>record = brute_force(a, b, c);
	cout << endl << "Рекорд: ";
	print_brute_force(record);
	run(a, b, c, symb);
	a = { {2, 1, 1, 0}, {1, 1, 0, 0}, {0, 0.5, 2, 1} };
	b = { 3, 6, 3, 1};
	c = { 5, 3, 8 };
	run(a, b, c, symb2);
	a = { {2, 1, 1, -1}, {1, 1, 0, 0}, {0, 0.5, 2, 0} };
	b = { 3, 6, 3, -1 };
	c = { 5, 3, 8 };
	run(a, b, c, symb2);
}
