#include<iostream>
//#include<cstring>
int main()
{
	using namespace std;
	string ln,fn,name;	
	cout<<"Enter your fisrt name:"<<endl;
	getline(cin,fn);
	cout<<"Enter your last name:"<<endl;
	getline(cin,ln);
	name=ln+", "+fn;

	cout<<"Here's the information in a single string:"<<name<<endl;
	return 0;
}
