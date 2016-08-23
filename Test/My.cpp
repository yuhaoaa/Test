#include<iostream>
#include<cstdlib>
#include<string>
#include<fstream>
using namespace std;

int main()
{
	ifstream in;
	string s;
	while (getline(in, s))
		cout << s << endl;
	system("pause");
	return 0;
}