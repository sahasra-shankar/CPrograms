#include "names.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int roll(void) {
    return random() % 6; //generates random from 0-5
}

uint32_t left(uint32_t pos, uint32_t players) {
    return (pos + players - 1) % players;
}

uint32_t right(uint32_t pos, uint32_t players) {
    return (pos + 1) % players;
}

int main(void) {
    int players = 0;
    unsigned int maxseed = 0;
    int firstRoll, secondRoll;
    int lowestRoll = 10;
    int fullRoll[players];
    int rightPlayer, leftPlayer;
    int lowestRP;
    int s = 0;
    printf("Number of players: ");
    scanf("%d", &players);
    if (players < 2 || players > 10) {
        fprintf(stderr, "Invalid number of players. \n");
    } else {
        const char *gamePlayers[players];
        int playLives[players];
        int numOfPlayersPlaying = players; //for incrementing/decrementing to help with condition
        printf("Random seed: ");
        scanf("%d", &maxseed);
        srandom(maxseed); // set maxseed as seed value for rand

        if (maxseed < UINT32_MAX && maxseed > 0) {
            for (int m = 0; m < players; m += 1) {
                playLives[m] = 3;
            }
            while (numOfPlayersPlaying > 1) {
                printf("Round %d\n", s + 1);
                lowestRoll = 10; // set to highest so first roll will automatically set to lowest
                for (int i = 0; i < players; i++) {
                    if (playLives[i] > 0) // players lives should be >0 to roll
                    {
                        gamePlayers[i] = names[i];
                        firstRoll = roll();
                        secondRoll = roll();
                        fullRoll[i] = firstRoll + secondRoll;
                        if (fullRoll[i] < lowestRoll) {
                            lowestRoll = fullRoll[i];
                            lowestRP
                                = i; // player w lowest roll is set to fullRoll index that equals it
                        }
                        if (fullRoll[i] != 10) {
                            printf(" - %s rolls %s...\n", gamePlayers[i],
                                rolls[firstRoll][secondRoll]);
                        }
                        if (fullRoll[i] == 10) {
                            printf(" - %s rolls Midnight...", gamePlayers[i]);
                            rightPlayer
                                = right(i, players); // uses left and right from outside main
                            leftPlayer = left(i, players);
                            playLives[rightPlayer] += 1;
                            playLives[leftPlayer] += 1;
                            if (playLives[rightPlayer] == 1) {
                                printf(" resurrects %s", gamePlayers[rightPlayer]);
                                numOfPlayersPlaying += 1;
                            } else if (playLives[rightPlayer] > 1) {
                                printf(" sparkles %s", gamePlayers[rightPlayer]);
                            }
                            if (playLives[leftPlayer] == 1) {
                                printf(" resurrects %s", gamePlayers[leftPlayer]);
                                numOfPlayersPlaying += 1;
                            } else if (playLives[leftPlayer] > 1) {
                                printf(" sparkles %s", gamePlayers[leftPlayer]);
                            }
                            printf("\n");
                        }
                    }
                    if (i == players - 1) // only after i iterates through to last player
                    {
                        if (playLives[lowestRP] > 0) {
                            playLives[lowestRP] = playLives[lowestRP] - 1;
                            printf("%s is forced to eat garlic!\n", gamePlayers[lowestRP]);
                            if (playLives[lowestRP] == 0) {
                                printf("%s has died.\n", gamePlayers[lowestRP]);
                                numOfPlayersPlaying -= 1;
                            } else if (playLives[lowestRP] == 1) {
                                printf("%s has %d life remaining.\n", gamePlayers[lowestRP],
                                    playLives[lowestRP]);
                            } else {
                                printf("%s has %d lives remaining.\n", gamePlayers[lowestRP],
                                    playLives[lowestRP]);
                            }
                        }
                    }
                }

                s += 1;
            }

            for (int r = 0; r < players; r += 1) {
                if (playLives[r] > 0) {
                    printf("%s wins the Garlic Game!\n", gamePlayers[r]);
                }
            }

        } else {
            fprintf(stderr, "Invalid random seed.\n");
        }
    }
}
