////////////////////////////////////////////////////////
// ECE 2574, Homework 4, Alazar Hailemariam
//
// File name:   bank.cpp
// Description: Client #1 to test ADT mapclass module (ECE2574 HW4),
//				also implements file I/O 
// Date:        11/11/2011
//
#include"simulate.h"
#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		if(argc != 3)
			throw 1;
		ifstream inFile;
		ofstream outFile;
		inFile.open(argv[1]);
		if(inFile.fail())
			throw 2;
		outFile.open(argv[2]);
		simulate simulation;
		simulation.simulateProgram(inFile, outFile);
	}
	catch(int e)
	{
		if(e==1)
			cout<<"Error!  I/O files not provided on the command line.\n";
		if(e==2)
			cout<<"Error!  Input file does not exist.\n";
	}
	system("pause");
}