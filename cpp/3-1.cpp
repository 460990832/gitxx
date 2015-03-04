#include<iostream>
int main()
{
	using namespace std;
	const int yz=12;
	int yc,c,ys;
	cout<<"Enter your height(inch)__\b\b";
	cin>>yc;
	c=yc/yz,ys=yc%yz;
	cout<<"You height is "<<c<<"ft "<<ys<<"inch\n";
	return 0;
}
