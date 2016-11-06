//==================================================================================
// DFS based topographical traversal
// =================================
//
// @author: Sriram Kuppahalli Venkatesha
// @email:  kuppa010@umn.edu
// =====================================
//
// Description:
// ============
//
// This is the class definition for implementing DFS based topographical traversal.
// refer this link for further details "https://www.youtube.com/watch?v=NyP3TKcruO8"
//
//==================================================================================
// Dfs.cpp : 
// Definition of all the functions used in DFS class.

#include "algorithm"
#include "../include/Dfs.h"

//Default constructor
DFS::DFS() {
	this->marked.clear();
	this->connections.clear();
	this->stack.clear();
}

//Overloaded constructor
DFS::DFS(vector<vector<int> > connection) {
	this->connections = connection;
	this->stack.clear();
	for (unsigned int i = 0; i < connections.size();i++) {
		this->marked.push_back(false);
	}
}

//Perform DFS traversal for al the unmarked nodes and update the stack
void DFS::dfs(int u) {
	marked[u] = true;
	vector<int> row = connections[u];
		for (int i: row) {
			if (!marked[i]) {
				DFS::dfs(i);
			}
		}
		this->stack.push_back(u);
}

//DFS kernel
void DFS::dfs_topo() {
	for (unsigned int i = 0; i < connections.size(); i++) {
		if (marked[i] == false)
			dfs(i);
	}
}

//reverse the stack to get the topographical sort.
vector<int> DFS::getStack() {
	reverse(stack.begin(), stack.end());
	return this->stack;
}
