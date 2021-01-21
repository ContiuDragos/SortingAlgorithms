#include <stdio.h>
#include "Profiler.h"

#define MAX_SIZE 5000
#define STEP_SIZE 100
#define NR_TESTS 5

Profiler p("medie");
void Swap(int *a, int*b)
{
    int aux=*a;
    *a=*b;
    *b=aux;
}

int BinarySearch(int v[], int element, int left, int right, int n)
{
    Operation opCompInsertion=p.createOperation("InsertionSort-comp",n);
    if(right<=left)
    {
        opCompInsertion.count();
        if(element>=v[left])
            return left+1;
        return left;
    }
    int mid=(left+right)/2;

    opCompInsertion.count();
    if(v[mid]==element)
        return mid+1;
    opCompInsertion.count();
    if(element<v[mid])
        return BinarySearch(v,element,left,mid-1,n);
    return BinarySearch(v,element,mid+1,right,n);
}
void BubbleSort(int v[], int n)
{
    Operation opCompBubble=p.createOperation("BubbleSort-comp",n);
    Operation opAtrBubble=p.createOperation("BubbleSort-atr",n);do{
        ok=1;
        for(i=1;i<n;i++)
        {
            opCompBubble.count();
            if (v[i - 1] > v[i]) {
                opAtrBubble.count(3);
                Swap(&v[i - 1], &v[i]);
                ok = 0;
            }
        }
    }while(ok==0);
    int ok=0, i=0;

}

void SelectionSort(int v[], int n)
{
    Operation opCompSelection=p.createOperation("SelectionSort-comp",n);
    Operation opAtrSelection=p.createOperation("SelectionSort-atr",n);

    int pozMin=-1;
    for(int i=0;i<n-1;i++)
    {
        pozMin=i;
        for(int j=i+1;j<n;j++)
        {
            opCompSelection.count();
            if(v[j]<v[pozMin])
                pozMin=j;
        }
        if(pozMin!=i)
        {
            opAtrSelection.count(3);
            Swap(&v[pozMin],&v[i]);
        }
    }
}

void InsertionSort(int v[], int n)
{
    Operation opAtrInsertion=p.createOperation("InsertionSort-atr",n);

    int aux=0, poz=-1, j=0;
    for(int i=1;i<n;i++)
    {
        aux=v[i];
        j=i-1;
        poz= BinarySearch(v,aux,0,j,n);
        while (j>=poz)
        {
            opAtrInsertion.count();
            v[j+1]=v[j];
            j--;
        }
        v[j+1]=aux;
        opAtrInsertion.count();
    }
}

void Show_vector(int v[], int n)
{
    for(int i=0;i<n;i++)
        printf("%d ",v[i]);
    printf("\n");
}

void demo()
{
    int v[]={ 7,2,8,5,9,1,6 };
    int n=sizeof(v)/ sizeof(v[0]);
    ///BubbleSort(v,n);
    ///SelectionSort(v,n);
    InsertionSort(v,n);
    Show_vector(v,n);
}
void perf(int order)
{
    int v[MAX_SIZE];
    int n;
    for(n=STEP_SIZE; n<=MAX_SIZE; n+=STEP_SIZE) {
        for(int i=1;i<=NR_TESTS;i++)
        {
            FillRandomArray(v, n,10,50000, false,order);
            BubbleSort(v,n);
            if (order < 2)
                FillRandomArray(v, n, 10, 50000, false, order);
            else
            {
                ///ca sa obtinem worst case pentru selection sort o sa duc primul element pe ultima pozitie din vectorul sortat crescator
                int aux = v[0];
                for (int j = 0; j < n - 1; j++)
                {
                    v[j] = v[j + 1];
                }
                v[n - 1] = aux;
            }
            SelectionSort(v,n);
            FillRandomArray(v,n,10,5000,false,order);
            InsertionSort(v,n);
        }
    }
    p.divideValues("BubbleSort-comp", NR_TESTS);
    p.divideValues("BubbleSort-atr", NR_TESTS);
    p.divideValues("SelectionSort-comp", NR_TESTS);
    p.divideValues("SelectionSort-atr", NR_TESTS);
    p.divideValues("InsertionSort-comp", NR_TESTS);
    p.divideValues("InsertionSort-atr", NR_TESTS);


    p.addSeries("BubbleSort-total", "BubbleSort-comp", "BubbleSort-atr");
    p.addSeries("SelectionSort-total", "SelectionSort-comp", "SelectionSort-atr");
    p.addSeries("InsertionSort-total", "InsertionSort-comp", "InsertionSort-atr");

    p.createGroup("Comp", "BubbleSort_comp", "SelectionSort_comp", "InsertionSort_comp");
    p.createGroup("Atr", "BubbleSort_atr", "SelectionSort_atr", "InsertionSort_atr");
    p.createGroup("Total", "BubbleSort_total", "SelectionSort_total", "InsertionSort_total");

}
int main() {
    demo();
    ///UNSORTED
    ///perf(0);

    ///ASCENDING
   /// p.reset("best");
    ///perf(1);

    ///DESCENDING
   /// p.reset("worst");
   /// perf(2);

   /// p.showReport();
    return 0;
}