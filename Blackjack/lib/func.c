#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "include/func.h"



void header();
//init
void initDeck(Card *deck);
void initPlayer(Player *player, char *name);
void initDistribution(Player *player, Player *dealer, Card *deck);
void resetPlayer(Player *player);
//game
int insurance(Player *player, Player *dealer);
void weber(Player *player);
void doubleDown(Player *player, Card *deck);
void hit(Player *player, Card *deck);
//other
void cardDistribution(Card *card, Card *deck);
void shuffleDeck(Card *deck);
void showCards(Player *player);
void cardsValue(Player *player);
void gameInfo(Player *player, Player *dealer);
void playingHand(Player *player, Player *dealer);



void header(){
	printf("\n");
	system("clear");
    printf("\n============ BLACKJACK ===========\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// INIT ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

//genero un mazzo ordinato di carte
void initDeck(Card *deck){
	int i = 0;
    for(int k = 0; k < 4; k++){
        for(int j = 0; j < 13; j++){
            strcpy(deck[i].rank, rank[j]);
            strcpy(deck[i].suit, suits[k]);
            i++;
        }
    }
}

void initPlayer(Player *player, char *name){
    for(int i = 0; i < 10; i++){
        strcpy(player->hand[i].rank, "null");
        strcpy(player->hand[i].suit, "null");
        player->value[i] = 0;
    }
    player->insurance = 0;
    player->blackjack = FALSE;
    player->bet = 0;
    player->totalValue = 0;
    player->wallet = 100000;
    strcpy(player->name, name);
}

//distribuzione iniziale delle carte
void initDistribution(Player *player, Player *dealer, Card *deck){
    for(int i = 0; i < 2; i++){
        cardDistribution(&player->hand[i], &*deck);
        cardDistribution(&dealer->hand[i], &*deck);
    }
}

void resetPlayer(Player *player){
	for(int i = 0; i < 10; i++){
		strcpy(player->hand[i].rank, "null");
		strcpy(player->hand[i].suit, "null");
		player->value[i] = 0;
	}
	player->insurance = 0;
	player->blackjack = FALSE;
	player->bet = 0;
	player->totalValue = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// GAME ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void weber(Player *player){
	int maxBet = player->wallet/2;
	printf("\nPortafogli: %d", player->wallet);
    do{
		printf("\nEffettua la tua puntata (min 1000 - max %d): ", maxBet);
        scanf("%d", &player->bet);
        while(getchar() != '\n');
    }while(player->bet < 1000 || player->bet > maxBet);
	player->wallet -= player->bet;
}

int insurance(Player *player, Player *dealer){
	int state = 0;  //valore di ritorno
    char choise;
    //se il dealer ha un asso come prima carta chiedo al giocatore se vuole assicurarsi
    if(strcmp(dealer->hand[0].rank, "A") == 0){
        do{
            printf("\nIl Dealer ha un ASSO, desideri assicurarti? [s/n]: ");
            scanf("%c", &choise);
            while(getchar() != '\n');
        }while(choise != 's' && choise != 'S' && choise != 'n' && choise != 'N');
        printf("\nAssicurazione: %d\n", player->bet/2);
        if(choise == 's'){
            player->insurance = player->bet / 2;
			player->wallet -= player->insurance;
			if(dealer->blackjack == 1){
				printf("\nIl Dealer ha un Blackjack, la mano è terminata");
				player->wallet += player->bet;
				player->wallet += player->insurance;
				state = 1;
			}
			else{
				printf("\nIl Dealer non ha un blackjack, si continua a giocare");
				dealer->wallet += player->insurance;
				player->insurance = 0;
			}
        }
    }
    return state;	//se il dealer ha un blackjack e il giocatore si è assicurato ritorna 1, altrimenti 0
}

void doubleDown(Player *player, Card *deck){
	player->wallet -= player->bet;
	player->bet *= 2;
    cardDistribution(&player->hand[2], &*deck);
}

void hit(Player *player, Card *deck){
    int i;
    for(i = 0; i < 10 && strcmp(player->hand[i].rank, "null") != 0; i++);
    cardDistribution(&player->hand[i], &*deck);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////// OTHER ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

void shuffleDeck(Card *deck){
    int swapper = 0; //indice della carta da scambiare
    Card temp = {"", ""}; //variabile temporanea per la carta da scambiare
    srand(time(NULL)); //inizializzo il seme per la generazione randomica di umeri
    for(int k = 0; k < 100; k++){    //mescolo 100 volte il mazzo
        for(int i = 0;i < 52; i++){
            //genera un numero casuale fra 0 e 51 compresi -> cioè le 52 carte del mazzo
            swapper = rand() % 52;
            //scambia 2 carte (quella corrente con quella generata random)
            temp = deck[i];
            deck[i] = deck[swapper];
            deck[swapper] = temp;
        }
    }
}

void cardDistribution(Card *card, Card *deck){
    int i;  //indice della carta nel mazzo
    for(i = 0; strcmp(deck[i].rank, "null") == 0; i++);
    strcpy(card->rank, deck[i].rank);
    strcpy(card->suit, deck[i].suit);
    //una volta data la carta, cambio il suo valore nel mazzo con null
    strcpy(deck[i].rank, "null");
    strcpy(deck[i].suit, "null");
}

//mostra tutte le carte nella mano del giocatore passato
void showCards(Player *player){
    for(int i = 0; i < 10 && strcmp(player->hand[i].rank, "null") != 0; i++){
        printf("\n  %s %s", player->hand[i].rank, player->hand[i].suit);
    }
}

void cardsValue(Player *player){
    player->totalValue = 0; //setto il valore totale delle carte a 0
    for(int i = 0; i < 10 && strcmp(player->hand[i].rank, "null") != 0; i++){   //per ogni carta nella mano del giocatore passato
        //se la carta è un asso
        if(strcmp(player->hand[i].rank, "A") == 0){
            if(player->totalValue <= 10){
                player->value[i] = 11;
            }
            else{
                player->value[i] = 1;
            }
        }
        //se la carta non è un asso
        else{
            if(strcmp(player->hand[i].rank, "10") == 0 || strcmp(player->hand[i].rank, "J") == 0 || strcmp(player->hand[i].rank, "Q") == 0 || strcmp(player->hand[i].rank, "K") == 0){
                player->value[i] = 10;
            }
            else{ //se il valore della carta è compreso tra 2 e 9
                player->value[i] = atoi(player->hand[i].rank); //converte una stringa in un intero
            }
        }
        player->totalValue += player->value[i];
    }
    //se il giocatore ha fatto blackjack riporto il valore 1 nella relativa variabile
    if(player->totalValue == 21 && player->value[2] == 0){
        player->blackjack = 1;
    }
}

//mostra la prima carta del dealer, tutte le carte del giocatore, valore totale delle carte, portafogli e puntata del giocatore
void gameInfo(Player *player, Player *dealer){
	header();
	printf("\nPortafogli: %d", player->wallet);
	printf("\nPuntata: %d", player->bet);
	printf("\n\nCarte del Dealer:");
	printf("\n  %s %s", dealer->hand[0].rank, dealer->hand[0].suit);
	printf("\n\nLe tue carte:");
	showCards(&*player);
	printf("\nValore: %d", player->totalValue);
}

//mostra la puntata, il portafogli, tutte le carte ch sono state distribuite e i valori della mano del dealer e del giocatore
void playingHand(Player *player, Player *dealer){
	printf("\n\nPortafogli: %d", player->wallet);
	printf("\nPuntata: %d", player->bet);
	printf("\n\nCarte del Dealer:");
	showCards(&*dealer);
	printf("\nValore: %d", dealer->totalValue);
	printf("\n\nLe tue carte:");
	showCards(&*player);
	printf("\nValore: %d", player->totalValue);
}
