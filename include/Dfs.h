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
// Dfs.h : 

// Definition of all the functions used in DFS class.
// Definition of the DFS class.

#include "fstream"
#include "iostream"
#include "string"
#include "sstream"
#include "vector"

using namespace std;

#ifndef DFS_H
#define DFS_H

class DFS {
private:
	vector<bool> marked;
	vector<vector<int> > connections;
	vector<int> stack;

public:
	DFS();
	DFS(vector<vector<int> >);
	void dfs(int);
	void dfs_topo();
	vector<int> getStack();
};
#endif // !DFS_H
