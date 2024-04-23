
/*
REGOLE:
1) il banco chiede carta se ha meno di 17
2) non c'è lo split
3) in caso di vittoria del giocatore con Blackjack la vittoria sarà di 3/2 della puntata
4) per giocare sono necessari almeno 2500$ nel portafogli per poter far fronte a tutte le possibili situazioni di gioco
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lib/include/func.h"


int main(){
    Card deck[52];
    Player player;
    Player dealer;
    int end;	//indica quando terminare la mano
    char choise;

	initPlayer(&dealer, "Dealer");
	initPlayer(&player, "Player");
	initDeck(deck);

    //game loop
    do{
		choise = '\0';
    	end = FALSE;
		resetPlayer(&player);
		resetPlayer(&dealer);

        header();
        //puntata
		weber(&player);
        //distribuzione iniziale delle carte
		shuffleDeck(deck);
        initDistribution(&player, &dealer, deck);

        strcpy(dealer.hand[0].rank, "A");
        strcpy(dealer.hand[1].rank, "10");

		//assegno un valore per ogni carta
		cardsValue(&player);
		cardsValue(&dealer);
		//mostro la prima carta del dealer e tutte le carte del giocatore
		gameInfo(&player, &dealer);

    	//se il giocatore non ha fatto blackjack
        if(player.blackjack == FALSE){
        	//controllo se cè la possibilità di assicurarsi o no
        	end = insurance(&player, &dealer);
	        //se il dealer non ha un asso oppure se si continua a giocare
        	if(end == FALSE){
				do{
		        	printf("\n\nScegli: ");
		        	printf("\n[r] raddoppia");
		        	printf("\n[c] carta");
		        	printf("\n[s] stai");
		        	printf("\n--> ");
		        	scanf("%c", &choise);
		        	while(getchar() != '\n');
		        }while(choise != 'r' && choise != 'c' && choise != 's');

		        switch(choise){
		        	case 'r':{
						choise = '\0';
		        		doubleDown(&player, deck);
		            	cardsValue(&player);
		        		break;
		        	}
		        	case 'c':{
						do{
							choise = '\0';
							hit(&player, deck);
							cardsValue(&player);
							gameInfo(&player, &dealer);
							if(player.totalValue < 21 && choise == '\0'){
								do{
									printf("\n\nScegli: ");
						        	printf("\n[c] carta");
						        	printf("\n[s] stai");
						        	printf("\n--> ");
									scanf("%c", &choise);
									while(getchar() != '\n');
						        }while(choise != 'c' && choise != 's');
							}
						}while(choise == 'c' && player.totalValue <= 21);
		        		break;
		        	}
		        }
				//distribuzione delle carte al dealer solo se il giocatore non ha sballato
				if(player.totalValue <= 21){
					header();
					while(dealer.totalValue < 17){
						hit(&dealer, deck);
						cardsValue(&dealer);
					}
					//controllo dei valori
					if(dealer.totalValue > 21){
						printf("\n\nIl Dealer ha sballato");
						printf("\nVince il Giocatore");
						player.wallet += (player.bet*2);
						dealer.wallet -= player.bet;
					}
					else{
						if(player.totalValue > dealer.totalValue){
							printf("\n\nIl valore delle carte del Giocatore è maggiore di quello delle carte del Dealer");
							printf("\nVince il Giocatore");
							player.wallet += (player.bet*2);
							dealer.wallet -= player.bet;
						}
						if(player.totalValue < dealer.totalValue && dealer.blackjack == FALSE){
							printf("\n\nIl valore delle carte del Dealer è maggiore di quello delle carte del Giocatore");
							printf("\nVince il Dealer");
							dealer.wallet += player.bet;
						}
						if(player.totalValue == dealer.totalValue){
							printf("\n\nIl valore delle carte del Giocatore e di quelle del Dealer si equivalgono");
							printf("\nPareggio");
							player.wallet += player.bet;
						}
						if(dealer.blackjack == TRUE && player.insurance == FALSE){
							printf("\n\nIl Dealer ha un Blackjack");
							printf("\nVince il Dealer");
							dealer.wallet += player.bet;
						}
					}
				}
				else{
					header();
					printf("\n\nIl Giocatore ha sballato");
					printf("\nVince il Dealer");
					dealer.wallet += player.bet;
				}
        	}
        }
		//se il giocatore ha fatto blackjack
		if(player.blackjack == TRUE){
			header();
			if(dealer.blackjack == TRUE){
				printf("\n\nSia il Giocatore che il Dealer hanno fatto Blackjack!");
				printf("\nPAREGGIO");
				player.wallet += player.bet;
			}
			if (dealer.blackjack == FALSE) {
				printf("\n\nIl Giocare ha un Blackjack");
				printf("\nVince il Giocatore");
				player.wallet = player.wallet + ((3/2) * player.bet);
				dealer.wallet = dealer.wallet - ((3/2) * player.bet);
			}
		}
		//mostro tutte le carte
		playingHand(&player, &dealer);

		if (player.wallet < 2500) {
			printf("\n\nHai troppi pochi soldi per giocare! Chiusura del programma...\n");
			choise = '\n';
		}
		else{
			printf("\n\nSe desideri continuare a giocare digita [s], altrimenti digita un qualunqe altro tasto");
			printf("\n--> ");
			scanf("%c", &choise);
			while(getchar() != '\n');
		}
    }while(choise == 's');

    return 0;
}
