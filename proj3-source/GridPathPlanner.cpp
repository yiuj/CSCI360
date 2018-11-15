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

	// Create map of nodes with loction and f,g,h data members
	// nodeMap = new gridNode*[width];
	// for(int i=0; i<width; i++) {
	// 	nodeMap[i] = new gridNode[height];
	// }
	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			nodeMap[xyLoc(x,y)] = new gridNode(x,y);
			
			nodeMap[xyLoc(x,y)]->loc = xyLoc(x,y);
			nodeMap[xyLoc(x,y)]->h = abs(x - destination.x) + abs(y - destination.y);
			nodeMap[xyLoc(x,y)]->g = INT_MAX;
		}
	}
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
	// if(start.x > 0) {
	// 	adjacents.push_back(xyLoc(start.x-1, start.y));
	// }
	// if(start.x < grid->GetWidth()-1) {
	// 	adjacents.push_back(xyLoc(start.x+1, start.y));
	// }
	// if(start.y > 0) {
	// 	adjacents.push_back(xyLoc(start.x, start.y-1));
	// }
	// if(start.y < grid->GetHeight()-1) {
	// 	adjacents.push_back(xyLoc(start.x, start.y+1));
	// }
	return adjacents;
}
  
void GridPathPlanner::FindPath(xyLoc start, std::vector<xyLoc> & path) {
	// TODO
	// Possible flow:
	// - Initialize data structures / open list
	// - Search until goal is selected for expansion
	// - Extract path
	// - Update heuristic if adaptive

	int width = grid->GetWidth();
	int height = grid->GetHeight();

	// Create map of nodes with loction and f,g,h data members
	// nodeMap = new gridNode*[width];
	// for(int i=0; i<width; i++) {
	// 	nodeMap[i] = new gridNode[height];
	// }
	for(int x=0; x<width; x++) {
		for(int y=0; y<height; y++) {
			
			nodeMap[xyLoc(x,y)]->loc = xyLoc(x,y);
			nodeMap[xyLoc(x,y)]->h = abs(x - destination.x) + abs(y - destination.y);
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
		// cout << "LOOKING AT: " << q->loc << " --> ";

		vector<xyLoc> adjacents = findAdjacents(q->loc);
		for(int i=0; i<adjacents.size(); i++) {
			xyLoc nextLocation = adjacents.at(i);
			// gridNode successor = nodeMap[nextLocation.x][nextLocation.y];
			int new_distance = q->g + 1;
			if(new_distance < nodeMap[nextLocation]->g) {
				nodeMap[nextLocation]->g = new_distance;
				// nodeMap[nextLocation.x][nextLocation.y].prev = &q;
				prev[nextLocation] = q->loc;
				// cout << " --> " << nodeMap[nextLocation.x][nextLocation.y].prev->loc << endl;
			}

			if(history.count(nextLocation) == 0) {
                if(nextLocation == destination) {
                    found = true;
                }
                openList.push(nodeMap[nextLocation]);
                history.insert(nextLocation);
				// cout << nextLocation << " ";
            }
		}
		// cout << endl;
	}
	// cout << "RIGHT AFTER LOOP: ";
	xyLoc currLoc = destination;
	prev[start] = xyLoc();
	while(currLoc.x != -1) {
		// cout << currLoc << ", ";
		path.push_back(currLoc);
		currLoc = prev[currLoc];
	}

	// if(nodeMap[destination.x][destination.y].prev) { // If destination was found
	// 	path.push_back(destination);
	// 	gridNode* tracker = nodeMap[destination.x][destination.y].prev;
	// 	while(tracker != nullptr) {
	// 		path.push_back(tracker->loc);
	// 		tracker = tracker->prev;
	// 	}
	// }
	reverse(path.begin(), path.end());
	// cout << "========================================" << endl;
	// cout << "PATH: " << endl;
	// for(int i=0; i<path.size(); i++) {
	// 	cout << path.at(i) << endl;
	// }
	// cout << "========================================" << endl;

	// checking if destination was found
	// cout << "========================================" << endl;
	// cout << "DESTINATION: (" << destination.x << "," << destination.y << ")" << endl;
	// if(nodeMap[destination.x][destination.y].prev) {
	// 	cout << "PREVIOUS: (" << nodeMap[destination.x][destination.y].prev->loc.x 
	// 		<< "," << nodeMap[destination.x][destination.y].prev->loc.y << ")" << endl;
	// }
	// else {
	// 	cout << "Destination not found" << endl;
	// }
	// cout << "========================================" << endl;
	cout << "EXPANSIONS: " << expansions << endl;
	expansions = 0;
}


int GridPathPlanner::GetHValue(xyLoc l) {
	// TODO
	if(!adaptive) {
		return nodeMap[l]->h;
	}
	return abs(l.x - destination.x) + abs(l.y - destination.y);
}

int GridPathPlanner::GetNumExpansionsFromLastSearch() {
	// TODO
	return 0;
}

