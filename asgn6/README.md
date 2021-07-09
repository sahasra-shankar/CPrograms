# Assignment 6

This program is the Santa Cruz take on the famous dystopian novel by George Orwell, "1984". By employing a hash table with linked lists and a bloom filter, the program will parse through the user input and determine whether the user is guilty of thoughtcrime, depending on whether or not the words that they use are considered either badspeak or just oldspeak. If the user is guilty simply of oldspeak, then they are just sent to be counseled whereas if they user is guilty of using badspeak or even badspeak AND oldspeak, then they are sent immediately to joycamp. In the case that the user is accused of thoughtcrime, they are promptly informed of which words were badspeak and oldspeak and the translations for the latter.

## Build

	$ clang -Wall -Werror -Wextra -Wpedantic

## Clean

	$ rm -f sorting sorting.o  
