#include<iostream>
int main()
{
	using namespace std;
	const float zh=60;
	int d,m,s;
	float f;
	cout<<"Enter a latitude in degrees,minutes,and seconds:\n";
	cout<<"First,enter the degrees: ";
	cin>>d;
	cout<<"Next,enter the minutes of arc: ";
	cin>>m;
	cout<<"Finally,enter the seconds of arc: ";
	cin>>s;
	cout<<d<<" degrees,"<<m<<" minutes,"<<s<<" seconds = ";
	cout<<d+m/zh+s/zh/zh<<" degrees"<<endl;
	return 0;

}
