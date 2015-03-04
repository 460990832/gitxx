#include<iostream>
int  zh(int c);
int main()
{
	using namespace std;
	int c;
	cout<<"Please enter a Celsius value:";
	cin>>c;	
	cout<<c<<" degrees Celsius is "<<zh(c)<<" degrees Fahreenheit."<<endl;
	return 0; 
}
int  zh(int c)
{
return 1.8*c+32.0;
}
