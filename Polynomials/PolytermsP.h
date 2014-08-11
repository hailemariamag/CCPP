
//////////////////////////////////////////////////////// 
// ECE 2574, Homework 2, Alazar Hailemariam
// 
// File name:   Poly.h 
// Description: Header file for the ADT Poly 
// Date:        10/05/2011  
// 
#include <iostream>  
#include <iomanip> 
#include <stdlib.h>  
using namespace std;  



typedef double CoefficientItem;  
//typedef int PolyItem;
//typedef float PolyItem;
struct Node;

class Poly  
{  
	// overload operator<< to print a Poly 
	friend ostream& operator<<(ostream& out, Poly& m); 
public: 
	Poly();
	// Default Constructor: create an empty list
	Poly(CoefficientItem co, int ex);
	// Constructor: create a list with a single polynomial(cofficient, exponent) 
	Poly& insertPoly(CoefficientItem co, int ex); 
	// Copy constructor: create a deep copy of Poly orig 
	Poly(const Poly& orig);
	// Destructor: deallocate memory that was allocated dynamically 
	~Poly();  
	// Overloaded assignment operator 
	Poly& operator=(const Poly& orig); 
	// Overloaded Poly math operators 
	Poly operator+(const Poly& orig)const; 
	Poly operator*(const Poly& orig) const;
	Poly operator/(const Poly& orig) const;
	Poly operator%(const Poly& orig) const;

private: 
	Node *Head;
}; // End Poly class  
// End header file