//==================================================================================
// Static Timing Analyzer
// ======================
//
// @author: Sriram Kuppahalli Venkatesha
// @email:  kuppa010@umn.edu
// =====================================
//
// Description:
// ============
//
// This is the entry ponit of the program. The aruments are read_ckt <netlist> or 
// read_nldm delays/slews <nldm file> or <netlist>
// This program uses DFS based topographical search to update the arrival time and
// calculate the slack and critical path.
// The modelled gates/ports have all the attributes for performing STA stored in 
// its object. The accessor and mutator functions are defined in Netlist.cc/.h file.
// Similarly, library cells are modelled and the definition can be seen in Library.*
//
//==================================================================================

#include "../include/Common.h"

//Main function is declared here.
//Arguments are as follows:
//	arg[0] : parser.obj
//	arg[1] : read_ckt/read_nldm/netlit nme
//	arg[2] : delay/slew
//	arg[3] : library file name
int main(int argc, char** argv) {

	int read_ckt = 0, read_nldm = 0;
	int delays = 0, slews = 0, full = 0;
	int valid = 0;
	double cktTime;
	string netlist, liblist;

	vector<vector<int> > fanoutInformation;
	vector<int> stack, criticalPath;
	vector<Node> components;
	vector<Lib> libraries;
	LibDetails libDetails;

	//parse the arguments
	if (argc == 3 && (string)argv[1] == "read_ckt") {
		read_ckt = 1;
		netlist = argv[2];
	}
	else if (argc == 4 && (string)argv[1] == "read_nldm") {
		read_nldm = 1;
		liblist = (string)argv[3];
		if ((string)argv[2] == "delays")
			delays = 1;
		else if ((string)argv[2] == "slews")
			slews = 1;
		else {
			cout <<"Incorrect usage of arguments. Please refer below" << endl;
			cout <<"./parser read_ckt  <netlist>          - to get the fanin/fanout details <<" << endl;
			cout <<"./parser read_nldm delays <nldm file> - to print cell delay LUTs" << endl;
			cout <<"./parser read_nldm slews  <nldm file> - to print cell slew LUTs" << endl;
			cout <<"./parser <netlist>                    - to print slack and critical path" << endl;
			exit(0);
		}
	}
	else if (argc == 2) {
		netlist = (string)argv[1];
		liblist = "nldm.lib";
		read_ckt = 1;
		read_nldm = 1;
		full = 1;
	}
	else {
		cout <<"Incorrect usage of arguments. Please refer below" << endl;
		cout <<"./parser read_ckt  <netlist>          - to get the fanin/fanout details <<" << endl;
		cout <<"./parser read_nldm delays <nldm file> - to print cell delay LUTs" << endl;
		cout <<"./parser read_nldm slews  <nldm file> - to print cell slew LUTs" << endl;
		cout <<"./parser <netlist>                    - to print slack and critical path" << endl;
		exit(0);
	}

	
	//empty the new vectors. Just being safe.
	components.clear();
	libraries.clear();

	cout << "Netlist/NLDM Parser + STA\n \n";
	
	if(read_nldm == 1) {
		cout << "Reading NLDM " << liblist << endl;
		//calling the NLDM parser
		parseLiblist(liblist, libraries);
		//this function call is not required if you are only parsing the nldm.
		updateLibPosition(libraries, &libDetails);
		if(delays == 1) {
			printDelay(libraries);
			cout << "Done";
		}
		else if(slews == 1) {
			printSlew(libraries);
			cout << "Done";
		}
		if(read_ckt == 0)
			exit(0);
	}

	if(read_ckt == 1) {
		cout << "Reading Netlist " << netlist << endl;
		parseNetlist(netlist, components, &libDetails);
		updateConnection(components);

		//copy fanout info for DFS topo.
		for (unsigned int i = 0; i < components.size(); i++)
			fanoutInformation.push_back(components[i].getFanout());
		
		//Perform DFS traversal to get the topogra`phical search.
		DFS d(fanoutInformation);
		d.dfs_topo();
		stack = d.getStack();
	
		if(full == 0 ) {
			printSummary(components, criticalPath, cktTime, full);
			cout << "Done";
			exit(0);
		}
	}

	//function to update the timing.
	updateTiming(components, libraries, libDetails, stack, cktTime, criticalPath);
	cout << endl;
	
	//Print the summary in a file.
	printSummary(components, criticalPath, cktTime, full);
	cout << "Done";
	return 0;
}
