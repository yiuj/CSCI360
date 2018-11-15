#include "PuzzleGenerator.h"
#include <limits>

using namespace std;

PuzzleGenerator::PuzzleGenerator(int _nRows, int _nColumns, int _minVal, int _maxVal)
	:nRows(_nRows), nColumns(_nColumns), minVal(_minVal), maxVal(_maxVal)
{}

Puzzle PuzzleGenerator::GeneratePuzzle()
{
	timer.StartTimer();
	maxtime = 59.9;	// To make sure we don't exceed a minute
	
	double random_walk_time = 5;	// 5 seconds.
	
	return HillClimbing(maxtime);
//*
	//return RandomWalk(random_walk_time);	// Do a random walk for some time and return the solution.
/*/
	// We could also do as many random walks as we can within the given time limit.
	Puzzle bestPuzzle = Puzzle(nRows, nColumns, minVal, maxVal);
	int bestValue = std::numeric_limits<int>::min();
	while (timer.GetElapsedTime() < maxtime-random_walk_time)
	{
		Puzzle p = RandomWalk(random_walk_time);
		int value = p.GetValue();
		if (value > bestValue) 
		{
			bestValue = value;
			bestPuzzle = p;
		}
	}
	return bestPuzzle;
//*/
}

Puzzle PuzzleGenerator::RandomWalk(double timelimit)
{
	// A very simple function that start at a random configuration and keeps randomly modifying it
	// until t hits the time limit. Returns the best solution found so far.

	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	
	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;			
	int bestValue = p.GetValue();
	
	// Keep track of the time so we don't exceed it.
	Timer t;
	t.StartTimer();
	
	// Loop until we hit the time limit.
	while (t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
	{
		// Generate a successor of p by randomly changing the value of a random cell 
		// (since we are doing a random walk, we just replace p with its successor).
		p = p.GetRandomSuccessor();	
		int value = p.GetValue();
		
		// Update the current best solution.
		if (value > bestValue)	// Calling GetValue() for the first time is costly
										// since the puzzle has to be evaluated first.
		{
			bestValue = value;	// Calling it a second time simply returns the value that was computed before.
			bestPuzzle = p;
		}
	}
	
	return bestPuzzle;
	
	// The following code is not executed in this function. It exists just as an example for getting all the successors of a puzzle.
}

Puzzle PuzzleGenerator::HillClimbing(double timelimit)
{
	Puzzle p(nRows, nColumns, minVal, maxVal);	// Generate a random puzzle with the specified values.
	
	// Keep track of the best puzzle found so far (and its value).
	Puzzle bestPuzzle = p;
	// Loop until we hit the time limit.
	int randomJumps = 10;
	double interval = timelimit/randomJumps;
	Timer t;
	t.StartTimer();
	for(int i=1; i<=randomJumps-1; i++) {
					
		int bestValue = p.GetValue();

		// Gets all successors
		vector<Puzzle> successors;
		bestPuzzle.GetAllSuccessors(successors);
		
		// Keep track of the time so we don't exceed it.
		
		while (t.GetElapsedTime() < i*interval && t.GetElapsedTime() < timelimit-0.1)	// To make sure that we don't exceed the time limit, we stop just before we hit the time limit.
		{
			// Generate a successor of p by randomly changing the value of a random cell 
			// (since we are doing a random walk, we just replace p with its successor).
			p = p.GetRandomSuccessor();	

			// Get successor with highest value
			
			int value = p.GetValue();
			for (int i=0; i<successors.size(); i++) {
				int successorValue = successors.at(i).GetValue();
				
				if(successorValue > value) {
					p = successors.at(i);
					value = successorValue;
				}
				
			}
			
			// Update the current best solution.
			if (value > bestValue)	// Calling GetValue() for the first time is costly
											// since the puzzle has to be evaluated first.
			{
				bestValue = value;	// Calling it a second time simply returns the value that was computed before.
				bestPuzzle = p;
			}
		}
	}
	
	return bestPuzzle;
}