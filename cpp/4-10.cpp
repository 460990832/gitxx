#include<iostream>
#include<array>
int main()
{
	using namespace std;
	cout<<"Enter your three times"<<endl;
	array<double,3> cj;
	for(int i=0;i<3;i++)
		cin>>cj[i];
	double s;
	s=cj[1]+cj[0]+cj[2];
	cout<<"Your average time is "<<s/3<<endl;
	return 0;
}		
