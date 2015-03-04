#include<iostream>
int main()
{
	using namespace std;
	float gl;
	cout<<"输入每百公里耗油量:";
	cin>>gl;
	cout<<"美国风格的耗油量为:"<<(3.875*62.14)/gl<<endl;
	return 0;
}
