//////////////////////////////////////////////////////// 
// ECE 2574, Homework 4, Alazar Hailemariam
// 
// File name:   simulate.h 
// Description: Header file for the ADT simulate class 
// Date:        11/11/2011  
// 
#ifndef SIMULATE_H_
#define SIMULATE_H_

#include<iostream>
#include<queue>
#include<list>
#include<iomanip>
#include<fstream>

using namespace std;


typedef double eventItem;
//The struct holds two values because an arrival event needs arrival time and transaction time
//However the same struct is used to push departures into event list
//Departures and differentiated from arrivals because the arrival value for a departure event is
//allways set to -1, a check is made on this value to differentiate between the two events in the program
struct events					
{
	eventItem arrival;
	eventItem departure;
};

class simulate
{
	friend ostream& operator<<(ostream& out, simulate& a);
public:
	//Default Constructor
	simulate();	
	//function which carries out entire operation
	void simulateProgram(ifstream& inFile, ofstream& outFile);
private:
	//Function to process arrivals
	void processArrival(events a, ifstream& infile, ofstream& outFile, list<events>& eventList, queue<events>& bankQueue);
	//Function to process departures
	void processDeparture(events newEvent, ofstream& outFile, list<events>& eventList, queue<events>& bankQueue);
	//List to store values for calculation of maximum and average time in bank
	list<events> maxavgTime();
	//Value to hold the current time
	eventItem currentTime;
	//Queue to hold arrival information later to be used in calculation of maximum and average times
	queue<events> arrivals;
	//Queue to hold departure information later to be used in calculation of maximum and average times
	queue<eventItem> departures;
	//Value to store number of customers
	int customers;
	//Value used to store the maximum length of line
	eventItem maxPeople;
	//Value used to store the maximum number of events
	eventItem numEvents;
	//Value used to store the sum of number of people in the queue at each moment 
	int totalLine;
};
#endif
