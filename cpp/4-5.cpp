#include<iostream>
int main()
{
	using namespace std;
	struct candybar
	{
		string name;
		double weight;
		int kll;	
	};
	candybar snack={"Mocha",2.3,5.0};
	cout<<"Name:"<<snack.name<<endl<<"Weight:"<<snack.weight<<endl<<"Calorie:"<<snack.kll<<endl;
	return 0;

}
