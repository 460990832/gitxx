#include<iostream>
long zh(float g)
{
	return g*63240;
}
int main()
{
	using namespace std;
	float y;
	cout<<"Enter the number of light years:";
	cin>>y;
	cout<<y<<" light years = "<<zh(y)<<" astronomicl units"<<endl;
	return 0;
}
