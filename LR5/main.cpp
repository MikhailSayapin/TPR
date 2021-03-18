#include <iostream>
#include "symplex_table.h"

using namespace std;

struct Input_data
{
	deque<deque<double>> a = { {16, 17, 8, 15, 17}, {0, 3, 19, 8, 2}, {13, 19, 7, 15, 9}, { 11, 15, 2, 16, 2 } };
	deque <double> b = { -1, -1, -1, -1 };
	deque <double> c = { 1, 1, 1, 1, 1 };
};

void run(Symplex& s_t)
{
	s_t.print_table();
	while (!s_t.check())
		s_t.jordan_ex();
	s_t.inter_print();
}

int main()
{
	Input_data I_d;
	setlocale(LC_ALL, "ru");
	cout << "Найдём оптимальную смешанную стратегию игрока А:" << endl << endl;
	Symplex s_t1(I_d.a, I_d.b, I_d.c, true);
	run(s_t1);

	cout << endl << "Найдём оптимальную смешанную стратегию игрока Б : " << endl << endl;
	transform(I_d.b.begin(), I_d.b.end(), I_d.b.begin(), [](int b) {return -b; });
	Symplex s_t2(I_d.a, I_d.b, I_d.c);
	run(s_t2);
}
