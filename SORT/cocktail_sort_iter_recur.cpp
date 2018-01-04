

#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <ctime>

using std::cout;

void sortIter(int A[],int n,int& counter);
void sortRecur(int A[],int low,int high,int& counter);
void swap(int* &A,int i,int j);
void printA(int A[],int n);
void timerIterSort(int nElement);
void timerRecurSort(int nElement);
int main(){
	cout<<"cocktail sort time comparison \n";

	srand (time(NULL));
	// 1000 element of random number array sort iteratively
	timerIterSort(1000);
 	// 2000 element of random number array sort iteratively
	timerIterSort(2000);
	// 4000 element of random number array sort iteratively
	timerIterSort(4000);

/*	timerIterSort(8000);
	timerIterSort(16000);

	timerIterSort(32000);
*/
  // 1000 element of random number array sort recursively
	timerRecurSort(1000);
  // 2000 element of random number array sort recursively
	timerRecurSort(2000);
	// 4000 element of random number array sort recursively
	timerRecurSort(4000);
/*	timerRecurSort(8000);
		timerRecurSort(16000);
		timerRecurSort(32000);
*/



	return 0;
}

//=============cocktail sort=======================
//iterative sort
void sortIter(int A[], int nElement,int& counter){

	int low = 0;
	int high = nElement - 1;

	while(low < high){
		for(int i = low;i < high;i++){
			//put the largest element at the end
			if(A[i] > A[i+1])
				swap(A,i,i+1);
			counter++;
		}
		//A[high] is the largest element
		//compare from the high-1 to low elenent
		high--;

		for(int j = high;j > low;j--){
			//put the smallest element at the beginning
			if(A[j-1] > A[j])
				swap(A,j-1,j);
			counter++;
		}
		//A[low] is tht smallest element
		//compare from low+1 to high element
	}

}
//recursive sort
void sortRecur(int A[],int low, int high,int& counter){
	if( low > high) return;
	for(int i = low;i < high;i++){
		//put the largest element at the end
		if(A[i] > A[i+1])
			swap(A,i,i+1);
		counter++;
	}
	//A[high] is the largest element
	//compare from the high-1 to low elenent
	high--;

	for(int j = high;j > low;j--){
		//put the smallest element at the beginning
		if(A[j-1] > A[j])
			swap(A,j-1,j);
		counter++;
	}
	//A[low] is tht smallest element
	//compare from low+1 to high element
	low++;
	sortRecur(A,low,high,counter);

}

//swap i, j element of A[]
void swap(int* &A,int i, int j){
	int tmp;
	tmp =A[i];
	A[i]=A[j];
	A[j]=tmp;

}
//print
void printA(int A[],int n){
	cout<<"\n";
	for(int i=0;i<n;i++){
		cout<<A[i]<<" ";
		if((i+1)%7==0)
			cout<<"\n";
	}
	cout<<"\n";
}
//count comparison and time iteratively
void timerIterSort(int nElement){

		int B[nElement];
		double sum=0;
		int counter=0;
		time_t start;
		time_t stop;
		for(int j=0;j<10;j++){
			for(int i=0;i<nElement;i++)
					  B[i]= rand() % RAND_MAX + 1;

			start =clock();

			sortIter(B,nElement,counter);

			stop = clock();

			sum +=double(stop-start);
		}
		cout<<"Iteratively comparison : "<<counter/10<<"\n";
		cout<<"Iteratively sorting time of "<<nElement<<" items average : "<<sum/10<<"\n";
}


//count comparison and time recursively
void timerRecurSort(int nElement){
	int C[nElement];
	double sum=0;
	int counter=0;
	time_t start;
	time_t stop;
	for(int j=0;j<10;j++){
		for(int i=0;i<nElement;i++)
				  C[i]= rand() % RAND_MAX + 1;

		start =clock();
		sortRecur(C,0,nElement-1,counter);

		stop = clock();

		sum +=double(stop-start);
	}

	cout<<"Recursively comparison : "<<counter/10<<"\n";
	cout<<"Recursively sorting time "<<nElement<<" items average : "<<sum/10<<"\n";
}
