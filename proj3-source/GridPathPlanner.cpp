#include "GridPathPlanner.h"

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_) {
	grid = grid_;
	destination = destination_;
	adaptive = adaptive_;
	larger_g = larger_g_;
}

GridPathPlanner::~GridPathPlanner(){
}

  
void GridPathPlanner::FindPath(xyLoc start, std::vector<xyLoc> & path) {
	// TODO
	// Possible flow:
	// - Initialize data structures / open list
  // - Search until goal is selected for expansion
  // - Extract path
  // - Update heuristic if adaptive
}


int GridPathPlanner::GetHValue(xyLoc l) {
	// TODO
	return 0;
}

int GridPathPlanner::GetNumExpansionsFromLastSearch() {
	// TODO
	return 0;
}

