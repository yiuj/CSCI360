#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include "PartiallyKnownGrid.h"
#include <iostream> 
#include <queue> 
#include <set>
#include <vector>
#include <map>

using namespace std;

class GridPathPlanner{
public:
	GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_);
	~GridPathPlanner();
	
	// Finds a path from the "start" to the target (set in constructor).
	// Fills the "path" vector with a sequence of xyLocs.
	void FindPath(xyLoc start, std::vector<xyLoc> & path);

	// Return the current heuristic distance to the target.
	int GetHValue(xyLoc l);

	// Return the number of expanded nodes in the most recent search.
	int GetNumExpansionsFromLastSearch();

	// Get adjacent locations
	vector<xyLoc> findAdjacents(xyLoc start);

	struct gridNode {
		public:
			gridNode() { loc = xyLoc(-1, -1);}
			gridNode(xyLoc _loc) {
				loc = _loc;
				g = INT_MAX;
				h = 0;
				prev = nullptr;
			}
			gridNode(int x, int y) {
				loc = xyLoc(x,y);
				g = INT_MAX;
				h = 0;
				prev = nullptr;
			}
			int g, h;
			xyLoc loc;
			gridNode* prev;

			bool operator<(gridNode const& other) const {
				int f1 = this->g + this->h;
				int f2 = other.g + other.h;

				if(f1 == f2) {
					return (this->g > other.g);
				}
				return (f1 > f2);
			}
	};
		
private:
	PartiallyKnownGrid* grid;	// Partially known grid (updates automatically as the agent moves.
	bool adaptive;						// If set, the heuristic should be updated after each search.
	xyLoc destination;				// Fixed goal cell.
	bool larger_g;	// If set to true, your search should tie-break towards larger g-values.
									// If set to false, your search should tie-break towards smaller g-values.

	int expansions;
	map<xyLoc, gridNode*> nodeMap;
	int gSGoal;
	bool firstRun;
};

#endif

