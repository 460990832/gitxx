#include<iostream>
int main()
{
	using namespace std;
	long long wd,us;
	cout<<"Enter the world's population:";
	cin>>wd;
	cout<<"Enter the population of the US:";
	cin>>us;
	cout<<"The population of the US is "<<double(us)/wd*100;
	cout<<"% of the world population."<<endl;
	return 0;
}
