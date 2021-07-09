# Assignment 1

This program simulates the Garlic Game when run. It first prompts the user for two inputs : player number and seed value. If valid inputs are given, each player's life count is set to 3. Using the random function, two die are rolled for each player in one round. The player with the lowest roll in each round loses a life. When a player has a life count of 0, they do not roll. In the case that a 12 is rolled(called "Midnight"), the players on the left and right of the player who rolled it either sparkle or get resurrected, if they were dead, and gain a life. Rounds are repeated until all but one vampire are dead. The last remaining undead player wins the game.

## Build 

    $ clang -Wall -Wextra -Werror -Wpedantic -o vampire vampire.c

## Clean

    $ rm -f vampire vampire.o 
