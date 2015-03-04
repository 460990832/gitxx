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
	candybar snack[3]={{"Mocha",2.3,5.0},{"QKL",2.66,4.0},{"BZ",3.333,4}};
	for(int i=0;i<3;i++)
	cout<<"Name:"<<snack[i].name<<endl<<"Weight:"<<snack[i].weight<<endl<<"Calorie:"<<snack[i].kll<<endl;
	return 0;

}
