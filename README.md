# Othelo Game
This program is a simple implementation of the game Othelo to be run from the command line.
<p float="center">
  <img src="./img 1.png" width=49% />
  <img src="./img 2.png" width=43% /> 
</p>

## Rules of the game

* In Othello, the two colours are Black and White and Black always plays first.
* The four squares in the middle of the board start with four counters already placed - white top left and bottom right; black top right and bottom left.
* A player without a move simply passes, and the other player keeps playing pieces until the first player can make a move again.


## Installation

Execute the following command to clone the source code and open the folder called Game-of-Life.

```
git clone git@github.com:crafaelgil/Othelo-Game.git
```
Then, inside the directory, execute the following command to compile program:

```bash
gcc -c othelo.c && gcc -o othelo.o othelo.c
```
To run the program simply type 
```bash
./othelo.o
```
## Usage
Right after being executed the propmt will ask you choose which player (1 or 2) you want to play. Player 1 places o and goes first. Player 2 places x and goes second.

To place a 'stone' (o or x) you need to manually enter the x and y coordinate.