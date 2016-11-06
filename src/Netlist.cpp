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
// Netlist.cpp : 
// Definition of all the functions used in Netlist class.

#include "../include/Netlist.h"

//Default constructor
Node::Node() {
	this->Name = "";
	this->Type = "";
	this->IsPort = false;
	this->PortDirection = false;
	this->InputCount = 0;
	this->OutputCount = 0;
	this->Inputs.clear();
	this->Outputs.clear();
	this->Fanin.clear();
	this->Fanout.clear();
	this->libPosition = -1;
	this->oCL = 0;
	this->oDelay = 0;
	this->oTau = 0;
	this->iDelay.clear();
	this->iTau.clear();
	this->slack = 0;
	this->internalDelay.clear();
	this->internalTau.clear();
}

//Overloaded constructor
Node::Node(string Name, string Type, bool IsPort, bool PortDirection, int InputCount, int OutputCount, vector<string> Inputs, vector<string> Outputs) {
	this->Name = Name;
	this->Type = Type;
	this->IsPort = IsPort;
	this->PortDirection = PortDirection;
	this->InputCount = InputCount;
	this->OutputCount = OutputCount;
	this->Inputs = Inputs;
	this->Outputs = Outputs;
}

//get the component name
string Node::getName() const {
	return Name;
}

//get the component type
string Node::getCompType() const {
	return Type;
}

//get the number of inputs
int Node::getInputCount() const {
	return this->Inputs.size();
}

//get the number of outputs
int Node::getOutputCount() const {
	return this->Outputs.size();
}

//get component type (port/gate)
bool Node::getIsPort() const {
	return IsPort;
}

//get port direction (in/out)
bool Node::getPortDirection() const {
	return PortDirection;
}

//get the fanin vector
vector<string> Node::getInputs() const {
	return Inputs;
}

//get the fanout vector
vector<string> Node::getOutputs() const {
	return Outputs;
}

//return the vector containing location of the fanins
vector<int> Node::getFanin() const {
	return Fanin;
}

//return the vector containing location of the fanouts
vector<int> Node::getFanout() const {
	return Fanout;
}

//return the library position to use for lib mapping
short int Node::getLibPosition() const {
	return libPosition;
}

//return the output slew
double Node::getOTau() const {
	return this->oTau;
}

//return the arrival time at the output (or) delay
double Node::getOTime() const {
	return this->oDelay;
}

//return the load capacitance
double Node::getOCL() const {
	return this->oCL;
}

//return the input tau of a particular pin
double Node::getInternalTau(short int loc) const {
	return this->internalTau[loc];
}

//return all the input tau
vector<double> Node::getInternalDelay() const {
	return this->internalDelay;
}

//return the arrival time of the component
vector<double> Node::getArrivalTime() const {
	return this->iDelay;
}

//return the slack of the component
double Node::getSlack() const {
	return this->slack;
}

//return the required arrival time of the component
double Node::getRequiredArrivalTime() const{
	return this->reauiredArrivalRime;
}

//Print the comopnent summary
void Node::printComponent() {
	cout << "Name:" << Node::Name << " "
		<< "Type:" << Node::Type << " "
		<< "Fanin:" << Node::InputCount << " "
		<< "Fanout:" << Node::OutputCount << " ";
	if (Node::Inputs.size() > 0) {
		cout << "InputNets: ";
		for (unsigned int i = 0; i < Node::Inputs.size(); i++) {
			if (i == (Node::Inputs.size() - 1)) {
				cout << Node::Inputs[i] << " ";
			}
			else {
				cout << Node::Inputs[i] << ", ";
			}
		}
	}
	if (Node::Outputs.size() > 0) {
		cout << "OutputNets: ";
		for (unsigned int i = 0; i < Node::Outputs.size(); i++) {
			if (i == (Node::Outputs.size() - 1)) {
				cout << Node::Outputs[i] << " ";
			}
			else {
				cout << Node::Outputs[i] << ", ";
			}
		}
	}
}

//set the name of the component
void Node::setName(string Name) {
	Node::Name = Name;
}

//set the type of the component
void Node::setType(string Type) {
	Node::Type = Type;
}

//set the numeber of inputs
void Node::setInputCount(int InputCount) {
	Node::InputCount = InputCount;
}

//set the numeber of outputs
void Node::setOutputCount(int OutputCount) {
	Node::OutputCount = OutputCount;
}

//set the component type (port - 1/gate - 0)
void Node::setCompType(bool Type) {
	Node::IsPort = Type;
}

//set the port direction
void Node::setPortDir(bool PortDir) {
	Node::PortDirection = PortDir;
}

//set the fanins
void Node::setInputs(vector<string> Inputs) {
	Node::Inputs = Inputs;
}

//set the fanouts
void Node::setOutputs(vector<string> Outputs) {
	Node::Outputs = Outputs;
}

//add fanin location to the fanin vector
void Node::setUpdateFanin(int Fanin) {
	Node::Fanin.push_back(Fanin);
}

//add fanout location to the fanout vector
void Node::setUpdateFanout(int Fanout) {
	Node::Fanout.push_back(Fanout);
}

//set the lib positoin corresponding to this component
void Node::setLibPosition(short int i) {
	Node::libPosition = i;
}

//set load capacitance
void Node::setOCL(double CL) {
	this->oCL = CL;
}

//set the output slew of the component
void Node::setOTau(double tau) {
	this->oTau = tau;
}

//set the outpout delay
void Node::setODelay(double dly) {
	this->oDelay = dly;
}

//list the tau corresponding to the fanins
void Node::setTau(double tau) {
	this->iTau.push_back(tau);
}

//list the delays corresponding to the fanins
void Node::setArrivalTime(double aT) {
	this->iDelay.push_back(aT);
}

//list the intenal tau corresponding to the fanins
void Node::setInternalTau(double tau) {
	this->internalTau.push_back(tau);
}

//list the intenal delay corresponding to the fanins
void Node::setInternalDelay(double delay) {
	this->internalDelay.push_back(delay);
}

//set the slack of the component
void Node::setSlack(double slack) {
	this->slack = slack;
}

//set he required arrival time
void Node::setRequiredArrivalTime(double rat) {
	this->reauiredArrivalRime = rat;
}

//destructor
Node::~Node() {

}
