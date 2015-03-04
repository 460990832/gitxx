#include<iostream>
int main()
{
	using namespace std;
	int in,ft;
	float tz,BMI,sg;
	const float a=0.0254,b=12,c=2.2;
	cout<<"Enter your height in foot and inch\n";
	cout<<"foot:";
	cin>>ft;
	cout<<"inch:";
	cin>>in;
	cout<<"Enter your weight in pounds:";
	cin>>tz;
	sg=(ft*b+in)*a;
	BMI=(tz/c)/(sg*sg);
	cout<<"Your BMI is "<<BMI<<endl;
	return 0;
}
