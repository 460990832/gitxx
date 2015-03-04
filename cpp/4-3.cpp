#include<iostream>
#include<cstring>
int main()
{
	using namespace std;
	char fn[20],ln[20],name[40];
	cout<<"Enter your fisrt name:"<<endl;
	cin.getline(fn,19);
	cout<<"Enter your last name:"<<endl;
	cin.getline(ln,19);
	strcat(ln,", ");
    //	cout<<ln<<endl;
	strcpy(name,ln);
	strcat(name,fn);
	cout<<"Here's the information in a single string:"<<name<<endl;
	return 0;
}
