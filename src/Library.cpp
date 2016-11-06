//==================================================================================
// NLDM class definition
// =====================
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
// Liblist.cpp : 
// Definition of all the functions used in Liblist class.

#include "../include/Library.h"

//Default constructor
Lib::Lib() {
	this->name = "";
	this->capacitance = 0;
	this->delayInputSlew.clear();
	this->delayOutputLoad.clear();
	this->slewInputSlew.clear();
	this->slewOutputLoad.clear();
	this->cellDelay.clear();
	this->outputSlew.clear();
}

//Destructor
Lib::~Lib() {

}

//return the cell name
string Lib::getCellName() const {
	return name;
}

//return the capacitance
double Lib::getCapacitance() const {
	return Lib::capacitance;
}

//return the input slews from the delay section
vector<double> Lib::getDelayInputSlew() const {
	return delayInputSlew;
}

//return the input slews from the slew section
vector<double> Lib::getSlewInputSlew() const {
	return slewInputSlew;
}

//return the input delay from the delay section
vector<double> Lib::getDelayOutputLoad() const {
	return delayOutputLoad;
}

//return the input delay from the slew section
vector<double> Lib::getSlewOutputLoad() const {
	return slewOutputLoad;
}

//return the cell delay
vector<vector<double> > Lib::getCellDelays() const {
	return cellDelay;
}

//return the cell slews
vector<vector<double> > Lib::getCellSlews() const {
	return outputSlew;
}

//return the delay element from the LUT
double Lib::getDelayElem(short int i, short int j) const{
	return this->cellDelay[i][j];
}

//return the slew element from the LUT
double Lib::getSlewElem(short int i, short int j) const {
	return this->outputSlew[i][j];
}

//set the cell name
void Lib::setCellName(string name) {
	Lib::name = name;
}

//set the load capacitance
void Lib::setCapacitance(double cap) {
	Lib::capacitance = cap;
}

//set the slews
void Lib::setDelayInputSlew(vector<double> Slew) {
	Lib::delayInputSlew = Slew;
}

//set the loads
void Lib::setDelayOutputLoad(vector<double> Load) {
	Lib::delayOutputLoad = Load;
}

//set the slews
void Lib::setSlewInputSlew(vector<double> Slew) {
	Lib::slewInputSlew = Slew;
}

//set the loads
void Lib::setSlewOutputLoad(vector<double> Load) {
	Lib::slewOutputLoad = Load;
}

//add the cell delays to the celay vector
void Lib::setCellDelays(vector<double> delay) {
	Lib::cellDelay.push_back(delay);
}

//add the cell slew to the celay vector
void Lib::setCellSlews(vector<double> delay) {
	Lib::outputSlew.push_back(delay);
}

//empty the delay vector
void Lib::setClearDelay() {
	Lib::cellDelay.clear();
}

//empty the slew vector
void Lib::setClearSlew() {
	Lib::outputSlew.clear();
}

//overloaded constructor
LibDetails::LibDetails() {
	this->capacitiveLoadUnit = "";
	this->currentUnit = "";
	this->leakagePowerUnit = "";
	this->nomProcess = 0;
	this->nomTemperature = 0;
	this->nomVoltage = 0;
	this->pullupResistanceUnit = "";
	this->timeUnit = "";
	this->voltageUnit = "";
	this->NAND = -1;
	this->NOR = -1;
	this->AND = -1;
	this->OR = -1;
	this->XOR = -1;
	this->INV = -1;
	this->BUF = -1;
}

//return the library position of the cell type
short int LibDetails::getNandPos() const {
	return this->NAND;
}

//return the library position of the cell type
short int LibDetails::getAndPos() const {
	return this->AND;
}

//return the library position of the cell type
short int LibDetails::getNorPos() const {
	return this->NOR;
}

//return the library position of the cell type
short int LibDetails::getOrPos() const {
	return this->OR;
}

//return the library position of the cell type
short int LibDetails::getXorPos() const {
	return this->XOR;
}

//return the library position of the cell type
short int LibDetails::getInvPos() const {
	return this->INV;
}

//return the library position of the cell type
short int LibDetails::getBufPos() const {
	return this->BUF;
}

//return the library position of the cell type
void LibDetails::setNandPos(short int pos) {
	if (this->NAND == -1)
		this->NAND = pos;
}

//return the library position of the cell type
void LibDetails::setNorPos(short int pos) {
	if (this->NOR == -1)
		this->NOR = pos;
}

//return the library position of the cell type
void LibDetails::setAndPos(short int pos) {
	if (this->AND == -1)
		this->AND = pos;
}

//return the library position of the cell type
void LibDetails::setOrPos(short int pos) {
	if (this->OR == -1)
		this->OR = pos;
}

//return the library position of the cell type
void LibDetails::setXorPos(short int pos) {
	if (this->XOR == -1)
		this->XOR = pos;
}

//return the library position of the cell type
void LibDetails::setInvPos(short int pos) {
	if (this->INV == -1)
		this->INV = pos;
}

//return the library position of the cell type
void LibDetails::setBufPos(short int pos) {
	if (this->BUF == -1)
		this->BUF = pos;
}
