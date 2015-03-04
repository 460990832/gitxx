#include<iostream>
int main ()
{
	using namespace std;
	string fn,ln;
	int age;
	char gd;
	cout<<"What is your firet name? ";
	getline(cin,fn);
	cout<<"What is your last name? ";
	getline(cin,ln);
	cout<<"What letter grade do you deserve? ";
	cin>>gd;
	cout<<"What is your age? ";
	cin>>age;
	cout<<"Name "<<ln<<","<<fn<<endl;
	cout<<"Grade: "<<char(gd+1)<<endl;
	cout<<"Age: "<<age<<endl;
	return 0;	
}
