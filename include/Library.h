//==================================================================================
// Library class definition
// ========================
//
// @author: Sriram Kuppahalli Venkatesha
// @email:  kuppa010@umn.edu
// =====================================
//
// Description:
// ============
//
// This is the class definition for modelling NLDM components.
//
//==================================================================================
//Liblist.h
//Liblist class definition.

#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
#include "vector"

using namespace std;

#ifndef LIBRARY_H
#define LIBRARY_H

class LibDetails {
public:
	//Default constructor
	LibDetails();

	//Accessor
	short int getNandPos() const;
		//Return NAND position

	short int getNorPos() const;
		//Return NOR position

	short int getAndPos() const;
		//Return AND position

	short int getOrPos() const;
		//Return OR position

	short int getXorPos() const;
		//Return XOR position

	short int getInvPos() const;
	//Return INV position

	short int getBufPos() const;
	//Return BUF position

	//Mutators
	void setNandPos(short int);
		//Set NAND lib position

	void setNorPos(short int);
		//Set NAND lib position

	void setAndPos(short int);
		//Set NAND lib position

	void setOrPos(short int);
		//Set NAND lib position

	void setXorPos(short int);
		//Set NAND lib position

	void setInvPos(short int);
		//Set NAND lib position

	void setBufPos(short int);
		//Set NAND lib position

private:
	string timeUnit;
	string leakagePowerUnit;
	string voltageUnit;
	string currentUnit;
	string pullupResistanceUnit;
	string capacitiveLoadUnit;
	float nomProcess;
	float nomTemperature;
	float nomVoltage;
	short int NAND, NOR, AND, OR, XOR, INV, BUF;
};

class Lib {
public:
	//Default constructor
	Lib();

	////Overloaded constructor
	//Lib(double capacitance, vector<vector<double> > cellDelay, vector<vector<double>> outputSlew);

	//Destructor
	~Lib();

	//Accessor
	string getCellName() const;
		//Return cell name

	double getCapacitance() const;
		//Return capaciatance

	vector<double> getDelayInputSlew() const;
		//Return delay input slews

	vector<double> getSlewInputSlew() const;
		//Return slew input slews

	vector<double> getDelayOutputLoad() const;
		//Return delay output load

	vector<double> getSlewOutputLoad() const;
		//Return slew output load

	vector<vector<double> > getCellDelays() const;
		//Return the cell delays

	vector<vector<double> > getCellSlews() const;
		//Return the cell slews

	double getDelayElem(short int, short int) const;
		//return an eleent from the 2D vector

	double getSlewElem(short int, short int) const;
		//return an element from the 2D vector

	//Mutator
	void setCellName(string);
		//Set cell name

	void setCapacitance(double);
		//set the capacitance

	void setDelayInputSlew(vector<double>);
		//Set the input delay values

	void setDelayOutputLoad(vector<double>);
		//Set the input load values

	void setSlewInputSlew(vector<double>);
		//Set the input delay values

	void setSlewOutputLoad(vector<double>);
		//Set the input load values

	void setClearDelay();
		//Clear the delays

	void setClearSlew();
		//Clear the slews

	void setCellDelays(vector<double>);
		//Set the cell delays

	void setCellSlews(vector<double>);
		//Set the cell slews

private:
	string name;
	double capacitance;
	vector<double> delayInputSlew;
	vector<double> delayOutputLoad;
	vector<double> slewInputSlew;
	vector<double> slewOutputLoad;
	vector<vector<double> > cellDelay;
	vector<vector<double> > outputSlew;

};

#endif //LIBRARY_H
