//////////////////////////////////////////////////////// 
// ECE 2574, Homework 4, Alazar Hailemariam
// 
// File name:   mapclass.cpp
// Description: Implementation file for the ADT simulate
// Date:        11/11/2011  
//
#include<iostream>
#include"simulate.h"

using namespace std;

//typedef eventItem double;


simulate::simulate()	//Default Constructor
{
	customers = 0;
	maxPeople = 0;
	currentTime = 0;
	numEvents = 0;
	totalLine = 0;
}
//This function pushes the first person into the event list and starts the while loop
void simulate::simulateProgram(ifstream& inFile, ofstream& outFile)
{
	list<events> eventList;				//Initialize variables
	queue<events> bankQueue;			//The eventlist always contains the event which happens first at fron and the event
	events cur;							//Which happens last in the back
	inFile>>cur.arrival;
	inFile>>cur.departure;
	try									//Exception for invalid file
	{
		if(cur.arrival >1000 || cur.arrival < 0 || cur.departure > 1000 || cur.departure <0)
			throw 1;
	}
	catch(int)
	{
		cout<<"Error!  Invalid input file detected.\n";
	}
	outFile<<"t = "<<currentTime<<":  Simulation begins\n";
	eventList.push_front(cur);
	while(!eventList.empty())
	{
		events newEvent = eventList.front();
		if(newEvent.arrival == -1)					//Check to see whether the next event is an arrival or departure			
			processDeparture(newEvent, outFile, eventList, bankQueue);		
		else
		{
			processArrival(newEvent, inFile, outFile, eventList, bankQueue);	//If arrival increment customers
			customers++;
		}
	}
	list<events> maxavg = maxavgTime();
	outFile<<"\n\nTotal number of customers = "<<customers<<endl;
	outFile<<"Average amount of time in the bank = "<<fixed<<setprecision(1)<<maxavg.front().arrival<<" minutes\n";
	outFile<<"Maximum amount of time in the bank = "<<fixed<<setprecision(1)<<maxavg.front().departure<<" minutes\n";
	outFile<<"Average amount of time waiting = "<<fixed<<setprecision(1)<<maxavg.back().arrival<<" minutes\n";
	outFile<<"Maximum amount of time waiting = "<<fixed<<setprecision(1)<<maxavg.back().departure<<" minutes\n";
	outFile<<"Average length of line = "<<totalLine/eventItem(numEvents)<<" customers\n";
	outFile<<"Maximum length of line = "<<maxPeople<<" customers\n";
}
//This function processes arrivals
void simulate::processArrival(events newEvent, ifstream& inFile, ofstream& outFile, list<events>& eventList, queue<events>& bankQueue)
{
	numEvents++;
	bool atFront = bankQueue.empty();	//Check band queue to check if its empty, if empty a departure event will be generated
	eventList.pop_front();
	bankQueue.push(newEvent);
	totalLine += bankQueue.size();		
	if(bankQueue.size()>maxPeople)		//Check if the line got bigger
		maxPeople = bankQueue.size();
	arrivals.push(newEvent);
	currentTime = newEvent.arrival;
	if(atFront)						
	{
		events cur;
		cur.arrival = -1;
		cur.departure = newEvent.arrival+newEvent.departure;
		eventList.push_front(cur);
		departures.push(cur.departure);
	}
	if(!inFile.eof())						//After processing arrival, generate the next arrival and place it in the appropriate 
	{										//position on the event list
		events cur;
		inFile>>cur.arrival;
		inFile>>cur.departure;
		try									//Exception for invalid input file
		{
			if(cur.arrival >1000 || cur.arrival < 0 || cur.departure > 1000 || cur.departure <0)
				throw 1;
		}
		catch(int)
		{
			cout<<"Error!  Invalid input file detected.\n";
			system("pause");
			exit(1);
		}
		if(cur.arrival <= eventList.front().departure)
			eventList.push_front(cur);
		else
			eventList.push_back(cur);
	}
	outFile<<"t = "<<left<<setw(2)<<currentTime<<": Arrival event\n";
}
//This function processes departures
void simulate::processDeparture(events newEvent, ofstream& outFile, list<events>& eventList, queue<events>& bankQueue)
{
	numEvents++;
	bankQueue.pop();			//pop the bank queue for the person leaving
	totalLine += bankQueue.size();
	eventList.pop_front();
	currentTime = newEvent.departure;	//update time 
	if(!bankQueue.empty())		//If the queue is not empty generate new departure event
	{
		events cur;
		cur.arrival = -1;
		cur.departure = currentTime + bankQueue.front().departure;
		if(eventList.empty() || cur.departure < eventList.front().arrival)
			eventList.push_front(cur);
		else
			eventList.push_back(cur);
		departures.push(cur.departure);
	}
	outFile<<"t = "<<left<<setw(2)<<currentTime<<": Departure event\n";
}
//This function calculates the average and maximum time in that bank
//Returns a struct events in where arrival contains average time and departure contains maximum time
list<events> simulate::maxavgTime()
{
	list<events> calcTimes;		//List to contain max time in bank and max wait time and average wait and in bank time
	events averageTime;			//event struct to containg max time in bank and average time in bank
	averageTime.arrival = 0;	
	averageTime.departure = 0;
	events waitTime;			//event struct to containg max wait time and average wait time
	waitTime.arrival = 0;
	waitTime.departure = 0;
	while(!departures.empty())	//Loop to calculate values
	{
		averageTime.arrival += departures.front() - arrivals.front().arrival;
		waitTime.arrival += departures.front() - arrivals.front().arrival - arrivals.front().departure;
		if(averageTime.departure < (departures.front() - arrivals.front().arrival) )
			averageTime.departure = departures.front() - arrivals.front().arrival;
		if(waitTime.departure < (departures.front() - arrivals.front().arrival - arrivals.front().departure) )	
			waitTime.departure = departures.front() - arrivals.front().arrival - arrivals.front().departure;
		departures.pop();
		arrivals.pop();
	}
	averageTime.arrival /= customers;
	waitTime.arrival /= customers;
	calcTimes.push_front(averageTime);
	calcTimes.push_back(waitTime);
	return calcTimes;		//Return a list containing the 4 calculations
}
