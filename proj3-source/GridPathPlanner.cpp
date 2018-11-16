#include "GridPathPlanner.h"

#include <iostream> 
#include <queue> 
#include <set>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_) {
	grid = grid_;
	destination = destination_;
	adaptive = adaptive_;
	larger_g = larger_g_;

	expansions = 0;
	
	int width = grid->GetWidth();
	int height = grid->GetHeight();

	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			nodeMap[xyLoc(x,y)] = new gridNode(x,y);
			
			nodeMap[xyLoc(x,y)]->loc = xyLoc(x,y);
			nodeMap[xyLoc(x,y)]->h = abs(x - destination.x) + abs(y - destination.y);
			nodeMap[xyLoc(x,y)]->g = INT_MAX;
		}
	}

	firstRun = true;
}

GridPathPlanner::~GridPathPlanner(){
}

vector<xyLoc> GridPathPlanner::findAdjacents(xyLoc start) {
	vector<xyLoc> adjacents;

	xyLoc up = xyLoc(start.x, start.y-1);
	xyLoc left = xyLoc(start.x-1, start.y);
	xyLoc right = xyLoc(start.x+1, start.y);
	xyLoc down = xyLoc(start.x, start.y+1);

	if(!grid->IsBlocked(up) && grid->IsValidLocation(up)) {
		adjacents.push_back(up);
	}
	if(!grid->IsBlocked(right) && grid->IsValidLocation(right)) {
		adjacents.push_back(right);
	}
	if(!grid->IsBlocked(left) && grid->IsValidLocation(left)) {
		adjacents.push_back(left);
	}
	if(!grid->IsBlocked(down) && grid->IsValidLocation(down)) {
		adjacents.push_back(down);
	}
	return adjacents;
}
  
void GridPathPlanner::FindPath(xyLoc start, std::vector<xyLoc> & path) {
	// TODO
	// Possible flow:
	// - Initialize data structures / open list
	// - Search until goal is selected for expansion
	// - Extract path
	// - Update heuristic if adaptive

	if(firstRun) {
		gSGoal = abs(start.x - destination.x) + abs(start.y - destination.y);
	}

	// reset g values
	int width = grid->GetWidth();
	int height = grid->GetHeight();
	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			nodeMap[xyLoc(x,y)]->g = INT_MAX;
		}
	}

	priority_queue < gridNode* > openList; // initialize open list
	set<xyLoc> history; // initialize history of nodes visited
	map<xyLoc, xyLoc> prev; // initialize map of previous locations

	// set previous locations of all locations to (-1, -1)
	// int width = grid->GetWidth();
	// int height = grid->GetHeight();
	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			prev[xyLoc(x,y)] = xyLoc();
		}
	}
	openList.push(nodeMap[start]);
	// openList.push(nodeMap[start.x][start.y]); // add starting node to open list
	nodeMap[start]->g = 0; // distance from start to start is 0
	history.insert(start);

	bool found = false; // destination found?

	while(!openList.empty() && !found) {
		// access and remove top of openList
		gridNode* q = openList.top();
		openList.pop();
		expansions++;

		vector<xyLoc> adjacents = findAdjacents(q->loc);
		for(int i=0; i<adjacents.size(); i++) {
			xyLoc nextLocation = adjacents.at(i);
			int new_distance = q->g + 1;
			if(new_distance < nodeMap[nextLocation]->g) {
				nodeMap[nextLocation]->g = new_distance;
				prev[nextLocation] = q->loc;
			}

			if(history.count(nextLocation) == 0) {
                if(nextLocation == destination) {
                    found = true;
                }
                openList.push(nodeMap[nextLocation]);
                history.insert(nextLocation);
            }
		}
		// update h-value
		if(adaptive) {
			q->h = gSGoal - q->g;
		}
	}
	xyLoc currLoc = destination;
	prev[start] = xyLoc();
	while(currLoc.x != -1) {
		path.push_back(currLoc);
		currLoc = prev[currLoc];
	}
	reverse(path.begin(), path.end());
	gSGoal = path.size();
	
	firstRun = false;
}


int GridPathPlanner::GetHValue(xyLoc l) {
	// TODO
	return nodeMap[l]->h; // h value is updated in the findPath function so it accounts for adaptive
}

int GridPathPlanner::GetNumExpansionsFromLastSearch() {
	// TODO
	int lastExpansions = expansions;
	expansions = 0;
	return lastExpansions;
}

