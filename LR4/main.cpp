#include <iostream>
#include <vector>
#include <deque>
#include "symplex_table.h"

using namespace std;

int main()
{
	Symplex s_t({ {2, 1, 1}, {1, 1, 0}, {0, 0.5, 2} }, { 3, 6, 3 }, { 5, 3, 8 });
	s_t.print_table();
	while(!s_t.check())
		s_t.jordan_ex();
	s_t.add_str();
	while (!s_t.check())
		s_t.jordan_ex();
}
