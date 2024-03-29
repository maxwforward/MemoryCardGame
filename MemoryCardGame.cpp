//####################################################################################################################################
//	Memory Card Game
//	Max Forward
//####################################################################################################################################

// Library Imports
#include <iostream>
#include <iomanip>

// Move everything inside of namespace std to the global scope
using namespace std;

//************************************************************************************************************************************
//	Part 1 - The Setup
//************************************************************************************************************************************

// Create enum for card values
enum cardValue
{
	zero, ace, two, three, four, five, six, seven, eight, nine, ten, jack, queen, king // Sets an integer value 0-13 for each card 
};

// Create enum for suit
enum cardSuit
{
	none, hearts, diamonds, spades, clubs // Sets an integer value for each suit: none=0; hearts=1; diamonds=2; spades=3; clubs=4
};

// Create a struct to represent the cards
struct cardStruct
{
	cardValue value; // card value
	cardSuit suit; // suit
	bool faceUp = false; // Make every card face down at start
};

// Function Declarations
void buildDeck(cardStruct* deck);
cardStruct draw(cardStruct* deck, int &sizeOfDeck);
void shuffleDeck(cardStruct* dealerDeck, int columns);
void printGrid(cardStruct** playArea, int rows, int columns);
void printCardValue(cardStruct card);
void printCardSuit(cardStruct card);
void flipCard(cardStruct** card, int x, int y);
bool cardMatch(cardStruct card1, cardStruct card2);
bool checkForWin(cardStruct** playArea, int rows, int columns);

//====================================================================================================================================
// main function - Menu of Memory Card Game
//====================================================================================================================================
int main()
{
	// Create an array of cardStructs to populate with cards in order to make the deck
	cardStruct deck[52];

	// Create an integer to store the size of the deck (how mny cards are in it)
	int sizeOfDeck = 52;

	// Populate a compile time array with cards
	buildDeck(deck); // Build a standard deck of 52 cards and don't shuffle them

	// Inititialize an integer for number of columns
	int columns;

	// Print the menu
	cout << endl << endl << "Welcome to Memory!" << endl;
	cout << "The rules are simple, pick two cards and see if they match. Continue until you've matched all of the cards!" << endl;
	cout << "You may enter -1 for your cards at any time during the game to quit." << endl << endl;
	// Prompt the user for how many columns they want to play with
	cout << "How many columns of cards do you want to play with?" << endl;
	cout << "Minimum of 3, maximum of 13: ";
	cin >> columns; // get number of columns from user

	// Create a "dealer" array of size 4 * columns for the dealer deck of cards
	// Create a single dimension array of cards to work as a shuffler and dealer
	cardStruct* dealerDeck; // Make a pointer for the dynamic array
	dealerDeck = new cardStruct[4 * columns]; // Dynamically allocate the array based on the number of columns times 4

	// Create a variable to store size of the dealer deck (how many cards are in it)
	int sizeOfDealerDeck = 0;

	// Fill the "dealer" array with cards taken from the deck
	for (int i = 0; i < (4 * columns); i++) // Repeat this loop the amount of times specified by the user
	{
		dealerDeck[i] = draw(deck, sizeOfDeck); // Draw a card from the deck to place in the dealer deck
		sizeOfDealerDeck++; // Increment the size of the dealer deck every time you draw a card
	}

	// Shuffle the cards in the dealer deck
	shuffleDeck(dealerDeck, columns);

	// Create a 2D array for the play-area that will hold cards
	cardStruct** playArea; // Make a pointer to a pointer for the 2D array
	playArea = new cardStruct*[4]; // Allocate the number of rows in a single dimension array of cardStruct*
	for (int row = 0; row < 4; row++)
	{
		// Allocate a single dimension array to each cardStruct* pointer
		playArea[row] = new cardStruct[columns];
	}

	// Deal the cards from the dealer deck into the play-area
	for (int row = 0; row < 4; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			playArea[row][column] = draw(dealerDeck, sizeOfDealerDeck); // Draw a card from dealer deck into the play area
		}
	}

//************************************************************************************************************************************
//	Part 2 - The Game
//************************************************************************************************************************************

	// Check to see if the player has won (all cards turned face up)
	while (!checkForWin(playArea, 4, columns)) // While game has NOT been won...
	{
		// Print the grid of blank cards with coordinates on the outter edge
		printGrid(playArea, 4, columns);

		// Initialize integer variables to store coordinates
		int xCoordinateCard1;
		int yCoordinateCard1;
		int xCoordinateCard2;
		int yCoordinateCard2;

		// Prompt the user for two cards to choose using the coordinates
		cout << endl << endl << "Choose column # for card 1: ";
		cin >> xCoordinateCard1;
		cout << "Choose row # for card 1: ";
		cin >> yCoordinateCard1;
		cout << "Choose column # for card 2: ";
		cin >> xCoordinateCard2;
		cout << "Choose row # for card 2: ";
		cin >> yCoordinateCard2;

		// Check for bad inputs
		// If user enters input that is not an integer...
		if (cin.fail()) // Returns true if input is not correct type
		{
			// Dont allow and ask user to try again
			cout << endl << endl << "Im sorry, you entered an invalid card. Please try again!" << endl;
			cin.clear();
			cin.ignore(1000, '\n');
		}
		// If user enters negative numbers...
		else if (xCoordinateCard1 < 0 || yCoordinateCard1 < 0 || // If user enters negative numbers for card 1...
			xCoordinateCard2 < 0 || yCoordinateCard2 < 0) // If user enters negative numbers for card 2...
		{
			// Quit game
			cout << endl << "Goodbye!" << endl;
			return 0;
		}
		// If user enters same coordinates for cards 1 and 2 (duplicate card choices)...
		else if (xCoordinateCard1 == xCoordinateCard2 && yCoordinateCard1 == yCoordinateCard2)
		{
			// Dont allow and ask user to try again
			cout << endl << "Im sorry, you entered duplicate card choices. Please try again!" << endl;
		}
		// If user enters choices that are out of bounds...
		else if (xCoordinateCard1 > (columns - 1) || yCoordinateCard1 > 3 || // If coordinates for card 1 are out of bounds...
			xCoordinateCard2 > (columns - 1) || yCoordinateCard2 > 3) // If coordinates for card 2 are out of bounds...
		{
			// Don't allow and ask user to try again
			cout << endl << "Im sorry, you entered an invalid card. Please try again!" << endl;
		}

		// If user inputs valid coordinates...
		else
		{
			// Store the cards located at the valid coordinates
			cardStruct card1 = playArea[yCoordinateCard1][xCoordinateCard1];
			cardStruct card2 = playArea[yCoordinateCard2][xCoordinateCard2];

			// If the cards at the valid coordinates have already been revealed...
			if (card1.faceUp == true || card2.faceUp == true) // If one of the cards is face up...
			{
				// Dont allow and ask user to try again
				cout << endl << "Im sorry, at least one of those cards have already been chosen! Please try again" << endl;
			}

			// If the cards at the valid coordinates have NOT been revealed...
			else
			{
				// Flip the cards
				flipCard(playArea, xCoordinateCard1, yCoordinateCard1); // Flip first card
				flipCard(playArea, xCoordinateCard2, yCoordinateCard2); // Flip second card

				// Print the grid to show flipped cards
				printGrid(playArea, 4, columns);

				// Check to see if the flipped cards match
				if (cardMatch(card1, card2)) // If flipped cards match...
				{
					// Congratulate the player and leave flipped cards face up
					cout << endl << endl << "Congratulations! Its a match!" << endl;
				}
				else // If flipped cards do NOT match...
				{
					cout << endl << endl << "Too bad! Not a match!" << endl;

					// Flip cards back face down
					flipCard(playArea, xCoordinateCard1, yCoordinateCard1); // Flip first card
					flipCard(playArea, xCoordinateCard2, yCoordinateCard2); // Flip second card
				}
			}
		}

		// Have user "press a key to continue" to resume the game
		system("pause");

		// Clear the screen by outputting multiple newlines to scroll the console window
		cout << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl 
			<< endl << endl << endl << endl << endl << endl << endl << endl << endl << endl << endl;
	}

	// When the game is won (all cards face-up)
	cout << endl << "Congratulations! You Won!" << endl << endl; // Congratulate the player
}

//************************************************************************************************************************************
//	Fuctions
//************************************************************************************************************************************

//====================================================================================================================================
// buildDeck Function - Builds the deck by populating an array of size 52 with cardStructs
//====================================================================================================================================
void buildDeck(cardStruct* deck)
{
	// Initialize a cardStruct
	cardStruct card;

	// This loop organizes the deck as follows: A of Hearts, A of Diamonds, A of Spades, A of Clubs, 1 of Hearts, 1 of Diamonds...
	for (int i = 0; i < 52; i++)
	{
		card.value = (cardValue)(1 + (i / 4));
		card.suit = (cardSuit)(1 + (i % 4));
		deck[i] = card;
	}
}

//====================================================================================================================================
// draw Function - Draws a card from the deck by removing the first element from the deck array and returning it while also 
// "cleaning up" the array by shuffling everything down 
//====================================================================================================================================
cardStruct draw(cardStruct* deck, int &sizeOfDeck)
{
	// Define a cardStruct to store the first card in the deck
	cardStruct removedCard = deck[0];

	// Define a cardStruct to represent an empty space where there is no card
	cardStruct noCard;
	noCard.value = zero; // Set the value to 0
	noCard.suit = none; // Set the suit to none

	// "Clean up the array"
	// Remove the first element of the array and shuffle everything down
	for (int i = 0; i < (sizeOfDeck - 1); i++) // Repeat this loop until you reach the last element in the array
	{
		deck[i] = deck[i + 1]; // Replace the element of the array with the element after it
	}

	// Set the last element of the array to no card
	deck[sizeOfDeck - 1] = noCard;

	// Decrement the size of the deck
	sizeOfDeck--;

	// Return the card you removed
	return removedCard;
}

//====================================================================================================================================
// shuffleDeck Function - Shuffles the cards in the dealer deck
//====================================================================================================================================
void shuffleDeck(cardStruct* dealerDeck, int columns)
{
	// Shuffle the dealer deck
	for (int i = 0; i < ((4 * columns) / 2); i++) // Repeat this loop "half the size of the deck" amount of times
	{
		// Save 2 different random positions to shuffle
		int randomPosition1 = rand() % (4 * columns); // Random int between 0 and (4 * columns)
		int randomPosition2 = rand() % (4 * columns); // Random int between 0 and (4 * columns)

		// Define 2 card structs to svae the cards you want to shuffle
		cardStruct shuffledCard1 = dealerDeck[randomPosition1];
		cardStruct shuffledCard2 = dealerDeck[randomPosition2];

		// Swap the two cards in order to "shuffle"
		dealerDeck[randomPosition1] = shuffledCard2;
		dealerDeck[randomPosition2] = shuffledCard1;
	}
}

// Output the grid (play-area)
//======================================================================================================================================
void printGrid(cardStruct** playArea, int rows, int columns)
{
	// Print column coordinates
	cout << endl << "###";
	for (int column = 0; column < columns; column++)
	{
		cout << setw(4) << column;
	}

	// Print each row of cards
	for (int row = 0; row < rows; row++)
	{
		// Print row coordinates
		cout << endl << setw(3) << row;

		// Print cards in each row
		for (int column = 0; column < columns; column++)
		{
			// If card is turned over (not face up) print "**"
			if (playArea[row][column].faceUp == false)
			{
				cout << setw(4) << "**"; // print "**" with 2 spaces in front
			}

			// else - if card is fipped over (IS face up) print the card
			else
			{
				// Print correct characters to represent the value and suit
				printCardValue(playArea[row][column]); // Prints card value
				printCardSuit(playArea[row][column]); // Prints card suit
			}
		}
	}
}

// Print the correct character to represent card value
//======================================================================================================================================
void printCardValue(cardStruct card)
{
	switch (card.value)
	{
	case 1: // If integer value is 1, needs to be represented as a "A" for Ace
		cout << setw(3) << "A"; // print "A" with 2 spaces in front
		break;

	case 11: // If integer value is 11, needs to be represented as a "J" for Jack
		cout << setw(3) << "J"; // print "J" with 2 spaces in front
		break;

	case 12: // If integer value is 12, needs to be represented as a "Q" for Queen
		cout << setw(3) << "Q"; // print "Q" with 2 spaces in front
		break;

	case 13: // If integer value is 13, needs to be represented as a "K" for King
		cout << setw(3) << "K"; // print "K" with 2 spaces in front
		break;

	default: // If integer value is equal to anything else, needs to be represented as an integer
		cout << setw(3) << card.value; // prints the integer value of the card
		break;
	}
}

// Print the correct character to represent card suit
//======================================================================================================================================
void printCardSuit(cardStruct card)
{
	switch (card.suit)
	{
	case 1: // If integer value is 1 print Hearts symbol
		cout << "H";
		break;

	case 2: // If integer value is 2 print Diamonds symbol
		cout << "D";
		break;

	case 3: // If integer value is 3 print Spades symbol
		cout << "S";
		break;

	case 4: // If integer value is 4 print Clubs symbol
		cout << "C";
		break;
	}
}

// Flips card
//======================================================================================================================================
void flipCard(cardStruct** card, int x, int y)
{
	// If card is face down, flip it face up
	if (card[y][x].faceUp == false)
	{
		card[y][x].faceUp = true;
	}

	// If card is face up, flip it face down
	else
	{
		card[y][x].faceUp = false;
	}
}

// Check to see if cards match
//======================================================================================================================================
bool cardMatch(cardStruct card1, cardStruct card2)
{
	// If the value of card 1 is equal to the value of card 2, return true
	if (card1.value == card2.value)
	{
		return true;
	}

	// If the value of card 1 is not equal to the value of card 2 return false
	else
	{
		return false;
	}
}

// Check for win by checking if all cards are face up
//======================================================================================================================================
bool checkForWin(cardStruct** playArea, int rows, int columns)
{
	// Initialize a counter to count how many cards are face up
	int counter = 0;

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			if (playArea[row][column].faceUp == true)
			{
				counter++; // increment the counter every time you find a card face up
			}
		}
	}

	// If the number of cards face up is equal to the number of cards in the game return true
	if (counter == (rows * columns))
	{
		return true;
	}

	// If the number of cards face up is not equal to the number of cards in the game return false
	else
	{
		return false;
	}
}