
#include <stdio.h>//读入输出测试
#include <stdlib.h>
typedef int ElementType;
typedef struct Node *PtrToNode;
struct Node {
    ElementType Data;
    PtrToNode   Next;
};
typedef PtrToNode List;
List Read(); /* 细节在此不表 */
void Print( List L ); /* 细节在此不表；空链表将输出NULL */
List Merge( List L1, List L2 );
int main()
{
    List L1, L2, L;
    L1 = Read();
    L2 = Read();
    L = Merge(L1, L2);
    Print(L);
    Print(L1);
    Print(L2);
    return 0;
}
List Read()
{
	int n,i;
	List L0;
	L0=(List)malloc(sizeof(struct Node));
	L0->Next=NULL;
	scanf("%d",&n);
	if(n)
	{
		List r=L0;
		List temp0;
		for(i=0;i<n;i++){
			temp0=(List)malloc(sizeof(struct Node));
			scanf("%d",&(temp0->Data));
			r->Next=temp0;
			r=temp0;
		}
		r->Next=NULL;
	}
	return L0;
}
void Print(List L)
{
	if(L->Next)
	{
		List tempp;
		tempp=L;
		while(tempp->Next)
		{
			tempp=tempp->Next;
			printf("%d ",tempp->Data);
		}
		printf("\n");
	}
	else
	{
		printf("NULL\n");
	}
}
List Merge( List L1, List L2 )
{
    List L3= (List)malloc(sizeof(struct Node));
    List r=L3;
    L3->Next=NULL;
    List l1=L1->Next;
    List l2=L2->Next;
    while(l1!=NULL&&l2!=NULL)
    {
        if(l1->Data<l2->Data)
        {
            L3->Next=l1;
            l1=l1->Next;
            L3=L3->Next;
        }
        else
        {
            L3->Next=l2;
            l2=l2->Next;
            L3=L3->Next;
        }
    }
    if(l1!=NULL)
    {
        L3->Next=l1;
    }
    if(l2!=NULL)
    {
        L3->Next=l2;
    }
    L1->Next=NULL;
    L2->Next=NULL;
    return r;
}
