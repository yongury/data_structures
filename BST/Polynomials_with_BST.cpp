#include <iostream>

class Term {
   public:
      Term();

      int getCoeff();
      int getDegX();
      void setCoeff(int coeff);
      void setDegX(int deg_x);

   private:
      int coefF;
      int deg_X;


};



class Polynomial
{
   public :
      // create the zero polynomial
      Polynomial();
      // creates a monomial with one term c x^d
      Polynomial(int c, int d) ;
      // returns the highest degree of the polynomial
      // in the special case of the zero polynomial return -1
      int degree() const;
      // returns the coefficient of term that has degree n
      int coeff(int n) const;
      // evaluates the polynomial with the given value of x
      double evaluate(double x) const;
      // determines if the polynomial is the zero polynomial
      bool isZero() const;
      // insert a term into the polynomial
      void insert(int coeff, int degree);
      // returns the number of terms in the polynomial:
      // in the special case of the zero polynomial return 0
      int numberOfTerms() const;

      // copy constructor
      Polynomial(const Polynomial&);
      // destructor
      ~Polynomial();
      // overloaded assignment operator
      Polynomial& operator = ( const Polynomial&);

      // outputs the polynomial nicely
      friend std::ostream& operator<< (std::ostream&, const Polynomial&);
      // first read the number of terms followed by the coefficient
      // and degree for each term
      friend std::istream& operator>> (std::istream&, Polynomial&);
      // add (sum) two polynomials
      friend Polynomial operator +(const Polynomial& a, const Polynomial& b);


  private:

     struct Node {
         	  Node* left;
         	  Term* value;
      		  Node* right;
           };
      Node* root;

      static Node* makeLeaf(int coeff,int degree);
      static Node* insert(int coeff, int degree, Node* p);
      static Node* search(int degree, Node* p);
      static void searchSameAdd(int coeff, int degree,Node* &p);
      static void printInorder(Node* p);
      static Node* copyTree( Node* p);
      static void destroyTree(Node* &p);
      static void insertTerm(int coeff,int degree,Polynomial& p);
      static void sumHelperInorder(Node* p, Polynomial& c);

      static void evaluateHelperInorder(Node* p, int x,double& result);

      static int countNodes(const Node* p);

      static void printTerm(Node* p);

      static int degree(Node* p);
      static Node* minValueNode(Node* p);
      static Node* deleteNode(Node* root, int key);

};


// mononomial consists of one coefficient and degree of x
Term::Term(){

}
int Term::getCoeff(){
	return coefF;
}
int Term::getDegX(){
	return deg_X;
}
/*
int Term::getDegY(){
	return deg_Y;
}
*/
void Term::setCoeff(int coeff){
	this->coefF = coeff;
}
void Term::setDegX(int deg_x){
	this->deg_X = deg_x;
}


// create the zero polynomial
  Polynomial::Polynomial(){
	  root=nullptr;

  }


  // creates a monomial with one term c x^d
  Polynomial::Polynomial(int c, int d) {
	  root = makeLeaf(c,d);
  }

  // returns the highest degree of the polynomial
  // in the special case of the zero polynomial return -1
int Polynomial::degree() const{
	if(root==nullptr)
		return -1;
	else{
		Node* p =root;
		int larg;
			larg=p->value->getDegX();
			while(p->left){

				p=p->left;
				larg=p->value->getDegX();
			}
		return larg;

	}

}
  // returns the coefficient of term that has degree n
int Polynomial::coeff(int n) const{
	if(root==nullptr)
		return 0;
	else{
		Node* q= root;
		int coeff;
		q=search(n,q);
		if(q==nullptr)
			return 0;
		else
			coeff = q->value->getCoeff();

		return coeff;
	}

}
// help function to get x^degree
double xDegPow(int degree, int x){
	double result=1;
	for(int i=0;i<degree;i++)
		result *= x;
	return result;

}
//help function inorder style of evaluates the polynomial with the given value of x
void Polynomial::evaluateHelperInorder(Node* p, int x,double& result){
	if(p != nullptr){
		if(p->left !=nullptr)
			evaluateHelperInorder(p->left,x,result);

		result += p->value->getCoeff()*xDegPow(p->value->getDegX(),x) ;

		if(p->right !=nullptr)
			evaluateHelperInorder(p->right,x, result);

	}else{
		std::cout<<" empty \n";
	}
}

  // evaluates the polynomial with the given value of x
double Polynomial::evaluate(double x) const{
	double result=0;
	Node* p =root;
	evaluateHelperInorder(p,x,result);

	return result;
}
  // determines if the polynomial is the zero polynomial
bool Polynomial::isZero() const{
	return root==nullptr;
}
  // insert a term into the polynomial
void Polynomial::insert(int coeff, int degree){
	insertTerm(coeff,degree,*this);

}

//search if the p has a nomial of degree x
Polynomial::Node* Polynomial::search(int degree, Node* p){
	if(p == nullptr){
		return p;
	}
	else if( p->value->getDegX() == degree){
		return p;
	}
	else if(degree > p->value->getDegX())
		return search(degree,p->left);
	else
		return search(degree,p->right);
}
// add coefficient of same degree of x
void Polynomial::searchSameAdd(int coeff, int degree,Node* &p){
	//std::cout<<"searchSameAdd\n";
	if(p==nullptr){
		p=insert(coeff,degree,p);
	}
	//if root's degree == degree
	else if(p->value->getDegX() == degree){
		p->value->setCoeff(coeff + p->value->getCoeff());
		//if sum of coefficient of root is zero, remove the term
		if(p->value->getCoeff()==0){
			Node* q= p;
			q= deleteNode(q, q->value->getDegX());
			p=q;
		}
	}
	else{
		Node* q1=p;
		Node* q= p;
		q=search(degree,q);
		if(q==nullptr){
			p=insert(coeff,degree,p);
		}else{
			q->value->setCoeff(coeff + q->value->getCoeff());
			// if sum of coefficient is zero remove the term
			if(q->value->getCoeff()==0){
				q1= deleteNode(q1, q->value->getDegX());
			}
		}
	}
}
void Polynomial::insertTerm(int coeff,int degree, Polynomial& p){
	if(coeff == 0)
		return;
	searchSameAdd(coeff,degree,p.root);


}
// first read the number of terms followed by the coefficient
// and degree for each term
std::istream& operator>> (std::istream& input, Polynomial& p){
	  int coeff;
	  int degree;
	  int nTerm;
	  input >>nTerm;
	  for(int i=0;i<nTerm;i++){
		  input>>coeff>>degree;
		  Polynomial::insertTerm(coeff,degree,p);

	  }

    return input;
  }

//count total of term in the tree
int Polynomial::countNodes(const Node* p){
  if(p==nullptr)
    return 0;
  return countNodes(p->left)+countNodes(p->right)+1;
}
// return the total nmber of terms
int Polynomial::numberOfTerms() const{
	return countNodes(root);
}
// create a term
Polynomial::Node* Polynomial::makeLeaf(int coeff,int degree){
	Node* p = new Node();
	Term* t = new Term();
	p->value =t;
	t->setCoeff(coeff);
	t->setDegX(degree);
	p->left=nullptr;
	p->right = nullptr;
	return p;

}
// insert a term to the tree
Polynomial::Node* Polynomial::insert(int coeff, int degree, Node* p){
	if(p==nullptr) return makeLeaf(coeff,degree);

	if(degree > p->value->getDegX()){
		p->left = insert(coeff, degree, p->left);
	}
	else{
		p->right = insert(coeff,degree,p->right);
	}
	return p;
}
//print the term nicely
void Polynomial::printTerm(Node* p){
	char plusMinus='+';
//	if(p==this->root)
	//	plusMinus = ' ';
//	else{
		if(p->value->getCoeff()<0)
			plusMinus = '\0';


		if(p->value->getDegX()==1){
			if(p->value->getCoeff() == 1)
				std::cout<<"+x";
			else if( p->value->getCoeff() == -1)
				std::cout<<"-x";
			else
				std::cout<<plusMinus<<p->value->getCoeff()<<"x";
		}
		else if(p->value->getDegX()==0){
			std::cout<<""<<plusMinus<<""<<p->value->getCoeff();
		}
		else{
			if(p->value->getCoeff() == 1)
				std::cout<<"x^"<<p->value->getDegX();
			else if( p->value->getCoeff() == -1)
				std::cout<<"-x^"<<p->value->getDegX();
			else
				std::cout<<""<<plusMinus<<p->value->getCoeff()<<"x^"<<p->value->getDegX()<<"";

		}

}
//print Inorder style
void Polynomial::printInorder(Node* p){
	if(p != nullptr){
		if(p->left !=nullptr)
			printInorder(p->left);

		printTerm(p);
		if(p->right !=nullptr)
			printInorder(p->right);

	}else{
		std::cout<<" empty \n";
	}
}
//remove all terms and release the allocated memory
void Polynomial::destroyTree(Node* &p){
	if(p==nullptr)
			return;
	else{
		destroyTree(p->left);
		destroyTree(p->right);
		Node* discard = p;
		delete discard;
		p=nullptr;

	}
}

Polynomial::Node* Polynomial::copyTree( Node* p){
	if(p==nullptr)
		return nullptr;

	Node* newP = makeLeaf(p->value->getCoeff(),p->value->getDegX());
	newP->left = copyTree(p->left);
	newP->right = copyTree(p->right);
	return newP;
}
Polynomial::Polynomial(const Polynomial& other){
	if(other.root==nullptr)
		return;
	root = copyTree(other.root);
}

Polynomial& Polynomial::operator = ( const Polynomial& rtSide){

	if( this == &rtSide)
		return *this;

	destroyTree(root);
	root = copyTree(rtSide.root);

	return *this;
}




std::ostream& operator<< (std::ostream& output, const Polynomial& p){
	if(p.root==nullptr)
		output<<"( )";
	else{
		Polynomial::Node* q=p.root;
		Polynomial::printInorder(q);
	}

	return output;
}

void Polynomial::sumHelperInorder(Node* p, Polynomial& c){
	if(p != nullptr){
		if(p->left !=nullptr)
			sumHelperInorder(p->left,c);
		Polynomial::insertTerm(p->value->getCoeff(),p->value->getDegX(),c);
		if(p->right !=nullptr)
			sumHelperInorder(p->right,c);

	}else{
		std::cout<<" empty \n";
	}
}
Polynomial operator +(const Polynomial& a, const Polynomial& b){
	Polynomial c;

	if(a.root !=nullptr)
		Polynomial::sumHelperInorder(a.root,c);
	if(b.root !=nullptr)
		Polynomial::sumHelperInorder(b.root,c);

	return c;
}

Polynomial::Node* Polynomial::minValueNode(Node* p){
    Node* current = p;

    // loop down to find the leftmost leaf
    while (current->right != nullptr)
        current = current->right;

    return current;
}

Polynomial::Node* Polynomial::deleteNode(Node* root, int degree){
    // base case
    if (root == nullptr) return root;

    // If the degree to be deleted is smaller than the root's degree,
    // then it lies in left subtree
    if (degree > root->value->getDegX())
        root->left = deleteNode(root->left, degree);

    // If the degree to be deleted is greater than the root's degree,
    // then it lies in right subtree
    else if (degree < root->value->getDegX())
        root->right = deleteNode(root->right, degree);

    // if degree is same as root's degree, then This is the node
    // to be deleted
    else{
        // node with only one child or no child
        if (root->left == nullptr){
            Node *temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr){
            Node *temp = root->left;
            delete root;
            return temp;
        }

        // node with two children: Get the inorder successor (smallest
        // in the right subtree)
        Node* temp = minValueNode(root->left);

        // Copy the inorder successor's content to this node
        root->value->setCoeff(temp->value->getCoeff());
        root->value->setDegX(temp->value->getDegX());

        // Delete the inorder successor
        root->left = deleteNode(root->left, temp->value->getDegX());
    }
    return root;
}
Polynomial::~Polynomial(){
	destroyTree(root);
	root=nullptr;

}
