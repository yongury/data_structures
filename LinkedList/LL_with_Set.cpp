
//===============================Set.h====================
#include <iostream>

class Set {
   public:
      // constructs the empty Set
      Set();
      // returns true if the Set is empty, false otherwise
      bool isEmpty() const;
      // return the number of elements in the Set i.e. the
      // cardinality of the set
      int size() const;
      // insert x into the Set (in ascending order)
      //  (if x is already in the Set, does nothing)
      void insert(int x);

      // return the union of a and b
      // note that neither the Set a nor the Set b are changed
      friend const Set operator +(const Set & a, const Set & b);

      // return the intersection of a and b
      // note that neither the Set a nor the Set b are changed
      friend const Set operator *(const Set & a, const Set & b);

      // output a set in the form { element0 element 1  ... elementn-1 }
      friend std::ostream& operator<< (std::ostream&, const Set&);

      friend bool operator<(const Set & a, const Set& b);

      // copy constructor
      Set(const Set&);
      // destructor
      // implementation detail: return all the memory of the linked list
      ~Set();
      // overloaded assignment operator so that one set can be assigned to another
      Set& operator = (const Set& rtSide);

      void reverse();

   private:
      struct Node {
         int value;
         Node* next;
      };
      // The linked list is sorted in ascending order so that the
      // Set operations union and intersect can be done in linear time using merging.
      //  The insert operation needs to insert in order and if x is already there,
      // it doesn't do anything.
      // So the list is always sorted and never has duplicates.
      Node* list;

      // suggested constructor:
      // constructor that takes p and sets the list to p
      // no copies of the nodes of p are made
      Set(Node* p);

      // make a new node with the value x and the link set to p
      // return a pointer to the new node
      // we want to access cons in friend functions so make it static
      static Node * cons(int x, Node* p );


      // reverse in linear time the list pointed to by p
      // the reverse is destructive so the list returned are the
      // original nodes
      // we want to access reverse in friend functions so make it static
      static Node * reverse(Node * p);
      // return intersection of two lists
      static Node* commonInLists(Node* p, Node* q);
      static Node* mergeListUniques(Node* p, Node* q);
      static Node* insert(int x, Node* &p);

      static Node* copySet(Node* p);

      static Node* reverseNoMemory(Node* p);

      static void deleteSet(Node* & p);
};


//==============================Set.cpp======================

// constructor
Set::Set():list(nullptr) {
   // no body
}

// Set operations
// test if the Set is empty by comparing the private variable list
// to the nullptr
// one operation so regardless  of size of set  O(1)
// precondition:
//   p has been assigned a valid address or the nullptr
bool Set::isEmpty() const{
   return (list == nullptr);
}

// output onto out a list of the form { element0 element 1  ... elementn-1 }
// precondition:
//  Set s is a valid, well initialized Set
// complexity of printing the list is O(n) when there  are n elements in the Set s
std::ostream& operator<< (std::ostream& out, const Set& s) {
   out << "{ ";
   for (Set::Node* p = s.list; p != nullptr; p = p->next )
      out << p->value << " ";
   out << "}";
   return out;
}

// union of Set a and Set b
// use a type of "merge algorithm"
// a has m elements and b has n elements and both linked lists are sorted
// in the worse case, when the last value in both sets is the same, we need to go
// through both sets .... thus  union is O(m+n) (and this includes the reverse)
// precondition:
//  the lists in the Set a and in the Set b  are sorted in ascending order
const Set operator +(const Set & a, const Set & b) {


   Set::Node *p = a.list;  // pointer to help me traverse a
   Set::Node *q = b.list;  // pointer to help me traverse b
   Set::Node *result = nullptr;  // points to the resulting list from the merge

   result = Set::mergeListUniques(p,q);
   Set c;
   c.list =result;
   return c;

}
//return merged list without redundant value
Set::Node* Set::mergeListUniques(Node* p, Node* q){
  if(p==nullptr){
    if(q==nullptr)
      return nullptr;
    else{
      return cons(q->value,mergeListUniques(nullptr,q->next));
    }
  }
  if(q==nullptr)
    return mergeListUniques(q,p);
  if(p->value == q->value)
    return cons(p->value, mergeListUniques(p->next,q->next));
  else if(p->value < q->value){
    return cons(p->value, mergeListUniques(p->next,q));
  }
  else
    return cons(q->value, mergeListUniques(p,q->next));
}

// intersection  ... a type of merge algorithm
// traverse both lists a and b until the values of both lists are equal
// and then make a Node with that value and add it to the list pointed to by result
// complexity: O(n)
// if a has m elements and b has n elements and both linked lists are sorted,
// in the worse case their intersection takes O(m+n)
// reverse is done in linear time and we know that reverse takes at most
// minimum(n,m)
// precondition:
//  the lists in the Set a and in the Set b  are sorted in ascending order
const Set operator *(const Set& a, const Set& b) {

   // you can change the code in this function

   Set::Node *p = a.list;  // pointer to help me traverse a
   Set::Node *q = b.list;  // pointer to help me traverse b
   Set::Node *result = nullptr;  // points to the resulting list from the merge


	result = Set::commonInLists(p,q);
	Set c;
	c.list = result;
	return c;
}
// return common values of two lists
Set::Node* Set::commonInLists(Node* p, Node* q){
  if(p==nullptr || q==nullptr)
	  return nullptr;
  if(p->value == q->value){
      return cons(q->value,commonInLists(p->next,q->next));
  }
  else if(p->value < q->value)
      return commonInLists(p->next,q);
  else
      return commonInLists(p,q->next);
}
//if a is subset of b, return true
//otherwise return false
bool operator< (const Set & a, const Set& b){
	Set::Node *p = a.list;  // pointer to help me traverse a
	Set::Node *q = b.list;  // pointer to help me traverse b
	while(p!=nullptr && q!=nullptr){
	    if(p->value < q->value)
	      return false;
	    else if( p->value > q->value)
	      q=q->next;
	    else{
	      p=p->next;
	      q=q->next;
	    }
	  }
	  return (p==nullptr);

}

// copy constructor
Set::Set(const Set& otherSet) {
   #ifndef NDEBUG
   std::cout << "&&&&&&&&&& copy constructor called &&&&&&&&&&&\n";
   #endif
   list = copySet(otherSet.list);

}
//delete list
void Set::deleteSet(Node* & p){
	if(p){
		Node* discard;
		discard = p;
		p=p->next;
		delete discard;
		deleteSet(p);
	}
}

// destructor
// postcondition:
//    every Node in the list is deleted and the list is set to nullptr
Set::~Set(){
   #ifndef NDEBUG
   std::cout << "called the destructor+++++++++++++++\n";
   #endif
   deleteSet(list);
}

//copy list
Set::Node* Set::copySet(Node* p){
  if(p==nullptr)
    return nullptr;

  return cons(p->value, copySet(p->next));
}
// overload the assignment operator
Set& Set::operator = (const Set& otherSet) {
   #ifndef NDEBUG
   std::cout << "!!!!!!!!!!!!!!!!!!! called the overloaded = operator\n";
   #endif
   if(this != &otherSet){
	   deleteSet(list);
   }
   list = copySet(otherSet.list);


   return *this;
}

// make a new node and put in the value field x and in the next field p
// precondition: none
// postcondition:
//   a pointer to the newly created Node is returned
// complexity: O(1)
Set::Node * Set::cons( int x, Node * p ) {
   Node * q;
   q = new Node;
   q->value = x;
   q->next = p;
   return q;
}

int Set::size() const{
	Node* free;
	free=list;
	int count=0;
	std::cout<<"size \n";

	while (free)
		{
		  ++count;
		  free = free->next;
		}

	return count;
}
// help function for insert(int x)
Set::Node* Set::insert(int x, Node* &p){

	if(p==nullptr || x < p->value){
		p=cons(x,p);
		return p;
	}
	if(x == p->value)
		return p;
	else{
		Node* q;
		q=p;
		q->next=insert(x,p->next);
		return q;
	}

}
// insert x into the Set (in ascending order)
//  (if x is already in the Set, does nothing)
void Set::insert(int x){

	list = insert(x,list);

}

void Set::reverse(){
	list = reverseNoMemory(list);
}

//reverse list without memory allocation
Set::Node* Set::reverseNoMemory(Node* p){
  if(p==nullptr){
    return nullptr;
  }
  Node* tail = p->next;
  if(tail ==nullptr){
    return p;
  }
  Node* first = reverseNoMemory(tail);
  tail->next =p;
  p->next = nullptr;
  return first;
}
//===========================testSet.cpp====================

#include <cstdlib>

void f(Set a) {
   a.insert(19);
}

Set g() {
   Set a;
   a.insert(3);
   a.insert(2);
   a.insert(5);
   return a;
}

void h() {
   Set local;
   local.insert(1);
   local.insert(2);
   std::cout << "a local set has values " << local << "\n";
}

int main() {
    using std::cout;
    using std::endl;
    Set s, t;
    cout << "empty set: " <<  s << endl;
    cout << "\t expecting: { }\n";
    cout << "|s|=" << s.size() << endl;
    cout << "\t expecting:|s|= 0\n";
    cout<<"insert begin\n";
    s.insert(3);

    s.insert(1);
    s.insert(9);
    s.insert(3); s.insert(7); s.insert(5);
    s.insert(9);

    cout << "s = " << s << endl;

    cout << "\t expecting: s = { 1 3 5 7 9 }\n";
    t.insert(3); t.insert(5); t.insert(4);
    t.insert(5);
    cout << "with insertions: t = "<< t << endl;
    cout << "\t expecting: t = { 3 4 5 }\n";

    Set a = s + t;
    cout << "s union t = " << a << endl;
    cout << "\t expecting: s = { 1 3 4 5 7 9 }\n";

    Set b = s * t;
    cout << "s intersect t = " << b << endl;
    cout << "\t expecting: t = { 3 5 }\n";


    Set c(b); //copy constructor test
    cout << "initialize c with b = " << c << endl;
    cout << "\t expecting: c = { 3 5 }\n";

    //operator = test
    c=t;
    cout << "assign t to c  = " << c << endl;
    cout << "\t expecting: c = {3 4 5 }\n";

     //destructor test

     c.~Set();
     cout << "delete c  = " << c << endl;
         cout << "\t expecting: c = { }\n";

     // operator < test
     cout<<"operator < test\n";
     cout<<" c < t ?\t"<<"c :"<<c<<"\tt :"<<t<<endl;

     if(c<t)
    	 cout<<"t < c\n";
     else
    	 cout<<"t </ c \n";

     cout<<" t < c ?\t"<<"t :"<<t<<"\tc :"<<c<<endl;
     if(t<c)
    	 cout<<"t < c\n";
     else
    	 cout<<"t </ c \n";

     cout<<" b < t ?\t"<<"b :"<<b<<"\tt :"<<t<<endl;
	  if(b<t)
		 cout<<"b < t\n";
	  else
		 cout<<"b </ t \n";
	  cout<<" t < b ?\t"<<"t :"<<t<<"\tb :"<<b<<endl;
	  if(t<b)
		 cout<<"t < b\n";
	  else
		 cout<<"t </ b \n";
	  cout<<" t < t ?\t"<<"t :"<<t<<"\tt :"<<t<<endl;
	  	  if(t<t)
	  		 cout<<"t < t\n";
	  	  else
	  		 cout<<"t </ t \n";
	  cout<<"a :"<<a <<endl;
	  cout<<"b :"<<b <<endl;
	  cout<<"s :"<<s <<endl;
	  s.reverse();
	  cout<<"s DESC:"<<s <<endl;
	  //cout<<"a :"<<a <<endl;
    return 0;
}
