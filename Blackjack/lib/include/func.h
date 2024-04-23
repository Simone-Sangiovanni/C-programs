#define TRUE 1
#define FALSE 0


typedef struct {
    char rank[5];
    char suit[7];
}Card;

typedef struct {
    Card hand[10];
    int value[10];
    int totalValue;
    int insurance;
    int blackjack;
    int bet;
    int wallet;
    char name[7];
}Player;


static const char *rank[] = {"A","2","3","4","5","6","7","8","9","10","J","Q","K"};
static const char *suits[] = {"QUADRI","CUORI","FIORI","PICCHE"};


extern void header();
//init
extern void initDeck(Card *deck);
extern void initPlayer(Player *player, char *name);
extern void initDistribution(Player *player, Player *dealer, Card *deck);
extern void resetPlayer(Player *player);
//game
extern int insurance(Player *player, Player *dealer);
extern void weber(Player *player);
extern void doubleDown(Player *player, Card *deck);
extern void hit(Player *player, Card *deck);
//other
extern void shuffleDeck(Card *deck);
extern void cardDistribution(Card *card, Card *deck);
extern void showCards(Player *player);
extern void cardsValue(Player *player);
extern void gameInfo(Player *player, Player *dealer);
extern void playingHand(Player *player, Player *dealer);
