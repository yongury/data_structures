

#include <iostream>
#include <fstream>
#include <limits> // for declaration of 'numeric_limits' for cin
#include <ios>    // for declaration of 'streamsize' for cin
using std::cout;
using std::endl;
using std::string;
using std::cin;
using std::ifstream;
using std::istream;

#include <queue>
#include <stack>

struct Node {
   int vertex;
   Node *next;
};

typedef Node* List;

// make a new node and link it to p
List cons(int v, List p);

// determine if the vertex v is in the List
bool member(int v, List p){
	if(p==nullptr)
		return false;
	if(p->vertex == v)
		return true;
	return member(v,p->next);
}

// insert edge  ij into G if not in G already
// add a Node both to G[j] and to G[i] because the graph is simple, undirected
// check that 0 <= i < n and that 0 <= j < n and if not, ignore and do not insert edge
void insertEdge(int i, int j, List *G, int n){
	if(i < 0 || j < 0 || i > n || j >n)
		return;
	else{
		if(member(j,G[i]) && member(i,G[j])){
			return;
		}
		else{
				G[i]=cons(j,G[i]);
				G[j]=cons(i,G[j]);
		}
	}
}
void printList(List* G,int n){
	List p;

	for(int i=0;i<n;i++){
		cout<<"G["<<i<<"] :";
		p=G[i];
		while(p != nullptr){
			cout<<p->vertex<<", ";
			p=p->next;
		}
		cout<<"\n";
	}

}
//using recursive traverse
void visit(List* G, bool* M, int v){
	M[v]= true;
	for(List p=G[v];p!=nullptr;p=p->next){
		int u = p->vertex;
		if(!M[u])
			visit(G,M,u);
	}
}
//using queue breadth first traverse
void visitQue(List* G, bool* M, int v){
	std::queue<int> que;

	que.push(v);
	while(!que.empty()){
		v = que.front(); //deque the first vertex
		que.pop();
		List p= G[v]; //traverse v's neighbour vertex list
		while(p != nullptr){
			if(!M[p->vertex]){ // if the vertex is not traversed, assign to true(traversed)
				que.push(p->vertex); // add vertex to queue
				M[p->vertex]= true;
			}
			p=p->next;
		}
	}
}
//using stack depth first traverse
void visitStack(List* G, bool* M, int v){

	std::stack<int> sta;
	sta.push(v);
	while(!sta.empty()){
		v=sta.top();
		sta.pop(); //pop the top vertex
		List p = G[v]; // traverse v's neighbour list
		while(p != nullptr){
			if(!M[p->vertex]){ //if vertex is not traversed, assign to true
				sta.push(p->vertex);// add ertex to stack
				M[p->vertex]= true;
			}
			p=p->next;
		}

	}
}


bool isConnected(List* G,int n){
	if(n<=1)
		return true;
	bool* M = new bool[n];
	for(int i=0;i<n;i++)
		M[i]=false;
	int v0=0;
	//visit(G,M,v0); // call recursively visit
	//visitQue(G,M,v0); // call using queue
	visitStack(G,M,v0); // call using stack
	for(int i=0;i<n;i++){
		if(!M[i]){
			delete[] M;
			return false;
		}

	}
	cout<<"M :";
	for(int i=0;i<n;i++)
		cout<<M[i]<<" ";
	cout<<"\n";
	delete[] M;
	return true;

}

// make a new node and link it to p
List cons(int v, List p) {
   return new Node{v, p};
}
