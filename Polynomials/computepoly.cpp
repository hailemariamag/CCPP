////////////////////////////////////////////////////////
// ECE 2574, Homework 2, Alazar Hailemariam
//
// File name:   computepoly.cpp
// Description: Client #1 to test ADT PolynomialP module (ECE2574 HW2),
//				also implements file I/O 
// Date:        10/05/2011
//
#include<iostream>
#include"PolytermsP.h"
#include<fstream>

#include<string>

using namespace std;

void main(int argc, char *argv[])
{
	int numX = 0;				//Used to increment forloop for each 'X' found
								//because 'X' signifies the end of one list and the beginning of another
								//a boolean firstpoly is set to false which shifts the poly being inserted to the second list
	int ex, co;				//Initialize exponent and coefficients to be inserted into a list
	bool insert = true;		//Used to switch between inserting exponent and coefficient
	bool firstPoly = true;	//Used to switch between inserting into the first list and second list
	ifstream inFile;		//Streams for I/O files
	ofstream outFile;
	Poly first, second;		//Initialize empty lists for input from file
	char* line = new char;	//To stream from file using get() function
	
	try
	{
		if(argc!=3)			//Check correct number of arguments for command line run and throw exception if incorrect
			throw 5;		
		inFile.open(argv[1]);	//open input file
		if(inFile.fail())		//throw exception if open input file fails
			throw 2;
		else
		{
			for(int x = 0; x<=numX; x++)	//For loop for inserting into list because while loop breaks each time an 'X' is found
			{								//before break however bool to input into second list is set and numX incremented
				inFile.get(*line);			
				while(!inFile.eof())
				{
					if(*line == 'X')	//Check for 'X'
					{
						firstPoly = false;		//Inserting into first list is done
						numX++;					//Increment numX
						break;
					}
					else if(*line == ' ' || *line == '\n');	//If ' ' or '\n' found do nothing
					else
					{					//condition of numbers and '-' 
						if(insert)		//check flag to insert into coefficient or exponent
						{
							if(*line == '-')	//if '-' found change coefficient into negative number		
							{
								inFile.get(*line);
								co = atoi(line);
								co = -co;
							}
							else
								co = atoi(line);
							if(co==0)				//if trying to insert 0 as a coefficient throw exception
								throw 3;
							insert = false;			//clear flag to insert next number into exponent
						}
						else
						{
							if(*line == '-')		//if trying to insert negative exponents throw exception
							{
								throw 4;
							}
							else
								ex = atoi(line);
							if(firstPoly)					//insert into lists
								first.insertPoly(co,ex);
							else if(!firstPoly)
								second.insertPoly(co,ex);
							insert = true;			//set flag to insert next number into coefficient
						}
					}
					inFile.get(*line);
				}
			}
		}
		if(numX!=3)			//Check for correct number of 'XXX'
			throw 1;
		outFile.open(argv[2]);
		if(!outFile.fail())						//Carry out operations, format output, and output results
		{
			outFile<<first + second;
			outFile<<"YYY\n";
			outFile<<first * second;
			outFile<<"ZZZ\n";
			outFile<<first  /second;
			outFile<<"RRR\n";
			outFile<<first % second;
		}

	}
	catch(int e)		//Catch for various exceptions
	{
		if(e == 1)
			cout<<"Bad input file!"<<endl;
		else if(e == 2)
			cout<<"Input file does not exist!"<<endl;
		else if(e == 3)
			cout<<"Cannot input 0 as a coefficient!"<<endl;
		else if(e == 4)
			cout<<"Exponent cannot be negative!"<<endl;
		else if(e == 5)
			cout<<"Command requires <input_file> <output_file>"<<endl;
	}

	system("pause");
}
//end client test file