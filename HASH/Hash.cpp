

#include <iostream>
#include <stdlib.h>
using std::cout;

class Hash {
	public:
		Hash();
		int getKey();
		int h(int x);
		int h2(int x);
    //find empty spot with linear probing
		int searchEmptyL(int x);
    //find empty spot with quadratic probing
		int searchEmptyQ(int x);
    //find empty spot with double hash probing
		int searchEmptyDH(int x);
		void insertL(int x);
		void insertQ(int x);
		void insertDH(int x);
		int getComp();
		void resetComp();
		~Hash();
	private:
		static const int INITIAL_CAPACITY = 1999;
		static const int BIG_NUMBER = 1073741824;  //2^30
		int* T;
		int capacity; //table size
		int comparison;
};


Hash::Hash(){
	capacity =INITIAL_CAPACITY;
	comparison=0;
	T=new int[capacity];
  //initialze with -1 as empty slot
	for(int i=0;i<capacity;i++)
		T[i]=-1;
}
// get x
int Hash::getKey(){
	return rand() % BIG_NUMBER;
}
//get hashed value
int Hash::h(int x){
	if(x<0)
		x *=-1;

	return (x % capacity);
}

//for double hash
int Hash::h2(int x){
	if(x<0)
		x *=-1;

	return (7 - x % 7);
}

//Double Hashing probing
int Hash::searchEmptyDH(int x){
	int probe = x;
	int offset = h2(x);
	while(T[h(probe)] != -1){
		comparison++;
		probe = h(probe) + offset * h2(probe);
	}
		return h(probe);
}
//Quadratic probing
int Hash::searchEmptyQ(int x){
	int probe = x;
	int i=0;
	while(T[h(probe)] != -1){
		comparison++;
		i++;
		probe = h(x)+ i*i;
	}
		return h(probe);
}
//Linear probing
int Hash::searchEmptyL(int x){
	int probe = x;
	int i=0;
	while(T[h(probe)]!=-1){
		comparison++;
		i++;
		probe = x + i;

	}
		return h(probe);


}
//linear probing insert
void Hash::insertL(int x){

	T[searchEmptyL(x)]=x;
}
//Quadratic probing insert
void Hash::insertQ(int x){

	T[searchEmptyQ(x)]=x;
}
//DoubleHashing probing insert
void Hash::insertDH(int x){

	T[searchEmptyDH(x)]=x;
}

int Hash::getComp(){
	return comparison;
}
void Hash::resetComp(){
	comparison=0;
}
Hash::~Hash(){
	delete[] T;
}
