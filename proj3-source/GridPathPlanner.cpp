#include "GridPathPlanner.h"

#include <iostream> 
#include <queue> 
#include <set>

using namespace std;

GridPathPlanner::GridPathPlanner(PartiallyKnownGrid* grid_, xyLoc destination_, bool adaptive_, bool larger_g_) {
	grid = grid_;
	destination = destination_;
	adaptive = adaptive_;
	larger_g = larger_g_;

	expansions = 0;
	
	int width = grid->GetWidth();
	int height = grid->GetHeight();

	// Create map of nodes with loction and f,g,h data members
	map = new gridNode*[width];
	for(int i=0; i<width; i++) {
		map[i] = new gridNode[height];
	}
	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			map[x][y].loc = xyLoc(x,y);
			map[x][y].h = abs(x - destination.x) + abs(y - destination.y);
			map[x][y].g = INT_MAX;
		}
	}
}

GridPathPlanner::~GridPathPlanner(){
}

vector<xyLoc> GridPathPlanner::findAdjacents(xyLoc start) {
	vector<xyLoc> adjacents;
	if(start.x > 0) {
		adjacents.push_back(xyLoc(start.x-1, start.y));
	}
	if(start.x < grid->GetWidth()-1) {
		adjacents.push_back(xyLoc(start.x+1, start.y));
	}
	if(start.y > 0) {
		adjacents.push_back(xyLoc(start.x, start.y-1));
	}
	if(start.y < grid->GetHeight()-1) {
		adjacents.push_back(xyLoc(start.x, start.y+1));
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

	priority_queue <gridNode> openList; // initialize open list
	set<gridNode> history; // initialize history of nodes visited


	openList.push(map[start.x][start.y]); // add starting node to open list
	map[start.x][start.y].g = 0; // distance from start to start is 0
	history.insert(map[start.x][start.y]);

	bool found = false; // destination found?

	while(!openList.empty() && !found) {
		// access and remove top of openList
		gridNode q = openList.top();
		openList.pop();
		expansions++;

		vector<xyLoc> adjacents = findAdjacents(q.loc);
		for(xyLoc nextLocation: adjacents) {
			gridNode successor = map[nextLocation.x][nextLocation.y];
			int new_distance = q.g + 1;
			if(new_distance < successor.g) {
				successor.g = new_distance;
				successor.prev = q;
			}

			if(history.count(successor) == 0) {
                if(nextLocation == destination) {
                    found = true;
                }
                openList.push(successor);
                history.insert(successor);
            }
		}
	}



	if(path.empty()) {
		path.push_back(start);
	}







	std::cout << "PATH: ";
	for(int i=0; i<path.size(); i++) {
		std::cout << "(" << path.at(i).x << "," << path.at(i).y << "), ";
	}
	std::cout << std::endl;
}


int GridPathPlanner::GetHValue(xyLoc l) {
	// TODO
	if(!adaptive) {
		return map[l.x][l.y].h;
	}
	return abs(l.x - destination.x) + abs(l.y - destination.y);
}

int GridPathPlanner::GetNumExpansionsFromLastSearch() {
	// TODO
	return 0;
}

