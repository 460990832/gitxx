#include<iostream>
int main()
{
	using namespace std;
	struct pisa
	{
		string name;
		double diameter;
		double weight;
	};
	pisa *ps =new pisa; 
	cout<<"Name:";
	cin>>ps->name;
	cout<<"Diameter:";
	cin>>ps->diameter;
	cout<<"Weight:";
	cin>>ps->weight;
	cout<<"Name: "<<ps->name<<endl<<"Diameter: "<<ps->diameter<<endl<<"Weight: "<<ps->weight<<endl;
	return 0;
			
	
}		
