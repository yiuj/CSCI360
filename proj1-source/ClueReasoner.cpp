#include "ClueReasoner.h"
using namespace std;

int ClueReasoner::GetPlayerNum(string player)
{
	if (player == case_file)
		return num_players;
	
	for (int i = 0; i < num_players; i++)
		if (player == players[i])
			return i;
			
	cout<<"Illegal player: "<<player<<endl;
	return -1;
}

int ClueReasoner::GetCardNum(string card)
{
	for (int i = 0; i < num_cards; i++)
		if (card == cards[i])
			return i;
			
	cout<<"Illegal card: "<<card<<endl;
	return -1;
}

int ClueReasoner::GetPairNum(int player, int card) 
{
	return player * num_cards + card + 1;
}

int ClueReasoner::GetPairNum(string player, string card) 
{
	return GetPairNum(GetPlayerNum(player), GetCardNum(card));
}

int ClueReasoner::Query(string player, string card) 
{
	return solver->TestLiteral(GetPairNum(player,card));
}

string ClueReasoner::QueryString(int return_code)
{
	if (return_code == kFalse)
		return "n";
	else if (return_code == kTrue)
		return "Y";
	else if (return_code == kUnknown)
		return "-";
	else
		return "X";
}

void ClueReasoner::PrintNotepad()
{
	for (int i = 0; i < num_players; i++)
		cout<<"\t"<<players[i];
	cout<<"\t"<<case_file<<endl;
	
	for (int i = 0; i < num_cards; i++)
	{
		cout<<cards[i]<<"\t";
		for (int j = 0; j < num_players; j++)
			cout<<QueryString(Query(players[j], cards[i]))<<"\t";
		
		cout<<QueryString(Query(case_file, cards[i]))<<endl;
	}
}
	
void ClueReasoner::AddInitialClauses()
{
	/* The following code is given as an example to show how to create Clauses and post them to the solver. SatSolver.h uses the following typedefs:
		typedef int Literal;
		typedef std::vector<Literal> Clause;
		
	That is, a Literal (a propositional variable or its negation) is defined as a positive or a negative (meaning that it is in negated form, as in -p or -q) integer, and a Clause is defined as a vector of Literals.
	
	The function GetPairNum(p, c) returns the literal that corresponds to card c being at location p (either a player's hand or the case_file). 
	See ClueReasoner.h, lines 7-31 for a definition of the arrays and variables that you can use in your implementation. 
	*/

	// Each card is in at least one place (including the case file).
	for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		Clause clause;
		for (int p = 0; p <= num_players; p++)	// Iterate over all players, including the case file (as a possible place for a card).
			clause.push_back(GetPairNum(p, c)); // Add to the clause the literal that states 'p has c'.
		
		solver->AddClause(clause);
	}
	
	// If a card is in one place, it cannot be in another place.
	// TO BE IMPLEMENTED AS AN EXERCISE
	for (int c = 0; c < num_cards; c++)	// Iterate over all cards.
	{
		for (int p = 0; p <= num_players; p++) {	// Iterate over all players, including the case file (as a possible place for a card).
			for (int px = 0; px <= num_players; px++) {	// Iterate over all players, including the case file (as a possible place for a card).
				if(p!=px) {
					Clause clause;
					clause.push_back(GetPairNum(p,c) * (-1)); // add -Cp OR -Cpx
					clause.push_back(GetPairNum(px,c) * (-1));
					solver->AddClause(clause);
				}
			}
		}
	}
	
	// At least one card of each category is in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE
	int cf = num_players; // set case file to the number of players p=cf
	// setting ranges for categories of cards in cards array
	int sus_start = 0;
	int sus_end = num_suspects;
	int wea_start = sus_end;
	int wea_end = wea_start + num_weapons;
	int roo_start = wea_end;
	int roo_end = roo_start + num_rooms;

	Clause clause;
	for(int sus_c = sus_start; sus_c < sus_end; sus_c++) { // Iterate over suspect cards
		clause.push_back(GetPairNum(cf,sus_c));
	}
	solver->AddClause(clause);

	clause.clear();
	for(int wea_c = wea_start; wea_c < wea_end; wea_c++) { // Iterate over weapon cards
		clause.push_back(GetPairNum(cf,wea_c));
	}
	solver->AddClause(clause);

	clause.clear();
	for(int roo_c = roo_start; roo_c < roo_end; roo_c++) { // Iterate over room cards
		clause.push_back(GetPairNum(cf,roo_c));
	}
	solver->AddClause(clause);

	// No two cards in each category can both be in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE
	for(int sus_c = sus_start; sus_c < sus_end; sus_c++) { // Iterate over suspect cards
		for(int sus_cx = sus_start; sus_cx < sus_end; sus_cx++) { // Iterate over suspect cards
			if(sus_c!=sus_cx) {
				Clause clause;
				clause.push_back(GetPairNum(cf,sus_c) * (-1));
				clause.push_back(GetPairNum(cf,sus_cx) * (-1));
				solver->AddClause(clause);
				// cout << cards[sus_c] << GetPairNum(cf,sus_c) * (-1) << " ??? " << cards[sus_cx] << endl;
			}
		}
	}
	for(int wea_c = wea_start; wea_c < wea_end; wea_c++) { // Iterate over suspect cards
		for(int wea_cx = wea_start; wea_cx < wea_end; wea_cx++) { // Iterate over suspect cards
			if(wea_c!=wea_cx) {
				Clause clause;
				clause.push_back(GetPairNum(cf,wea_c) * (-1));
				clause.push_back(GetPairNum(cf,wea_cx) * (-1));
				solver->AddClause(clause);
				// cout << cards[wea_c] << GetPairNum(cf,wea_c) * (-1) << " ??? " << cards[wea_cx] << endl;
			}
		}
	}
	for(int roo_c = roo_start; roo_c < roo_end; roo_c++) { // Iterate over suspect cards
		for(int roo_cx = roo_start; roo_cx < roo_end; roo_cx++) { // Iterate over suspect cards
			if(roo_c!=roo_cx) {
				Clause clause;
				clause.push_back(GetPairNum(cf,roo_c) * (-1));
				clause.push_back(GetPairNum(cf,roo_cx) * (-1));
				solver->AddClause(clause);
				// cout << cards[roo_c] << GetPairNum(cf,roo_c) * (-1) << " ??? " << cards[roo_cx] << endl;
			}
		}
	}
}

void ClueReasoner::Hand(string player, string hand_cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). 
	// Remember that the players array is sorted wrt the order that the players play. 
	// Also note that, player_num (not to be confused with num_players) is a private variable of the 
	// ClueReasoner class that is initialized when this function is called.
	// player_num = GetPlayerNum(player);
	
	// TO BE IMPLEMENTED AS AN EXERCISE
	for(int i=0; i<3; i++) {
		Clause clause;
		clause.push_back(GetPairNum(player, hand_cards[i]));
		solver->AddClause(clause);
	}
}

void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	
	// TO BE IMPLEMENTED AS AN EXERCISE
	// your turn and someone refutes
	int suggester_num = GetPlayerNum(suggester);
	int refuter_num = GetPlayerNum(refuter);

	if(refuter != "" && card_shown != "") {
		int tracker = suggester_num;
		tracker++;
		if(tracker >= num_players) {
			tracker = 0;
		}
		while(refuter != players[tracker]) {
			//players[tracker] has none of the 3 cards
			Clause clause;
			clause.push_back(GetPairNum(players[tracker], card1) * (-1));
			solver->AddClause(clause);
			clause.clear();

			clause.push_back(GetPairNum(players[tracker], card2) * (-1));
			solver->AddClause(clause);
			clause.clear();

			clause.push_back(GetPairNum(players[tracker], card3) * (-1));
			solver->AddClause(clause);
			clause.clear();

			tracker++;
			if(tracker >= num_players) {
				tracker = 0;
			}
		}

		Clause clause;
		clause.push_back(GetPairNum(refuter, card_shown));
		solver->AddClause(clause);
	}

	// not your turn and someone refutes
	else if(refuter != "" && card_shown == "") {
		int tracker = suggester_num;
		tracker++;
		if(tracker >= num_players) {
			tracker = 0;
		}
		while(refuter != players[tracker]) {
			//players[tracker] has none of the 3 cards
			Clause clause;
			clause.push_back(GetPairNum(players[tracker], card1) * (-1));
			solver->AddClause(clause);
			clause.clear();

			clause.push_back(GetPairNum(players[tracker], card2) * (-1));
			solver->AddClause(clause);
			clause.clear();

			clause.push_back(GetPairNum(players[tracker], card3) * (-1));
			solver->AddClause(clause);
			clause.clear();

			tracker++;
			if(tracker >= num_players) {
				tracker = 0;
			}
		}

		Clause clause;
		clause.push_back(GetPairNum(refuter, card1));
		clause.push_back(GetPairNum(refuter, card2));
		clause.push_back(GetPairNum(refuter, card3));
		solver->AddClause(clause);
	}

	// no one refutes --> your cards or cf
	else if(refuter == "") {
		Clause clause;
		clause.push_back(GetPairNum(suggester, card1));
		clause.push_back(GetPairNum(case_file, card1));
		solver->AddClause(clause);

		clause.clear();
		clause.push_back(GetPairNum(suggester, card1));
		clause.push_back(GetPairNum(case_file, card1));
		solver->AddClause(clause);
		
		clause.clear();
		clause.push_back(GetPairNum(suggester, card1));
		clause.push_back(GetPairNum(case_file, card1));
		solver->AddClause(clause);
	}
}

void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	// TO BE IMPLEMENTED AS AN EXERCISE (you don't need to implement this)
}

