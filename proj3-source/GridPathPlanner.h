#ifndef GRID_PATH_PLANNER_H
#define GRID_PATH_PLANNER_H

#include "PartiallyKnownGrid.h"



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
				prev = NULL;
			}
			int g, h;
			xyLoc loc;
			gridNode prev;
	};

	struct CompareSmallG
	{
		bool operator() (gridNode gn1, gridNode gn2)
		{
			int f1 = gn1.g + gn1.h;
			int f2 = gn2.g + gn2.h;

			if(f1 == f2) {
				return (gn1.g > gn2.g);
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
	gridNode **map;
};

#endif

