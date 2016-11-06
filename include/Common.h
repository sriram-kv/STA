//==================================================================================
// Static Timing Analyzer common function
// ======================================
//
// @author: Sriram Kuppahalli Venkatesha
// @email:  kuppa010@umn.edu
// =====================================
//
// Description:
// ============
//
// This file contains the definition of all the functions used in the parser.cpp
// Some function uses Node and Library classes.
// No need to inherit the class since data is only read or set to the actual 
// Node/Library objects
//
//==================================================================================
#include "string"
#include "iomanip"
#include "Dfs.h"
#include "Netlist.h"
#include "Library.h"
#include "algorithm"

using namespace std;

//Macro to remove whitespaces
#define remove_space(String) String.erase(remove_if(String.begin(), String.end(), ::isspace), String.end());

//extract the net name of the port
string getPortName(string port) {
	int start, end;
	string input_port;

	start = port.find_first_of('(');
	end = port.find_last_of(')');

	input_port = "";

	input_port = port.substr(start + 1, (end - start - 1));

	return input_port;
	//return name;
}

//get the name of the net an output is connected to.
vector<string> getOutportName(string port) {
	int start;
	vector<string> outport_name;
	start = port.find_first_of('=');

	outport_name.clear();
	outport_name.push_back(port.substr(0, start));

	return outport_name;
	//return name;
}

//get the gate name
string getCompName(string port) {
	int start, end;

	start = port.find_first_of('=');
	end = port.find_last_of('(');

	return port.substr(start + 1, (end - start - 1));
	//return name;
}

//get the fanins; comma separated
vector<string> getPortNets(string port) {
	string inputs;
	string segment;

	vector<string> inPorts;
	int start, end;
	start = port.find_first_of('(');
	end = port.find_last_of(')');
	inputs = port.substr(start + 1, end - start - 1);
	//cout << inputs << endl;
	istringstream stream(inputs);

	while (std::getline(stream, segment, ',')) {
		inPorts.push_back(segment);
		//cout << segment << endl;
	}
	return inPorts;
}

//parse the string to a vector of double datatype
vector<double> getValues(string text) {
	char delimiter = ',';
	vector<double> values;
	values.clear();
	string acc = "";
	string line = text;
	for (unsigned int i = 0; i < line.size(); i++) {
		if (line[i] == delimiter) {
			values.push_back(stod(acc));
			acc = "";
		}
		else
			acc += line[i];
	}
	return values;
}

//return the library position of a cell
int libPosition(string name, LibDetails *lib) {
	if (name == "NAND")
		return lib->getNandPos();
	else if (name == "NOR")
		return lib->getNorPos();
	else if (name == "AND")
		return lib->getAndPos();
	else if (name == "OR")
		return lib->getOrPos();
	else if (name == "XOR")
		return lib->getXorPos();
	else if (name == "INV")
		return lib->getInvPos();
	else if (name == "BUF" || name == "BUFF")
		return lib->getBufPos();
	else
		return -1;
}

//Netlist parser logic
//   1. chech each line, remove whitespace
//   2. identify input/output/gate while reading
//   3. set the fanin, fanout, type, etc of the corresponding component
void parseNetlist(string netlist, vector<Node>& component, LibDetails *lib) {
	string line, compName;
	fstream net(netlist);
	Node parser;
	int valid = 0;
	vector<string> terminalPort;
	terminalPort.clear();

	if (net.is_open()) {
		while (std::getline(net, line)) {
			remove_space(line);
			// ignore comments in the netlist
			if ((line[0] != '#') && (!line.empty())) {
				//identify and set the default attributes of the inputs
				if (line.find("INPUT") != line.npos) {
					parser.setName("INP-" + getPortName(line));
					parser.setType("Port");
					parser.setCompType(true);
					parser.setPortDir(true);
					parser.setInputCount(1);
					parser.setOutputCount(1);
					terminalPort.push_back(getPortName(line));
					parser.setInputs(terminalPort);
					parser.setOutputs(terminalPort);
					parser.setLibPosition(-1);
					terminalPort.clear();
					valid = 1;
				}
				//identify and set the default attributes of the outputs
				else if (line.find("OUTPUT") != line.npos) {
					parser.setName("OUTP-" + getPortName(line));
					parser.setType("Port");
					parser.setCompType(true);
					parser.setPortDir(false);
					parser.setInputCount(1);
					parser.setOutputCount(1);
					terminalPort.push_back(getPortName(line));
					parser.setInputs(terminalPort);
					parser.setOutputs(terminalPort);
					parser.setLibPosition(-1);
					terminalPort.clear();
					valid = 1;
				}
				//identify and set the calculated attributes of the gates
				else if (line.find("=") != line.npos) {
					terminalPort = getOutportName(line);
					compName = getCompName(line);
					parser.setName(compName + "-" + terminalPort[0]);
					parser.setType("Gate");
					parser.setCompType(false);
					parser.setPortDir(false);
					parser.setInputs(getPortNets(line));
					parser.setOutputs(getOutportName(line));
					parser.setInputCount(parser.getInputCount());
					parser.setOutputCount(parser.getOutputCount());
					if (compName == "NOT" || compName == "INV")
						parser.setLibPosition(libPosition("INV", lib));
					else
						parser.setLibPosition(libPosition(compName, lib));
					valid = 1;
				}
				else {
					cout << "\nSomething wrong with the netlist \n";
					exit(0);
				}
			}
			if (valid == 1) {
				component.push_back(parser);
				//cout << name << " " << compType << " " << inputPortCount << " " << output <<"\n";
				valid = 0;
			}
		}
	}
	else {
		cout << "\nSomething wrong with the netlist \n";
		exit(0);
	}

}

//Library parser logic
//   1. chech each line, remove whitespace
//   2. identify cell, delay&slew LUT reading
void parseLiblist(string& liblist, vector<Lib>& component) {
	string line, cellName;
	fstream lib("nldm.lib");
	Lib parser;
	int valid = 0, cellIsOpen = 0, curlyCount = 0,
		setDelayIndex = 0, setSlewIndex = 0,
		readingDelays = 0, readingSlews = 0;

	vector<double> row;
	row.clear();

	cout << liblist << endl;

	if (lib.is_open()) {
		while (std::getline(lib, line)) {
			remove_space(line);
			// identify the cell name
			if (line.find("cell(") != line.npos) {
				cellName = getPortName(line);
				parser.setCellName(cellName);
				row.clear();
				cellIsOpen = 1;
				valid = 1;
			}

			if (valid == 1) {
			// extract the capacitance
				if (line.find("capacitance:") != line.npos) {
					parser.setCapacitance(stod(line.substr(line.find_first_of(':') + 1, line.find_first_not_of(';') - 1)));
				}

				if (line.find("{") != line.npos)
					curlyCount += 1;
				else if (line.find("}") != line.npos)
					curlyCount -= 1;

				if (curlyCount != 0) {
					if (line.find("capacitance:") != line.npos)
						parser.setCapacitance(stod(line.substr(line.find_first_of(':') + 1, line.size() - 2)));

					//identify delay/slew LUT
					if (line.find("cell_delay") != line.npos) {
						setDelayIndex = 1;
						setSlewIndex = 0;
					}
					else if (line.find("output_slew") != line.npos) {
						setDelayIndex = 0;
						setSlewIndex = 1;
					}

					//parse the indices from string to a vector
					if (line.find("index_1") != line.npos) {
						row = getValues(line.substr(line.find_first_of('"') + 1, line.find_last_of('"')) + ',');

						if (setDelayIndex == 1)
							parser.setDelayInputSlew(row);
						else if (setSlewIndex == 1)
							parser.setSlewInputSlew(row);

						row.clear();
					}

					if (setDelayIndex == 1) {
					//parse the indices from string to a vector
						if (line.find("values") != line.npos) {
							row = getValues(line.substr(line.find_first_of('"') + 1, line.size() - 3));
							parser.setCellDelays(row);
							row.clear();
							readingDelays = 1;
						}
						else if (readingDelays == 1) {
							//take care of the '\' character in the LUT
							if (line.find('\\') != line.npos) {
								row = getValues(line.substr(line.find_first_of('"') + 1, line.size() - 2));
								parser.setCellDelays(row);
								row.clear();
							}
							else if (line.find(';') != line.npos) {
								row = getValues(line.substr(line.find_first_of('"') + 1, line.size()) + ',');
								parser.setCellDelays(row);
								row.clear();
								readingDelays = 0;
							}
						}
					}

					//parse the indices from string to a vector
					if (line.find("index_2") != line.npos) {
						row = getValues(line.substr(line.find_first_of('"') + 1, line.find_last_of('"') - 1) + ',');

						if (setDelayIndex == 1)
							parser.setDelayOutputLoad(row);
						else if (setSlewIndex == 1)
							parser.setSlewOutputLoad(row);

						row.clear();
					}

					if (setSlewIndex == 1) {
						if (line.find("values") != line.npos) {
							row = getValues(line.substr(line.find_first_of('"') + 1, line.size() - 3));
							parser.setCellSlews(row);
							row.clear();
							readingSlews = 1;
						}
						else if (readingSlews == 1) {
							//take care of the '\' character in the LUT
							if (line.find('\\') != line.npos) {
								row = getValues(line.substr(line.find_first_of('"') + 1, line.size() - 2));
								parser.setCellSlews(row);
								row.clear();
							}
							else if (line.find(';') != line.npos) {
								row = getValues(line.substr(line.find_first_of('"') + 1, line.size()) + ',');
								parser.setCellSlews(row);
								row.clear();
								readingSlews = 0;
							}
						}
					}
				}
			}

			if (valid == 1 && curlyCount == 0) {
				component.push_back(parser);
				//cout << name << " " << compType << " " << inputPortCount << " " << output <<"\n";
				valid = 0;
				parser.setClearDelay();
				parser.setClearSlew();
			}
		}
	}
	else {
		cout << "\nSomething wrong with the liberty file \n";
		exit(0);
	}

}

//set the lib position for the cells
void updateLibPosition(vector<Lib>& libs, LibDetails *details) {
	string cellName;

	for (unsigned int i = 0; i < libs.size(); i++) {
		cellName = libs[i].getCellName();
		if (cellName == "NAND2_X1") {
			details->setNandPos(i);
		}
		else if (cellName == "NOR2_X1") {
			details->setNorPos(i);
		}
		else if (cellName == "AND2_X1") {
			details->setAndPos(i);
		}
		else if (cellName == "OR2_X1") {
			details->setOrPos(i);
		}
		else if (cellName == "XOR2_X1") {
			details->setXorPos(i);
		}
		else if (cellName == "INV_X1") {
			details->setInvPos(i);
		}
		else if (cellName == "BUF_X1") {
			details->setBufPos(i);
		}
	}
}

//update the fanin/fanout attributes of all the components
void updateConnection(vector<Node>& component) {
	vector<string> output;// , input;
	int iSize = 0;
	for (unsigned int i = 0; i < component.size(); i++) {
		output = component[i].getOutputs();
		//for loop 'j' is redundant. I'm assuming m=components of multiple output.
		// It can be used for components with more than one outputs latter.
		for (int j = 0; j < component[i].getOutputCount(); j++) {
			for (unsigned int k = 0; k < component.size(); k++) {
				//input = component[k].Inputs;
				//input = component[k].getInputs();
				iSize = component[k].InputCount;
				if (i != k) {
					if (iSize == 0)
						break;		//performance optimization.
					else {
						//check all the fanin of the particular node before going ahead
						for (unsigned int l = 0; l < component[k].getInputCount(); l++) {
							if (output[j].compare(component[k].Inputs[l]) == 0) {
								component[i].setUpdateFanout(k);	//set fanin and fanout together
								component[k].setUpdateFanin(i);
								--iSize;	//decrement this variable to improve the performance
							}
						}
					}
				}
			}
		}
	}
}

//Function to print fanin's
void printFanin(vector<Node>& component) {
	vector<int> fanin;
	for (unsigned int i = 0; i < component.size(); i++) {
		if (component[i].getPortDirection() == false) {
			cout << component[i].getName() << ": ";
			fanin = component[i].getFanin();
			//cout << "fanin size " << fanin.size() << " ";
			for (unsigned int j = 0; j < fanin.size(); j++) {
				if (j == fanin.size() - 1)
					cout << component[fanin[j]].getName();
				else
					cout << component[fanin[j]].getName() << ", ";
			}
			cout << "\n";
		}
	}
}

//Function to print fanout's
void printFanout(vector<Node>& component) {
	vector<int> fanout;
	for (unsigned int i = 0; i < component.size(); i++) {
		if (!((component[i].getPortDirection() == false) && (component[i].getIsPort() == true))) {
			cout << component[i].getName() << ": ";
			fanout = component[i].getFanout();
			//cout << "fanin size " << fanin.size() << " ";
			for (unsigned int j = 0; j < fanout.size(); j++) {
				if (j == fanout.size() - 1)
					cout << component[fanout[j]].getName();
				else
					cout << component[fanout[j]].getName() << ", ";
			}
			cout << "\n";
		}
	}
}

//Function to print the values of a vector - Mainly used for debug.
void printVectorValues(vector<double> list) {
	for (unsigned int j = 0; j < list.size(); j++) {
		if (j == list.size() - 1)
			cout << list[j] << endl;
		else
			cout << list[j] << ", ";
	}
}

//Write the delay information of the library cells to a file
void printDelay(vector<Lib>& component) {
	vector<double> runner1D;
	vector<vector<double> > runner2D;
	ofstream outPut;

	outPut.open("delay_LUT.txt");
	
	//outPut << "=================================================================================" << endl;
	//outPut << " Delay LUT" << endl;
	//outPut << "=================================================================================" << endl;

	for (unsigned int i = 0; i < component.size(); i++) {
		//outPut << endl;
		outPut << "cell" << ": " << setw(10) << left << component[i].getCellName() << endl;
		//outPut << setw(15) << left << "Cell" << ": " << setw(10) << left << component[i].getCellName() << endl;
		//outPut << setw(15) << left << "Capacitance" << ": " << setw(10) << left << component[i].getCapacitance() << endl;
		//outPut << setw(15) << left << "Input Slews" << ": ";
		outPut << "input slews" << ": ";
		runner1D = component[i].getDelayInputSlew();
		for (unsigned int j = 0; j < runner1D.size(); j++) {
			if (j == runner1D.size() - 1)
				outPut << runner1D[j] << endl;
				//outPut << setw(10) << left << runner1D[j] << endl;
			else
				outPut << runner1D[j] << ",";
				//outPut << setw(10) << left << runner1D[j] << ", ";
		}
		runner1D.clear();
		outPut << "load cap" << ": ";
		//outPut << setw(15) << left << "Output Loads" << ": ";
		runner1D = component[i].getDelayOutputLoad();
		for (unsigned int j = 0; j < runner1D.size(); j++) {
			if (j == runner1D.size() - 1)
				//outPut << setw(10) << left << runner1D[j] << endl;
				outPut << runner1D[j] << endl;
			else
				//outPut << setw(10) << left << runner1D[j] << ", ";
				outPut << runner1D[j] << ",";
		}
		runner1D.clear();

		runner2D = component[i].getCellDelays();
		outPut << endl;
		outPut << "delays:" << endl;
		for (unsigned int j = 0; j < runner2D.size(); j++) {
			runner1D = runner2D[j];
			for (unsigned int j = 0; j < runner1D.size(); j++) {
				if (j == runner1D.size() - 1)
					outPut << runner1D[j] << ";" << endl << endl;
					//outPut << setw(10) << left << runner1D[j] << ";" << endl;
				else
					outPut << runner1D[j] << ",";
					//outPut << setw(10) << left << runner1D[j] << ", ";
			}
		}
	outPut << endl;
	//outPut << "=================================================================================" << endl;
	}
}

//Write the slew information of the library cells to a file
void printSlew(vector<Lib>& component) {
	vector<double> runner1D;
	vector<vector<double> > runner2D;
	ofstream outPut;

	outPut.open("slew_LUT.txt");
	
	//outPut << "=================================================================================" << endl;
	//outPut << " Slew LUT" << endl;
	//outPut << "=================================================================================" << endl;

	for (unsigned int i = 0; i < component.size(); i++) {
		//outPut << endl;
		outPut << "cell" << ": " << setw(10) << left << component[i].getCellName() << endl;
		//outPut << setw(15) << left << "Cell" << ": " << setw(10) << left << component[i].getCellName() << endl;
		//outPut << setw(15) << left << "Capacitance" << ": " << setw(10) << left << component[i].getCapacitance() << endl;
		//outPut << setw(15) << left << "Input Slews" << ": ";
		outPut << "input slews" << ": ";
		runner1D = component[i].getSlewInputSlew();
		for (unsigned int j = 0; j < runner1D.size(); j++) {
			if (j == runner1D.size() - 1)
				//outPut << setw(10) << left << runner1D[j] << endl;
				outPut << runner1D[j] << endl;
			else
				//outPut << setw(10) << left << runner1D[j] << ", ";
				outPut << runner1D[j] << ",";
		}
		runner1D.clear();
		outPut << "load cap" << ": ";
		//outPut << setw(15) << left << "load cap" << ": ";
		runner1D = component[i].getSlewOutputLoad();
		for (unsigned int j = 0; j < runner1D.size(); j++) {
			if (j == runner1D.size() - 1)
				//outPut << setw(10) << left << runner1D[j] << endl;
				outPut << runner1D[j] << endl;
			else
				//outPut << setw(10) << left << runner1D[j] << ", ";
				outPut <<runner1D[j] << ",";
		}
		runner1D.clear();

		runner2D = component[i].getCellSlews();
		outPut << endl;
		outPut << "slews:" << endl;
		for (unsigned int j = 0; j < runner2D.size(); j++) {
			runner1D = runner2D[j];
			for (unsigned int j = 0; j < runner1D.size(); j++) {
				if (j == runner1D.size() - 1)
					//outPut << setw(10) << left << runner1D[j] << ";" << endl;
					outPut << runner1D[j] << ";" << endl << endl;
				else
					//outPut << setw(10) << left << runner1D[j] << ", ";
					outPut <<runner1D[j] << ",";
			}
		}
	outPut << endl;
	//outPut << "=================================================================================" << endl;
	}
}

//interpolation kernel
double getOptimal(double c1, double c, double c2, double t1, double t, double t2,
	              double v11, double v12, double v21, double v22) {
	return ((v11*(c2 - c)*(t2 - t) + v12*(c - c1)*(t2 - t) + v21*(c2 - c)*(t - t1) + v22*(c - c1)*(t - t1)) / ((c2 - c1)*(t2 - t1)));
}

//Heart of the parser. Dont touch!!
void updateTiming(vector<Node>& component, vector<Lib>& library, LibDetails& details, vector<int>& stack, double& Max, vector<int>& path) {
	vector<int> fanout, fanin, revStack;
	vector<int> criticalPath;
	int slkPos;
	double cktMax = 0, requiredTime = 0;

	//To calculate the arrival time and slew
	for (int i : stack) {
		double sum = 0;
		fanout.clear();
		fanout = component[i].getFanout();

		for (int j : fanout) {
			if (component[j].getIsPort() && (!component[j].getPortDirection()))
				// output ports has 4 times the capacitance of an inverter
				sum += 4 * library[details.getInvPos()].getCapacitance();
			else {
				if (component[j].getIsPort() && component[j].getPortDirection())
					sum += 0;
				else 
					// load cap is the sum of all the capacitance of the fanouts
					sum += library[component[j].getLibPosition()].getCapacitance();
			}			
		}

		component[i].setOCL(sum);

		double tau = 0, arrTime = 0;
		double loadCap = 0, c1 = 0, c2 = 0, t1 = 0, t2 = 0, max = 0;
		short int lowerL = 0, upperL = 0, lowerT = 0, upperT = 0, pos = 0;
		vector<double> slew, load, intDelay, iArrival;
		fanin.clear();
		intDelay.clear();
		iArrival.clear();
		fanin = component[i].getFanin();

		//set the default parameters for inputs
		if (component[i].getIsPort() && component[i].getPortDirection()) {
			arrTime = 0;
			tau = 0.002;
			component[i].setArrivalTime(arrTime);
			component[i].setTau(tau);
			component[i].setOTau(tau);
			component[i].setODelay(arrTime);
		}
		//set the computed parameters for gates
		else if (!component[i].getIsPort()) {
			//copy the slew and delay index of the cell type the component is using
			slew = library[component[i].getLibPosition()].getDelayInputSlew();
			load = library[component[i].getLibPosition()].getDelayOutputLoad();
			loadCap = component[i].getOCL();

			// find the position where the load capacitance is identified
			for (unsigned int j = 0; j < load.size(); j++) {
				if (load[j] < loadCap) {
					lowerL = j;
					c1 = load[j];
				}
				else break;
			}
			upperL = lowerL + 1;
			// corner case for load exceedin the LUT value
			if (upperL >= 7) {
				lowerL = 5;	//use the last 2 elements for extrapolating
				upperL = 6;	//TODO: use better way to extrapolate
				c1 = load[lowerL];
			}
			c2 = load[upperL];
			
			for (int j : fanin) {
			// get the lowest tau and the corresponding components output arrival time
				double v11 = 0, v12 = 0, v21 = 0, v22 = 0;
				double internalDelay = 0, internalTau = 0;
				arrTime = component[j].getOTime();
				tau = component[j].getOTau();
				component[i].setArrivalTime(arrTime);
				component[i].setTau(tau);

			// find the position where the lowest slew is located
				for (unsigned int k = 0; k < slew.size(); k++) {
					if (slew[k] < tau) {
						lowerT = k;
						t1 = slew[k];
					}
					else break;
				}

				upperT = lowerT + 1;
			// corner case for slew exceeding the LUT value
				if (upperT >= 7) {
					lowerT = 5;	//use the last 2 elements for extrapolating
					upperT = 6;	//TODO: use better way to extrapolate
					t1 = slew[lowerT];
				}
				t2 = slew[upperT];

				// set the Vxy for interpolation/extrapolation
				v11 = library[component[i].getLibPosition()].getDelayElem(lowerT, lowerL);
				v12 = library[component[i].getLibPosition()].getDelayElem(lowerT, upperL);
				v21 = library[component[i].getLibPosition()].getDelayElem(upperT, lowerL);
				v22 = library[component[i].getLibPosition()].getDelayElem(upperT, upperL);
				internalDelay = getOptimal(c1, loadCap, c2, t1, tau, t2, v11, v12, v21, v22);
				v11 = library[component[i].getLibPosition()].getSlewElem(lowerT, lowerL);
				v12 = library[component[i].getLibPosition()].getSlewElem(lowerT, upperL);
				v21 = library[component[i].getLibPosition()].getSlewElem(upperT, lowerL);
				v22 = library[component[i].getLibPosition()].getSlewElem(upperT, upperL);
				internalTau = getOptimal(c1, loadCap, c2, t1, tau, t2, v11, v12, v21, v22);

				//adjust the delay and slew for n-gated component
				if (component[i].getInputCount() > 2) {
					component[i].setInternalDelay(internalDelay * component[i].getInputCount() / 2);
					component[i].setInternalTau(internalTau * component[i].getInputCount() / 2);
				}
				else {
					component[i].setInternalDelay(internalDelay);
					component[i].setInternalTau(internalTau);
				}
			}

			intDelay = component[i].getInternalDelay();
			iArrival = component[i].getArrivalTime();

			//find the max(internal delay + internal slew)
			max = 0; pos = 0;
			for (unsigned int j = 0; j < intDelay.size(); j++) {
				if (max < (intDelay[j] + iArrival[j])) {
					max = intDelay[j] + iArrival[j];
					pos = j;
				}
			}

			//set the component outDelay and outSlew
			component[i].setODelay(intDelay[pos] + iArrival[pos]);
			component[i].setOTau(component[i].getInternalTau(pos));
		}
		else {
		//set the parameters for outputs
			for (int j : fanin) {
				arrTime = component[j].getOTime();
				tau = component[j].getOTau();
				component[i].setArrivalTime(arrTime);
				component[i].setTau(tau);
				component[i].setOTau(tau);
				component[i].setODelay(arrTime);
			}
		}
	}

	// to find the circuit time, check for the largest delay on the output
	for (int i : stack) { // traversing through the entire stack is still not a problem.
				// will implement a better method next time ;)
		if (component[i].getIsPort() && (!component[i].getPortDirection())) {
			if (cktMax < component[i].getOTime()) {
				cktMax = component[i].getOTime();
			}
		}
	}

	Max = cktMax;
	requiredTime = 1.1 * cktMax; // hard-coded RAT. TODO: use an argument to set as arbitrary RAT

	double minSlk = requiredTime;

	//To calculate the slack
	revStack = stack; //FYI: I used DFS for topographical traversal. 
			  //I'm reversing the stack to iterate from output to input
	reverse(revStack.begin(), revStack.end());

	for (int i : revStack) {
	// get the position of minimum slack on the outputs
		if (component[i].getIsPort() && (!component[i].getPortDirection())) {
			component[i].setSlack(requiredTime - component[i].getOTime());
			component[i].setRequiredArrivalTime(requiredTime);
			if (minSlk > component[i].getSlack()) {
				minSlk = component[i].getSlack();
				slkPos = i;
			}
		}
		else {
			double minRAT = requiredTime;
			for (int j : component[i].getFanout()) {
				short int l = 0;
				for (int k : component[j].getFanin()) {
					if (i == k) {
						if (component[j].getIsPort() && (!component[j].getPortDirection())) {
							//output ports: easy to get the slack and RAT.
							component[k].setSlack(requiredTime - component[i].getOTime());
							component[k].setRequiredArrivalTime(requiredTime);
							//cout << component[k].getName() << " " << component[k].getOTime() << " " << endl;
						}
						// slack for the gates is the minimumRAT-delay
						else if (minRAT > (component[j].getRequiredArrivalTime() - component[j].getInternalDelay()[l])) {
							minRAT = (component[j].getRequiredArrivalTime() - component[j].getInternalDelay()[l]);
							//cout << component[j].getName() << " " << component[j].getRequiredArrivalTime() << " " << component[j].getInternalDelay()[l] << endl;
						}
					}
					l++;
				}
			}
			// slack for the gates is the minimumRAT-delay
			component[i].setRequiredArrivalTime(minRAT);
			component[i].setSlack(minRAT - component[i].getOTime());
			//cout << component[i].getName() << " " << component[i].getRequiredArrivalTime()*1000 << " " << component[i].getOTime()*1000 << " " << component[i].getSlack()*1000 << endl;
		}
	}
	bool flag = true;
	bool valid = false;
	
	//find 'A' critical path
	//traverse back from the output till input through gates.. obviously!..
	do {
		if (abs(component[slkPos].getSlack() - minSlk) < 0.00000001) 
			criticalPath.push_back(slkPos);
		for (int i : component[slkPos].getFanin()) 
			if (abs(component[i].getSlack() - minSlk) < 0.00000001 )
				slkPos = i;
	}
	while (!(component[slkPos].getIsPort() && component[slkPos].getPortDirection() && flag));
	
	//store the critical path
	criticalPath.push_back(slkPos);
	reverse(criticalPath.begin(), criticalPath.end());
	path = criticalPath;


}

//Function to print the circuit summary..
//Basic c++ stuff..
void printSummary(vector<Node>& components, vector<int>& criticalPath, double time, int full) {
	vector<int> fanin, fanout;
	ofstream outPut, details;

	int priIn = 0, priOut = 0, AND = 0, NAND = 0, OR = 0, NOR = 0, NOT = 0, BUF = 0, others = 0;
	if(full == 0) {
		outPut.open("ckt_details.txt");
		for (unsigned int i = 0; i < components.size(); i++)	{
			if (components[i].getIsPort() && components[i].getPortDirection()) { priIn++; }
			else if (components[i].getIsPort() && !components[i].getPortDirection()) { priOut++; }
			else if (components[i].getName().find("NAND") != components[i].getName().npos) { NAND++; }
			else if (components[i].getName().find("AND") != components[i].getName().npos) { AND++; }
			else if (components[i].getName().find("OR") != components[i].getName().npos) { OR++; }
			else if (components[i].getName().find("NOR") != components[i].getName().npos) { NOR++; }
			else if (components[i].getName().find("NOT") != components[i].getName().npos) { NOT++; }
			else if (components[i].getName().find("BUFF") != components[i].getName().npos) { BUF++; }
			else { others++; }
		}

		//outPut << "=================================================================================" << endl;
		outPut << priIn << " primary inputs" << endl;
		outPut << priOut << " primary outputs" << endl;
		if (NAND >= 1) { outPut << NAND << " NAND gates" << endl; }
		if (AND >= 1) { outPut << AND << " AND gates" << endl; }
		if (NOR >= 1) { outPut << NOR << " NOR gates" << endl; }
		if (OR >= 1) { outPut << OR << " OR gates" << endl; }
		if (NOT >= 1) { outPut << NOT << " NOT gates" << endl; }
		if (BUF >= 1) { outPut << BUF << " BUFF gates" << endl; }
		if (others >= 1) { outPut << others << "Other gates" << endl; }
		outPut << endl;
		//outPut << "=================================================================================" << endl;
		
		//print Fanout
		outPut << "Fanout..." << endl;
		for (unsigned int i = 0; i < components.size(); i++) {
			if (!((components[i].getPortDirection() == false) && (components[i].getIsPort() == true))) {
				outPut << components[i].getName() << ": ";
				fanout = components[i].getFanout();
				//cout << "fanin size " << fanin.size() << " ";
				for (unsigned int j = 0; j < fanout.size(); j++) {
					if (j == fanout.size() - 1)
						outPut << components[fanout[j]].getName();
					else
						outPut << components[fanout[j]].getName() << ", ";
				}
				outPut << "\n";
			}
		}
		outPut << endl;
		//print Fanin
		outPut << "Fanin..." << endl;
		for (unsigned int i = 0; i < components.size(); i++) {
			if (components[i].getPortDirection() == false) {
				outPut << components[i].getName() << ": ";
				fanin = components[i].getFanin();
				//cout << "fanin size " << fanin.size() << " ";
				for (unsigned int j = 0; j < fanin.size(); j++) {
					if (j == fanin.size() - 1)
						outPut << components[fanin[j]].getName();
					else
						outPut << components[fanin[j]].getName() << ", ";
				}
				outPut << "\n";
			}
		}
	}
	
	if (full == 1) {
		details.open("ckt_traversal.txt");
		//details << "=================================================================================" << endl;
		details << setw(16) << left << "Circuit Delay : " << time*1000 << " ps" << endl;
		details << endl;
		//details << "=================================================================================" << endl;
			details << "Gate slacks:"
			//<< setw(40) << left << "Name"
			//<< setw(15) << left << "Cap(fF)"
			//<< setw(12) << left << "RAT(ps)"
			//<< setw(12) << left << "Arr(ps)"
			//<< setw(12) << left << "Slew(ps)"
			//<< setw(12) << left << "Slack(ps)"
			<< endl;

		for (unsigned int i = 0; i < components.size(); i++) {
			//cout << components[i].getName() << " " <<
			//	components[i].getRequiredArrivalTime() * 1000 << " " <<
			//	components[i].getOTime() * 1000 << " " <<
			//	components[i].getSlack() * 1000 << ;
			details << components[i].getName() << ": " 
				//<< setw(40) << left << components[i].getName()
				//<< setw(15) << left << components[i].getOCL()
				//<< setw(12) << left << components[i].getRequiredArrivalTime() * 1000
				//<< setw(12) << left << components[i].getOTime() * 1000
				//<< setw(12) << left << components[i].getOTau() * 1000
				//<< setw(12) << left << components[i].getSlack() * 1000
				<< components[i].getSlack() * 1000 << " ps"
				<< endl;
		}
		details << endl;
		//details << "=================================================================================" << endl;
		details << "Critical Path:" << endl;
		for (unsigned int i = 0; i < criticalPath.size(); i++) {
			if (i == criticalPath.size() - 1)
				details << components[criticalPath[i]].getName() << endl;
			else 
				details << components[criticalPath[i]].getName() << ", ";
		}
		details.close();
	}
	outPut.close();
	
}
