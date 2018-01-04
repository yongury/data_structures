

//================================Poly.h================================
#include <string>
#include <fstream>

class Term {
   public:
      Term();

      int getCoeff();
      int getDegX();
      int getDegY();
      void setCoeff(int coeff);
      void setDegX(int deg_x);
      void setDegY(int deg_y);

   private:
      int coefF;
      int deg_X;
      int deg_Y;


};



class Poly {
   public:
      Poly();

      // add polynomial
      void insert(int coeff,int deg_x,int deg_y);

      void sort();
      //differentiation of X
      void diffX();
      //differentiation of Y
      void diffY();
      //get total number of poynomials of the list
      int getTotalPoly();

      void printPoly();
      // add two polynomials
      friend const Poly operator +(const Poly & a, const Poly & b);
      // copy constructor
      Poly(const Poly&);

	// destructor
	  ~Poly();


   private:
      struct Node {

    	  Term* value;
    	  Node* next;

      };
      //functor comparison of two polynomials
      //if a poly's degree of x is bigger return 1;
      //if a, b has same degree of x then compare degree of y return -1
      //if x and y are same return 0
      struct Compare {
		  int operator()(Poly::Node* a, Poly::Node* b){
		  int a_x = a->value->getDegX();
		  int a_y = a->value->getDegY();

		  int b_x = b->value->getDegX();
		  int b_y = b->value->getDegY();

		  if(a_x > b_x)
			  return 1;
		  else if(a_x <b_x)
			  return -1;
		  else{
			  if(a_y > b_y)
				  return 1;
			  else if(a_y <b_y)
				  return -1;
			  else
				  return 0;
		  }
	  }
	};

     // Compare cmp1;
      Node* head;

      int numPoly;

      //sort by mergesort
      static void merge(Node* A, int a_start, int a_end,int b_start, int b_end, Node* &W);
      static void mergeSort(Node* A, int low, int high, Node* &W);
      static Node* cons( int coeff,int deg_x,int deg_y,Node *p );
//      static Node*& append( int coeff,int deg_x,int deg_y,Node* &p );
      static Node* reverse(Node* p);
      static Node* copyNode(Node* p);
      static bool remove(Node* target, Node* &p);
      static Node* mergeListUniques(Node* p, Node* q);
      static int countPoly(Node* p);

};


//================================Poly.cpp================================

#include <iostream>
#include <limits>
#include <ios>

Poly::Poly(){
	head=nullptr;
	numPoly=0;
}

void Poly::merge(Node* A, int a_start, int a_end,int b_start, int b_end, Node* &W){

	Node* p = A;
	//using functor to compare of term of two polynomials
	Compare cmp;
	//initialize p as a_start's term
	for(int i=0;i<a_start;i++)
		p=p->next;
	int pEnd = a_end - a_start;
	Node* q = A;
	//initialize q as b_start ( 1 term after a_end)
	for(int i =0;i<=a_end;i++)
		q=q->next;
	int qEnd = b_end - b_start;

	//create List to store sorted term
	Node* r = W;

	while(pEnd >=0 && p && q && qEnd >= 0){
		//merge sort first part
		//if p List term is greater than q List term, store to r List
		if(cmp(p,q)>0){
			r=cons(p->value->getCoeff(),p->value->getDegX(),p->value->getDegY(),r);
			p=p->next;
			pEnd--;
		}
		//if p List term is smaller than q List term, store to r List
		else if( cmp(p,q)< 0){
			r=cons(q->value->getCoeff(),q->value->getDegX(),q->value->getDegY(),r);
			q=q->next;
			qEnd--;
		}
		//if p List term is same as q List term, store both to r List
		else{
			r=cons(p->value->getCoeff(),p->value->getDegX(),p->value->getDegY(),r);
			p=p->next;
			pEnd--;
			r=cons(q->value->getCoeff(),q->value->getDegX(),q->value->getDegY(),r);
			q=q->next;
			qEnd--;

		}

	}
	//store the rest of p to r if p has remained term
	while(p && pEnd >=0){
		r=cons(p->value->getCoeff(),p->value->getDegX(),p->value->getDegY(),r);
		p=p->next;
		pEnd--;
	}
	//store the rest of q to r if q has remained term
	while(q && qEnd >=0){
		r=cons(q->value->getCoeff(),q->value->getDegX(),q->value->getDegY(),r);
		q=q->next;
		qEnd--;
	}


	p = A;
	for(int i=0;i<a_start;i++)
		p=p->next;

	//reverse the list in order to make DESC
	r=reverse(r);

	//Set A with sorted r
	while(p && r){
		p->value->setCoeff(r->value->getCoeff());
		p->value->setDegX(r->value->getDegX());
		p->value->setDegY(r->value->getDegY());
		p=p->next;
		r=r->next;
	}

}

Poly::Node* Poly::reverse(Node* p){
	Node* tmp=nullptr;
	while(p){
		tmp=cons(p->value->getCoeff(),p->value->getDegX(),p->value->getDegY(),tmp);
		p=p->next;
	}
	return tmp;


}
// add a term at the beginning of the list
Poly::Node* Poly::cons( int coeff,int deg_x,int deg_y,Node *p ) {
   Node* q;
   q = new Node;
  q->next = p;

   Term* q1;
   q1= new Term;

   q1->setCoeff(coeff);
   q1->setDegX(deg_x);
   q1->setDegY(deg_y);
   q->value = q1;


   return q;
}
//
//Poly::Node*& Poly::append( int coeff,int deg_x,int deg_y,Node* &p ) {
//	if(p)
//	std::cout<<"append "<<p->value->getDegX()<<"\n";
//	else{
//
//	Node* q;
//   q = new Node;
//  q->next = nullptr;
//   std::cout<<"append "<<"\n";
//   p = q;
//	}
//
//
//
//   return p;
//}
//merge sort of A list and store to W List
void Poly::mergeSort(Node* A, int low, int high, Node* &W){

	if(high==low)
		return;
	int mid = (high + low)/2;

	mergeSort(A,low,mid,W);
	mergeSort(A,mid+1,high,W);
	merge(A,low,mid,mid+1,high,W );
}
void Poly::insert(int coeff,int deg_x,int deg_y){
	head = cons(coeff,deg_x,deg_y,head);
	numPoly++;
}

void Poly::sort(){
	Node* W;
	W=nullptr;

	mergeSort(head,0,numPoly-1,W);
}

void Poly::printPoly(){
	Node* p;
	p=head;
	char pre = '+';
	while(p){
		if(p->value->getCoeff()<0)
			pre = ' ';
		else
			pre = '+';

		std::cout<<pre<<p->value->getCoeff()<<"x^"<<p->value->getDegX()<<"y^"<<p->value->getDegY()<<"";
		p=p->next;
	}
	std::cout<<"\n";
}

int Poly::getTotalPoly(){
	return numPoly;
}
//count total number of terms in list
int Poly::countPoly(Node* p){

	if(p==nullptr)
		return 0;
	else{
		int count=0;
		while(p){
			p=p->next;
			count++;
		}
		return count;
	}
}
// add two polynomials
const Poly operator +(const Poly & a, const Poly & b) {


	Poly::Node* W1;
	W1=nullptr;
	//mergesort a
	Poly::mergeSort(a.head,0,a.numPoly-1,W1);

	Poly::Node* W2;
	W2=nullptr;
	//mergesort b
	Poly::mergeSort(b.head,0,b.numPoly-1,W2);


    Poly c;
    c.head = Poly::mergeListUniques(a.head,b.head);
    c.numPoly = Poly::countPoly(c.head);
    return c;

}

//merge two sorted list recursively
//if degree of x, y are same, sum their coefficient
//if sum of coefficient is zero, remove the term
Poly::Node* Poly::mergeListUniques(Node* p, Node* q){
  if(p==nullptr){
    if(q==nullptr)
      return nullptr;
    else{
      return cons(q->value->getCoeff(),q->value->getDegX(),q->value->getDegY(),mergeListUniques(nullptr,q->next));
    }
  }
  Compare cmp;
  if(q==nullptr)
    return mergeListUniques(q,p);

  if(cmp(p,q) == 0){
	  int sumCoeff;
	  	  sumCoeff = p->value->getCoeff() + q->value->getCoeff();
	  if(sumCoeff == 0)
		  return mergeListUniques(p->next,q->next);
	  else
		  return cons(sumCoeff,p->value->getDegX(),p->value->getDegY(), mergeListUniques(p->next,q->next));
  }
  else if(cmp(p,q) < 0){
    return cons(p->value->getCoeff(),p->value->getDegX(),p->value->getDegY(), mergeListUniques(p->next,q));
  }
  else
    return cons(q->value->getCoeff(),q->value->getDegX(),q->value->getDegY(), mergeListUniques(q->next,q));
}


bool Poly::remove(Node* target, Node* &p){
	if(p==nullptr)
		return false;
	if(p->value->getDegX() == target->value->getDegX() && p->value->getDegY() == target->value->getDegY()){
		Node* discard = p;
		p=p->next;
		delete discard;
		return true;
	}

	return remove(target,p->next);

}

Poly::Node* Poly::copyNode(Poly::Node* p){
	if(p==nullptr)
		return p;
	Node* copied = new Node();
	Term* copiedValue= new Term();
	copied->value = copiedValue;

	copied->value->setCoeff(p->value->getCoeff());

	copied->value->setDegX(p->value->getDegX());

	copied->value->setDegY(p->value->getDegY());
	copied->next = copyNode(p->next);

	return copied;
}
//differentiation of X
void Poly::diffX(){
	Node* p;

	std::cout<<"diffX : ";
	p = copyNode(head);
	char pre='+';


	while(p){
		if(p->value->getCoeff()<0)
			pre = ' ';
		else
			pre = '+';

		p->value->setCoeff(p->value->getCoeff() * p->value->getDegX());

		p->value->setDegX(p->value->getDegX()-1);
		if(p->value->getDegX() >=0 && p->value->getCoeff() !=0)
			std::cout<<pre<<p->value->getCoeff()<<"x^"<<p->value->getDegX()<<"y^"<<p->value->getDegY()<<"";
		p=p->next;

	}

	std::cout<<"\n";

}
//differentiation of Y
void Poly::diffY(){
	Node* p;
	p = copyNode(head);
	char pre='+';
	std::cout<<"diffY : ";
	while(p){
		if(p->value->getCoeff()<0)
			pre = ' ';
		else
			pre = '+';

		p->value->setCoeff(p->value->getCoeff() * p->value->getDegY());

		p->value->setDegY(p->value->getDegY()-1);
		if(p->value->getDegY() >=0 && p->value->getCoeff() !=0)
			std::cout<<pre<<p->value->getCoeff()<<"x^"<<p->value->getDegX()<<"y^"<<p->value->getDegY()<<"";
		p=p->next;
	}
	std::cout<<"\n";
}
//copy constructor
Poly::Poly(const Poly&){
}

// destructor
Poly::~Poly(){
}

// consiss of one coefficient, degree of X, and degree of Y
Term::Term(){

}
int Term::getCoeff(){
	return coefF;
}
int Term::getDegX(){
	return deg_X;
}
int Term::getDegY(){
	return deg_Y;
}
void Term::setCoeff(int coeff){
	this->coefF = coeff;
}
void Term::setDegX(int deg_x){
	this->deg_X = deg_x;
}
void Term::setDegY(int deg_y){
	this->deg_Y = deg_y;
}

//=========================testPoly.cpp========================

#include <iostream>
#include "Poly.h"
#include <sstream>

using namespace std;
//#include <string>
//#include<sstream>
using std::cout;
//using std::stoi;
#include <string>


#include <fstream>


int main() {
	cout<<"Polynomial\n";

	std::cout<<"=============================\n";
	Poly a;
	Poly b;
		a.insert(5,2,3);
		a.insert(2,8,5);
		a.insert(-3,5,1);
		a.insert(7,5,2);
		b.insert(103,1,1);
		b.insert(-3,2,1);
		b.insert(1,1,4);
		b.insert(1,5,100);
		cout<<"unsorted\n";
		cout<<"poly1: ";
		a.printPoly();
		cout<<"poly2: ";
		b.printPoly();
		cout<<"sorted\n";
		a.sort();
		b.sort();
		cout<<"a: ";
		a.printPoly();
		cout<<"b: ";
		b.printPoly();

		Poly c;
		std::cout<<"a+b : ";
		c= a + b;
		c.printPoly();
		cout<<"da/dx: ";
		a.diffX();
		cout<<"da/dy: ";
		a.diffY();
		cout<<"db/dx: ";
		b.diffX();
		cout<<"db/dy: ";
		b.diffY();
		std::cout<<"=============================\n";
		std::cout<<"=============================\n";
			Poly a1;
			Poly b1;
				a1.insert(7,0,0);
				a1.insert(1,1,0);
				a1.insert(2,8,5);
				a1.insert(5,2,3);
				b1.insert(7,8,1);
				b1.insert(-2,8,5);

				cout<<"unsorted\n";
				cout<<"poly1: ";
				a1.printPoly();
				cout<<"poly2: ";
				b1.printPoly();
				cout<<"sorted\n";
				a1.sort();
				b1.sort();
				cout<<"a: ";
				a1.printPoly();
				cout<<"b: ";
				b1.printPoly();

				Poly c1;
				std::cout<<"a+b : ";
				c1= a1 + b1;
				c1.printPoly();
				cout<<"da/dx: ";
				a1.diffX();
				cout<<"da/dy: ";
				a1.diffY();
				cout<<"db/dx: ";
				b1.diffX();
				cout<<"db/dy: ";
				b1.diffY();
		std::cout<<"=============================\n";
		std::cout<<"=============================\n";
			Poly a2;
			Poly b2;
				a2.insert(-2,5,4);
				a2.insert(3,4,3);
				a2.insert(1,0,1);
				b2.insert(4,0,0);
				b2.insert(6,0,0);
				b2.insert(1,1,1);

				cout<<"unsorted\n";
				cout<<"poly1: ";
				a2.printPoly();
				cout<<"poly2: ";
				b2.printPoly();
				cout<<"sorted\n";
				a2.sort();
				b2.sort();
				cout<<"a: ";
				a2.printPoly();
				cout<<"b: ";
				b2.printPoly();

				Poly c2;
				std::cout<<"a+b : ";
				c2= a2 + b2;
				c2.printPoly();
				cout<<"da/dx: ";
				a2.diffX();
				cout<<"da/dy: ";
				a2.diffY();
				cout<<"db/dx: ";
				b2.diffX();
				cout<<"db/dy: ";
				b2.diffY();

}
