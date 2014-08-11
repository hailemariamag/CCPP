//////////////////////////////////////////////////////// 
// ECE 2574, Homework 2, Alazar Hailemariam
// 
// File name:   PolytermsP.cpp
// Description: Implementation file for the ADT PolytermsP header
// Date:        10/05/2011  
//

#include<iostream>
#include <iomanip> 
#include <stdlib.h> 
#include"PolytermsP.h"

using namespace std;

struct  Node
{
	int exponent;
	CoefficientItem coefficient;
	Node *next;
};
Poly::Poly()					//Default Constructor: Create empty list
{
	Head = NULL;
}
Poly::Poly(CoefficientItem co, int ex)	//Constructor: Create list with a single polynomial
{
	try
	{
		Head = NULL;
		Head = new Node;
		if(Head == NULL)
			throw 1;
		Head->coefficient = co;		//insert coefficient and exponent
		Head->exponent = ex;
		Head->next = Head;			//achieve circular link
	}
	catch(int)
	{
		cout<<"Cannot alocate memory!\n";
	}
}
Poly::Poly(const Poly& orig)	//Copy Constructor: Deep copy list
{
	try
	{
		if(orig.Head==NULL)		//If list being copied is empty
		{
			Head=NULL;
		}
		else					//If list being copied is not empty
		{
			Node *cur, *cur2;
			Head = NULL;
			Head = new Node;
			if(Head == NULL)
				throw 1;
			cur2 = Head;
			cur = orig.Head;
			bool loop = true;												//bool to skip first loop of for loop because a circular list is used
			for(cur = orig.Head; (cur!=orig.Head) || (loop); cur=cur->next)	//transverse list
			{
				loop = false;
				cur2->coefficient = cur->coefficient;	//Copy coefficient and exponent
				cur2->exponent = cur->exponent;	
				if(cur->next==orig.Head)				//If transverse is complete
					cur2->next = Head;					//Circular link copied list
				else						//If transverse is not complete
				{
					cur2->next = new Node;	//Create new next node for list being created
					if(cur->next == NULL)	
						throw 2;
					cur2 = cur2->next;		//advance transverse node for list being copied
				}
			}
		}
	}
	catch(int)			//catch exception memory cannot be allocated
	{
		cout<<"Cannot allocate memory!\n";
	}
}
Poly::~Poly()				//Destructor
{
	if(Head != NULL)		//if list is not empty
	{
		Node * cur = Head;
		Node * cur2;
		while(cur->next != Head)	//Delete everything except head
		{
			cur = Head->next;
			cur2 = cur->next;
			Head->next = cur2;
			delete cur;
			cur = NULL;
			cur = cur2;
		}
		delete Head;			//Delete head and make NULL
		Head = NULL;		
	}	
}
ostream& operator<<(ostream& out, Poly& m)		//Print Overload
{
	Node *cur;
	bool loop = true;							//Bool used to bypass first cycle of for loop because list is circular
												//meaning end condition is same as begin condition
	if(m.Head == NULL)							//For empty matrix or for operations where resulting list is 0
	{
		out<<"0 0\n";
		return out;
	}
	for(cur=m.Head; (cur!=m.Head) || (loop); cur = cur->next)	//Transverse List
	{
		loop = false;
		out<<cur->coefficient<<" "<<cur->exponent<<"\n";		//Format to output to file
	}
	return out;
}
Poly& Poly::insertPoly(CoefficientItem co, int ex)	//Sorted Insert Function
{
	try
	{
		if(this->Head==NULL)				//Insert one item into an empty list
		{
			this->Head = new Node;			//Insert item and create circular list wiht one node
			if(Head == NULL)
				throw 1;
			this->Head->coefficient = co;
			this->Head->exponent = ex;
			this->Head->next = Head;
			return *this;
		}
		Node *cur;
		Node *cur2;
		bool inserted = false;
		bool loop = true;			//Bool used to skip first cycle of for loop because list is circular
		for(cur = this->Head; (cur!=this->Head) || (loop); cur=cur->next)	//Sorted insert
		{
			loop = false;
			if(ex < cur->exponent)		//if exponent of node being inserted is less than current node
										//being examined in the list
			{
				if(cur->next == this->Head)			//if next item is head node being inserted is smallest item therefore insert node
				{									//and complete circular link
					cur2 = new Node;
					if(cur2 == NULL)
						throw 2;
					cur2->exponent = ex;
					cur2->coefficient = co;
					cur->next = cur2;
					cur2->next = this->Head;
					break;
				}
			}
			else if(ex==cur->exponent)		//if exponent of node being inserted is equal to current node being examined add coefficients
			{
				cur->coefficient += co;
				if(cur->coefficient == 0)  //If addition of coefficients results in coefficient being equal to 0
				{
					if(cur == cur->next)	//If list only has one node make it an empty list
					{
						delete cur;
						cur = NULL;
						this->Head = NULL;
					}
					else	//if list has more than one node
					{
						cur2= cur;					//delete node from list and complete circular link
						while(cur2->next != cur)
							cur2 = cur2->next;
						cur2->next = cur->next;
						if(cur == this->Head)
							this->Head = cur2->next;
						delete cur;					
						cur = NULL;
					}
				}
				break;
			}
			else if(ex > cur->exponent)	//if exponent of node being inserted is greater than current node being examined insert node
			{							//and complete circular link
				cur2 = new Node;
				if(cur2 == NULL)
					throw 3;
				cur2->exponent = ex;
				cur2->coefficient = co;
				cur2->next = cur;
				Node *temp = cur;
				while(temp != cur->next)
					cur = cur->next;
				cur->next = cur2;
				if(temp == this->Head)
					this->Head = cur2;
				break;
			}
		}

	}
	catch(int)		//Memory allocation exception
	{
		cout<<"Cannot allocate memory!\n";
		this->Head = NULL;
		return *this;
	}
	return *this;
}
Poly Poly::operator+(const Poly& orig)const		//Addition Overload
{
	Poly sum(orig);				//Copy RHS into a new poly object called sum
	Node *cur = this->Head;
	bool loop = true;			//bool to bypass first loop
	if(this->Head == NULL)
		return sum;
	for(cur = this->Head; (cur!=this->Head) || (loop); cur=cur->next)
	{
		loop = false;		
		sum.insertPoly(cur->coefficient, cur->exponent);	//insert each node of LHS into sum using sorted insert
	}
	return sum;
}
Poly Poly::operator*(const Poly& orig)const		//Multiplication Overload
{
	Poly product;		//list to store the product of LHS and RHS
	bool loop = true;
	bool loop2 = true;
	for(Node *cur = this->Head; (cur!=this->Head) || (loop); cur=cur->next)	//Transverse LHS list
	{
		loop = false;
		for(Node *cur2 = orig.Head; (cur2!=orig.Head) || (loop2); cur2=cur2->next)	//Transvers RHS list
		{
			loop2 = false;
			product.insertPoly(cur->coefficient*cur2->coefficient, cur->exponent + cur2->exponent);	 //Multiply each LHS node with entire RHS list and insert into product	
		}
		loop2 = true;
	}
	return product;
}
Poly Poly::operator/(const Poly& orig)const		//Division Overload
{
	Poly quotient;			//list to store the quotient of LHS/RHS
	Poly dividend = *this;	//Copy LHS dividend to new list called dividend
	while(true)
	{
		Poly qTerm;
		//this operation returns an empty list if dividend is null
		//the dividend is decremented by an iteration of the quotient (qterm) * divisor
		//eventually the dividend will be less than the divisor upon which it will return quotient
		//further each iteration of qterm is accumulated in the quotient
		//also note when dividend is less than the divisor, this means dividend is now the remainder
		if((dividend.Head ==NULL)||(orig.Head->exponent > dividend.Head->exponent))
		{
			bool loop = true;
			for(Node *cur = quotient.Head;(quotient.Head!=NULL) && ((cur!=quotient.Head) || (loop)); cur = cur->next)
			{
				loop = false;
				cur->coefficient = -cur->coefficient;
			}
			return quotient;
		}
		qTerm.insertPoly(-(dividend.Head->coefficient/orig.Head->coefficient), dividend.Head->exponent-orig.Head->exponent);
		quotient = quotient + qTerm;				//quotient is accumulated
		dividend = (dividend + (qTerm * orig));		//dividend is decremented with every successfull division iteration
	}
}
Poly Poly::operator%(const Poly& orig)const		//Modulus Operator Overload
{
	//this function is exactly the same as the division operation except it returns the decremented dividend instead
	//of the quotient
	Poly quotient;
	Poly dividend = *this;
	while(true)
	{
		Poly qTerm;

		if((dividend.Head ==NULL)||(orig.Head->exponent > dividend.Head->exponent))
		{
			bool loop = true;
			for(Node *cur = quotient.Head;(quotient.Head!=NULL) && ((cur!=quotient.Head) || (loop)); cur = cur->next)
			{
				loop = false;
				cur->coefficient = -cur->coefficient;
			}
			return dividend;
		}
		qTerm.insertPoly(-(dividend.Head->coefficient/orig.Head->coefficient), dividend.Head->exponent-orig.Head->exponent);
		quotient = quotient + qTerm;
		dividend = (dividend + (qTerm * orig));
	}
}
Poly& Poly::operator=(const Poly& orig)		//Assignment Operator Overload
{
	//This function is a slightly modified version of the copy constructor
	try
	{
		if(orig.Head==NULL)
		{
			this->Head=NULL;
		}
		else
		{
			Node *cur, *cur2;
			this->Head = NULL;
			this->Head = new Node;
			if(this->Head == NULL)
				throw 1;
			cur2 = this->Head;
			cur = orig.Head;
			bool loop = true;
			for(cur = orig.Head; (cur!=orig.Head) || (loop); cur=cur->next)
			{
				loop = false;
				cur2->coefficient = cur->coefficient;
				cur2->exponent = cur->exponent;	
				if(cur->next==orig.Head)
					cur2->next = this->Head;
				else
				{
					cur2->next = new Node;
					if(cur2->next == NULL)
						throw 2;
					cur2 = cur2->next;
				}
			}
		}
	}
	catch(int)
	{
		cout<<"Cannot allocate memory!\n";
		this->Head = NULL;
		return *this;
	}
	return *this;
}


//End Polynomial Implementation
//End file