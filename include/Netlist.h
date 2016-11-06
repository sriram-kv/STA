//==================================================================================
// Node class definition
// =====================
//
// @author: Sriram Kuppahalli Venkatesha
// @email:  kuppa010@umn.edu
// =====================================
//
// Description:
// ============
//
// This is the class definition for modelling Gate/Port components.
//
//==================================================================================
//Netlist.h
//Netlist class definition and include files.

#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
#include "vector"

using namespace std;

#ifndef NETLIST_H
#define NETLIST_H

class Node {
public:
	//Default Constructer
	Node();

	//Overloaded Constructer
	Node(string Name, string Type, bool IsPort, bool PortDirection, int InputCount, int OutputCount, vector<string> Inputs, vector<string> Outputs);
	
	//Destructor
	~Node();

	//Accessor Functions
	string getName() const;
		//Name of the component

	string getCompType() const;
		//Component type: Port/Gate

	int getInputCount() const;
		//Number of input ports. 0 for Input ports

	int getOutputCount() const;
		//Number of output ports. 0 for Output ports

	bool getIsPort() const;
		//Port/Gate: 1-Port; 0-Gate

	bool getPortDirection() const;
		//Port direction. 1- Input; 0- Output

	vector<string> getInputs() const;
		//Return all Inputs

	vector<string> getOutputs() const;
		//Return all Outputs

	vector<int> getFanin() const;
		//Return Fanin

	vector<int> getFanout() const;
		//Return Fanout

	void printComponent();
		//prints the componet object

	short int getLibPosition() const;
		//get the lib position

	double getOTau() const;
		//get the output slew

	double getOTime() const;
		//get the arrival time at the output

	double getOCL() const;
		//get the output capacitance

	double getInternalTau(short int) const;
		//get cell port slew

	vector<double> getInternalDelay() const;
		//get internal delay

	vector<double> getArrivalTime() const;
		//get the arrival time vector

	double getSlack() const;
		//get the gate slack

	double getRequiredArrivalTime() const;
		//set the required arrival time

	//Mutator Functions
	void setName(string);
		//Set name of the component

	void setType(string);
		//Set type: Port/Gate

	void setInputCount(int);
		//Set number of input ports. 0 for Input ports

	void setOutputCount(int);
		//Set of output ports. 0 for Output ports

	void setCompType(bool);
		//Port/Gate: 1-Port; 0-Gate

	void setPortDir(bool);
		//Set port direction. 1- Input; 0- Output

	void setInputs(vector<string>);
		//Set all the Inputs

	void setOutputs(vector<string>);
		//Set all the outputs

	void setUpdateFanin(int);
		//Update the fanin

	void setUpdateFanout(int);
		//Update the fanout

	void setLibPosition(short int);
		//Set the library position

	void setOCL(double);
		//set the output capacitance

	void setTau(double);
		//set iTau;
	
	void setOTau(double);
		//set the output capacitance

	void setODelay(double);
		//Set the input slew

	void setArrivalTime(double);
		//set the arrival time

	void setInternalTau(double);
		//set cell port slew

	void setInternalDelay(double);
		//set cell port delay

	void setSlack(double);
		//set the gate slack

	void setRequiredArrivalTime(double);
		//set the required arrival time
	
	//As part of code optimization, I'm declaring the following variables as public
	vector<string> Inputs;
	int InputCount;
private:
	string Name;
	string Type;
	bool IsPort;
	bool PortDirection;
	int OutputCount;
	vector<string> Outputs;
	vector<int> Fanin;
	vector<int> Fanout;
	short int libPosition;
	double oCL, oTau, oDelay;
	double slack, reauiredArrivalRime;
	vector<double> iTau, iDelay;
	vector<double> internalTau, internalDelay;

};

#endif // !NETLIST_H
