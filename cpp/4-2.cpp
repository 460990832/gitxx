#include<iostream>
int main()
{
	using namespace std;
	string name,det;
	cout<<"Enter your name:"<<endl;
	getline(cin,name);
	cout<<"Enter your favorite dessert:"<<endl;
	getline(cin,det);
	cout<<"I have some delicious "<<det<<" for you, "<<name<<"."<<endl;
	return 0;
	
}
