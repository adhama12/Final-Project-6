
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>
using namespace std;


enum card_suit {
	S, H, D, C
};





	struct account { 
		string name; 
		string email; 
		string username; 
		string password; 

		int money; 


		int wins; 
		int total_played; 
		double win_percentage; 
	};

	struct card {
		int value; 
		card_suit suit; 
		bool up; 
	};

	struct player { 
		account info; 
		vector<card> hand; 
		int bet; 
		int insurance_bet; 
	};


card deal();
card hitMe();
int score(vector<card>);
bool hasAce(vector<card>);
void printCards(vector<card>);
char printSuit(card);

int winner(player, player&);
void payout(player, player&);

void bet(player&);
void insurance(vector<player>&);
account load();
void save(player);
account create();
void play(vector<player>&);
void clear(vector<card>&);
void dealer_play(player&);

class Menu {
public:
	void displayRules() {
		cout << "The rules of blackjack: You need to know the card values to know how to play 21. Cards 2-10 are worth the value of the number on the face of the card. Numbered cards are worth the corresponding number indicated on the card. Face cards (those with pictures on them) are worth 10, except for the Ace, which is worth 1 or 11. A picture combined with an Ace is Blackjack (a value of 21). The most important blackjack rule is simple: beat the dealer’s hand without going over 21. If you get 21 points exactly on the deal, that is called a “blackjack.” When you’re dealt a blackjack 21, it’s customary to pay out 3:2 or 2:1. That means you win $300 for ever $200 bet at 3:2, or $200 for every $100 bet at 2:1. Clearly. 2:1 is a better payout. Some casinos have moved this down to 6:5 or 7:5, however, this means you’ll get considerably less money over the long haul. A game that pays 1:1 on any kind of a blackjack is usually not even worth looking at. Whether you’re at a land-based casino or playing online blackjack, the gambling table is always laid out the same way. When you learn how to play 21, you will find each player has his or her own assigned betting area, laid out on the table for each seat position. The playing area includes a space for his/her cards, a betting area, and possibly an insurance field or location for a double down bet. The dealer, likewise, has a designated area for his or her cards, plus a “shoe” containing at least one deck of cards. A shoe is a box that might include an automated shuffler to randomly distribute a card each time the dealer removes one for the deal. Traditional land - based casinos, as well as online blackjack casinos, will use between oneand as many as eight decks per game.This helps to thwart those who might be counting cards or are considered “advantage” players who know how to manipulate blackjack rules.While counting cards is legal, a casino will ban anyone it considers to be a highly skilled player capable of imposing an advantage over the house in one or more casino games. Counting cards essentially is the act of tracking the number of high and low - value cards used to better predict a more likely outcome on a particular hand.Now that you know the basic tools of the game, it’s time to examine how to play.The blackjack rules assign numerical blackjack card values to every card. https://www.blackjack.org/blackjack-rules/ " << endl;
	};
	void displayMenu() {
		cout << "Welcome to the game of Blackjack!" << endl;
		cout << "Please select an option below." << endl;
		cout << "1) Rules" << endl;
		cout << "2) Play" << endl;
	}
};

int main() {
	unsigned int time_seed = time(0); 
	srand(time_seed); 
	player user; 
	player dealer; 
	vector<player> players; 
	char input; 

	dealer.info.username = "Dealer"; 
	players.push_back(dealer); 

	int menu; 

	do {
		Menu MyMenu;
		MyMenu.displayMenu(); 
		cin >> menu; 

		if (menu == 1) { 
			MyMenu.displayRules(); 
		}
		else if (menu == 2) { 
			cout << "How many players(0-10)" << endl; 
			int numPlayers; 
			cin >> numPlayers; 
			for (int i = 0; i < numPlayers; i++) { 
				cout << "Are you a new player(N) or existing player(E)?" << endl; 
				cin >> input; 

				if (input == 'N' || input == 'n') { 
					user.info = create(); 
					save(user); 
					players.push_back(user); 
				}
				else if (input == 'E' || input == 'e') { 
					user.info = load(); 
					players.push_back(user); 
				}

			}
		}
		else { 
			cout << "Please enter 0-10" << endl; 
			menu = 0; 
			cin.ignore(); 
		}
	} while (menu != 10); 

	bool cond = false; 
	int size = players.size();
	do {
		for (int i = 1; i < players.size(); i++) {
			if (players[i].info.money < 5) { 
				cout << players[i].info.username << " is currently out of money. Would you like to add some, yes(Y) or (N)?" << endl; //Lets them know they are out of funds and if they want to add some
				cin >> input; 
				if (input == 'Y' || input == 'y') { 
					cout << "How much would you like to add?" << endl; 
					cin >> players[i].info.money;
				}
				else { 
					save(players[i]); 
					players.erase(players.begin() + (i));
					i--;

				}
			}

			if (cond && players.size() > 1) {
				cout << players[i].info.username << " Would you like to keep playing(P) or quit(Q)" << endl;
				cin >> input; 
				if (input == 'q' || input == 'Q') { 
					save(players[i]); 
					players.erase(players.begin() + (i)); 
					i--;
				}
			}
		}
		play(players); 
		cond = true; 
	} while (players.size() > 1);
	

	return 0;
}

void dealer_play(player & dealer) {
	if ((score(dealer.hand) < 17) || (score(dealer.hand) == 17 && hasAce(dealer.hand))) { 
		dealer.hand.push_back(hitMe()); 
	}
}

void play(vector<player> & players) {
	char input; 

	for (int i = 1; i < players.size(); i++) { 
		bet(players[i]); 
		cout << "Money: " << players[i].info.money << setw(10) << "Bet: " << players[1].bet << endl; 
	}

	
	for (int i = 0; i < (players.size() * 2); i++) {
		players[(i % players.size())].hand.push_back(deal());
		if ((i % players.size()) == 0 && (i % 2) == 0) { 
			players[(i % players.size())].hand[(i % 2)].up = false; 
		}
	}
	
	for (int i = 1; i < players.size(); i++) {
		cout << players[i].info.username << " has: " << score(players[i].hand) << setw(10) << endl;
	}

	
	for (int i = 0; i < players.size(); i++) {
		cout << players[i].info.username << " Cards:" << endl;
		printCards(players[i].hand);
	}

	
	bool cont = true; 
	for (int i = 1; i < players.size(); i++) { 
		do {
			if (players[0].hand[1].value == 1 && cont) { //if the dealer has an ace and cont is true - basically this only happens the first time if the dealer doesn't have blackjack
				insurance(players); //asks player if they want insurance
				if (score(players[0].hand) == 21) { //checks to see if the dealer has blackjack - we know the first card is an A
					players[0].hand[0].up = true; //if they do we set the first card to face up

					printCards(players[0].hand); 

					/**
					The below function cycles through the players and pays them out since they lost
					*/
					for (int i = 1; i < players.size(); i++) {
						payout(players[0], players[i]);
					}
					input = 'S'; //sets input to stay since they just lost
				}
				cont = false; //if the dealer didn't have blackjack this is now false
			}
			if (players[0].hand[1].value >= 10 && cont) { //if the dealer has a 10 or face card showing we don't check for insurance but if they have blackjack that's game
				if (score(players[0].hand) == 21) { //if they have blackjack
					players[0].hand[0].up = true; //puts the dealers first card face up

					printCards(players[0].hand); //prints the dealers card
					/**
					The below function pays out the players since they just lost
					*/
					for (int i = 1; i < players.size(); i++) {
						payout(players[0], players[i]);
					}
					input = 'S'; //input is now S since the players lost
				}
				cont = false; //if the dealer doesn't have 21 we don't care about this anymore
			}
			/**
			As long as the players score is less than 21
			*/
			if (score(players[0].hand) <= 21) {
				
				if (((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || players[i].hand[0].value == players[i].hand[1].value) && players[i].hand.size() == 2 && score(players[i].hand) == 10) {
					cout << players[i].info.username << " score: " << score(players[i].hand) << endl; 
					cout << "Would you like to take a hit(H), or stand(S)?" << endl;
				}
				else if (((players[i].hand[0].value >= 10 && players[i].hand[1].value >= 10) || (players[i].hand[0].value == players[i].hand[1].value)) && players[i].hand.size() == 2) { //if they can split their cards
					cout << players[i].info.username << " score: " << score(players[i].hand) << endl; 
					cout << "take a hit(H), or stay(S)?"<< endl; 
				}
				else if (players[i].hand.size() == 2 && score(players[i].hand) >= 9 && score(players[i].hand) <= 11 && !(hasAce(players[i].hand))) { //can they double down
					cout << players[i].info.username << " score: " << score(players[i].hand) << endl; 
					cout << "take a hit(H), or stay(S)?" << endl; 
				}
				else { //they can't do anything special
					cout << players[i].info.username << " score: " << score(players[i].hand) << endl; 
					cout << "Hit(H) or Stay(S), default is to take a stay?"; 
				}
				cin >> input; //takes in the input
				switch (input) {
				
				case 'H':
					players[i].hand.push_back(hitMe()); //we give them one more card for their hit
					printCards(players[i].hand); //reprint their cards
					cout << players[i].info.username << " score is now " << score(players[i].hand) << endl; //reprint their score
					break;
				default: 
					input = 's';
				}
				if (score(players[i].hand) > 21) { 
					input = 'S'; //so we can quit
				}
			}
		} while (!(input == 'S' || input == 's')); 
	}

	dealer_play(players[0]); 

	players[0].hand[0].up = true; //now the everybody can see the first card

	/**
	The below method shows everybody's score and cards including dealers
	*/
	for (int i = 0; i < players.size(); i++) {
		cout << players[i].info.username << " score: " << score(players[i].hand) << " Cards: ";
		printCards(players[i].hand);
	}

	
	for (int i = 1; i < players.size(); i++) {
		if (score(players[i].hand) > 21) { 
			cout << "You busted! ";
		}
		int win = winner(players[0], players[i]); 
		if (win == 1) {
			players[i].info.wins += 1; 
		}
		payout(players[0], players[i]);
		clear(players[i].hand); 
		players[i].info.total_played += 1; 
	}

	clear(players[0].hand); 
}

void clear(vector<card> & hand) {
	hand.clear();
}

account create() {
	account user; 
	cout << "What is the username you'd like to use?" << endl;
	fstream input; 
	string filename; 

	
	do {
		cin >> user.username; 
		filename = user.username + ".dat"; 
		input.open(filename); 
		if (!input.fail()) { 
			cout << "This username is already taken. Please choose another one." << endl; 
		}
	} while (!input.fail()); 

	cout << "Please enter a password." << endl; 
	cin.ignore(); 
	getline(cin, user.password); 
	cin.ignore(); 

	cout << "Please enter your name." << endl; 
	getline(cin, user.name); 

	

	cout << "Please enter how much money you'd like to deposit" << endl; 
	cin >> user.money; 

	user.total_played = 0; 
	user.wins = 0; 
	user.win_percentage = 0; 

	input.close(); 
	return user; 
}

void save(player user) {
	ofstream output; 
	string filename = user.info.username + ".dat"; 

	output.open(filename, ios::out); 

	if (!output) {
		cerr << "Output file will not open and data could not be saved. " << endl;
	}

	double percent = 0; 
	if (user.info.total_played == 0) {
		percent = 0; 
	}
	else {
		percent = ((double)user.info.wins / user.info.total_played) * 100; 
	}
	output << user.info.username << endl << user.info.password << endl << user.info.name << endl << endl << user.info.money << endl << user.info.total_played << endl << user.info.wins << endl << setprecision(3) << percent; //writes the data to the file

	output.close(); 
}


account load() {
	string username, password; 
	string filename; 
	account user; 
	ifstream input;

	
	do {
		cout << "What is your username?" << endl; 
		cin >> username; 
		filename = username + ".dat"; 
		input.open(filename); 
	} while (input.fail()); 
	getline(input, user.username); 
	getline(input, user.password); 

	do {
		cout << "What is your password?" << endl; 
		cin >> password; 
	} while (!(password == user.password)); 

	
	getline(input, user.name);
	input >> user.money;
	input >> user.total_played;
	input >> user.wins;
	input >> user.win_percentage;

	input.close(); 
	return user; 
}

void insurance(vector<player> & players) {
	int size = players.size(); 

	for (int i = 1; i < size; i++) {
		int bet;
		players[i].insurance_bet = 0;
		if ((players[i].info.money - (players[i].bet / 2)) >= 0) { 
			do {
				cin.ignore();
				cout << "How much would you like to bet for insurance, up to " << (players[i].bet / 2) << players[i].info.username << "?(0 if you don't want to take out insurance)" << endl; //asks if they want to make an insurance bet
				cin >> bet; 
			} while (!(bet <= (players[i].bet / 2) && bet >= 0)); 

			if (bet != 0) {
				players[i].insurance_bet = bet; 
				players[i].info.money -= bet; 
			}
		}
	}

	if (score(players[0].hand) == 21) { 
		for (int i = 1; i < size; i++) {
			if (players[i].insurance_bet != 0) { 
				players[i].info.money += (players[i].insurance_bet * 2);
				players[i].insurance_bet = 0; 
			}
		}
	}
	else {
		for (int i = 1; i < size; i++) {
			if (players[i].insurance_bet != 0) { 
				players[i].insurance_bet = 0; 
			}
		}
	}
}


void bet(player & user) {
	int bet;
	do {
		cout << user.info.username << endl; 
		cout << "How much would you like to bet?" << endl;
		cin >> bet; 
	} while (!(bet <= user.info.money)); 
	user.info.money -= bet; 
	user.bet = bet; 
}


void payout(player dealer, player & user) {
	if (winner(dealer, user) == 1) { 
		if (score(user.hand) == 21 && hasAce(user.hand) && user.hand.size() == 2) { 
			user.info.money += ((user.bet * 3) / 2); 
			user.bet = 0;
			cout << user.info.username << " won!" << endl;
		}
		else {
			user.info.money += (user.bet * 2); 
			user.bet = 0; 
			cout << user.info.username << " won!" << endl;
		}
	}
	else if (winner(dealer, user) == 0) { 
		user.info.money += user.bet; 
		user.bet = 0; 
		cout << user.info.username << " tied!" << endl;
	}
	else { 
		user.bet = 0; 
		cout << user.info.username << " lost!" << endl;
	}
}

int winner(player dealer, player & user) {
	if (score(dealer.hand) == score(user.hand)) { 
		return 0;
	}
	else if (((score(dealer.hand) < score(user.hand)) && (score(user.hand) <= 21)) || (score(dealer.hand) > 21 && score(user.hand) <= 21)) { //user won
		return 1;
	}
	else { 
		return -1;
	}
}



char printSuit(card new_card) {
	switch (new_card.suit) {
	case 0:
		return 'S';
	case 1:
		return 'H';
	case 2:
		return 'D';
	case 3:
		return 'C';
	}
}

/**
Prints the cards to the screen
*/
void printCards(vector<card> hand) {
	const string CARD_VALUES[14] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "X" }; //makes it easier to print

	for (int i = 0; i < hand.size(); i++) {
		/**cout << "----------" << endl
		<< "|" << ((hand[i].up)?(hand[i].value):('X')) << ((hand[i].up)?((printSuit(hand[i]))):('X')) << setw(6) << "|" << endl
		<< "|" << setw(8) << "|" << endl
		<< "|" << setw(8) << "|" << endl
		<< "|" << setw(8) << "|" << endl
		<< "|" << setw(6) << ((hand[i].up)?(hand[i].value):('X')) << ((hand[i].up)?(printSuit(hand[i])):('X')) << "|" << endl
		<< "----------" << endl;*/
		if (hand[i].up) { //if the hand is face up we print this
			cout << CARD_VALUES[(hand[i].value - 1)] << printSuit(hand[i]) << " ";
		}
		else { //if it's face down we print XX
			cout << CARD_VALUES[13] << CARD_VALUES[13] << " ";
		}
	}
	cout << endl;
}
/**
Lets us know if the hand has an ace
*/
bool hasAce(vector<card> hand) {
	bool has_ace = false; //For now we say there is no ace in the hand
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i].value == 1) { //we have an ace
			has_ace = true; //so we set this to true
		}
	}

	return has_ace;
}
/**
Gets the score for the user
We treat Aces initially as 1 and then later check to see if the hand contains an Ace
*/
int score(vector<card> hand) {
	int total = 0; //setting up the total value
	for (int i = 0; i < hand.size(); i++) {
		if (hand[i].value >= 10) { //if it's 10, J, Q, or K
			total += 10; //adds 10 to the toal
		}
		else {
			total += hand[i].value; //adds the value to the total
		}
	}

	if (hasAce(hand) && total <= 11) { //if the hand has an ace and we won't bust
		total += 10; //add 10
	}

	return total; //return the total
}
//gets a new card for the player
card hitMe() {
	return deal(); //add another card to the players hand
}
/**
takes in nothing and returns a card
makes a new card and assigns it a random value between 1-13
assigns a suit to the card as well
returns the card
*/
card deal() {
	card new_card; //card we will be returning

	new_card.value = 1 + rand() % 13; //makes sure the random number is between 1 and 13 for the value
	int suit = rand() % 4;
	switch (suit) { //makes sure the random number is between 1 and 4 for the suit
	case 0:
		new_card.suit = S;
		break;
	case 1:
		new_card.suit = H;
		break;
	case 2:
		new_card.suit = D;
		break;
	case 3:
		new_card.suit = C;
		break;
	}

	new_card.up = true; //we'll change it later if it's the dealers face down card

	return new_card; //returning the card
}