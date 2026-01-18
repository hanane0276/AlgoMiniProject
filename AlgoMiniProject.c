#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Records part

typedef struct
{
    // Player information
    int number;
    int age;
    char name[50];

    // Part 1 variables
    int totalWins1;
    int totalLosses1;
    int streakWins1;

    // Part 2 variables
    int totalWins2;
    int totalLosses2;
    int streakWins2;

    // From both parts
    int score;
    int totalScore;

    // follow the queue
    int priority;
} Player;

typedef struct 
{
  int roundNum;
  Player player1;
  Player player2;

  time_t startTime;
  time_t endTime;

} Round;

// Queue

typedef struct Node
{
    Player info;
    struct Node *next;
} Node;

typedef struct
{
    Node *head;
    Node *tail;
} Queue;

// Queue Functions

void initQueue(Queue *q){
    q->head = NULL;
    q->tail = NULL;
}

bool isEmptyQueue (Queue q){
    return (q.head == NULL);
}

void enQueue (Queue *q , Player p){
    Node *newPlayer = malloc(sizeof(Node));
    if (newPlayer == NULL){
        return; // allocation error
    }
    newPlayer->info = p;
    newPlayer->next = NULL;
    if (q->tail == NULL){
        q->head = newPlayer;
        q->tail = newPlayer;
    } else {
        q->tail->next = newPlayer;
        q->tail = newPlayer;
    }
}

void deQueue(Queue *q,Player *p){
    if (isEmptyQueue(*q)){
        return;
    }
    Node *temp = q->head;
    *p = temp->info;
    q->head = q->head->next;
    if (q->head == NULL){
        q->tail = NULL;
    }
    free(temp);
}

// display Functions

void displayPlayer(Player p){
    printf("(%d, %s, age=%d, W1=%d, L1=%d, totalScore=%d, prio=%d)",
           p.number, p.name, p.age,p.totalWins1, p.totalLosses1,
           p.totalScore, p.priority);
}

void displayQueueF(Queue q){
    printf("=== Queue F ===\n");
    if (isEmptyQueue(q)){
        printf("Empty \n");
        return;
    }
    Node *current = q.head;
    while (current !=NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayQueueF1(Queue q){
    printf("=== Queue F1 ===\n");
    if (isEmptyQueue(q)){
        printf("Empty \n");
        return;
    }
    Node *current = q.head;
    while (current !=NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayQueueF3(Queue q){
    printf("=== Queue F3 ===\n");
    if (isEmptyQueue(q)){
        printf("Empty \n");
        return;
    }
    Node *current = q.head;
    while (current !=NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayLG(Node *LG){
    printf("=== List LG (winners) ===\n");
    if (LG == NULL){
        printf("Empty \n");
        return;
    }
    Node *current = LG;
    while (current !=NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayLP(Node *LP){
    printf("=== List LP (Losers) ===\n");
    if (LP == NULL){
        printf("Empty \n");
        return;
    }
    Node *current = LP;
    while (current !=NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

// display the Time

void displayStartTime(time_t t){
    struct tm *tm_info;
    char buffer[30];

    tm_info = localtime(&t);
    strftime(buffer,sizeof(buffer), " %Y-%m-%d %H:%M:%S ",tm_info);
    printf("Round start at: %s\n",buffer);
}

void displayEndTime(time_t t){
    struct tm *tm_info;
    char buffer[30];

    tm_info = localtime(&t);
    strftime(buffer,sizeof(buffer), " %Y-%m-%d %H:%M:%S ",tm_info);
    printf("Round End at: %s\n",buffer);
}

// Starting the game

void gameStart(int *n){
    printf("                 WELCOME TO OUR GAME                ");
    printf("\n\n\n\n");
    printf("Enter The Player Number: ");
    do {
       scanf("%d", n);
    } while (*n <=1);
}

// Fill the F queue and player information

int randomAge(int min,int max){
    return min + rand() % (max - min +1);
}

void createTheQueue(Queue *F , int n){
    initQueue(F);
    for (int i = 0; i < n; i++){
        Player p;

        p.number = i+1;
        p.age = randomAge(18,50);
        sprintf(p.name, "Player%d", i+1);

        p.totalLosses1 = 0;
        p.totalWins1 = 0;
        p.streakWins1 = 0;

        p.totalWins2 = 0;
        p.totalLosses2 = 0;
        p.streakWins2 = 0;

        p.score = 0;
        p.totalScore = 0;
        p.priority = 0;
        enQueue(F,p);
    }
}

// Lists Functions

void addToLp(Node **LP,Player p){
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL){
        return;
    }
    newNode->info = p;
    newNode->next = NULL;

    if (*LP == NULL){
        *LP = newNode;
    } else {
        Node *cur = *LP;
        while (cur->next !=NULL){
            cur = cur->next;
        }
        cur->next = newNode;
    }
}

void insertInDescendding(Node **LG,Player p){
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL){
        return;
    }
    newNode->info = p;
    newNode->next = NULL;

    if (*LG == NULL || p.totalScore >= (*LG)->info.totalScore){
        newNode->next = *LG;
        *LG = newNode;
        return;
    }
    Node *cur = *LG;
    while (cur->next != NULL && cur->next->info.totalScore > p.totalScore){
        cur = cur->next;
    }
    newNode->next = cur->next;
    cur->next = newNode;
}

// Part 1 Functions

int RandomVAlue(){
    return rand() % 10000;
}

int digitSum(int x){
    int sum = 0;
    if (x < 0){
        x = -x;
    }
    while (x > 0){
        sum+= x % 10;
        x /= 10;
    }
    return sum;
}
//use boolean to check if u really get players
bool getAPlayer(Queue *F ,Queue *F1,Queue *F3,
                int *nbF, int *nbF1, int *nbF3, Player *p){
    if (!isEmptyQueue(*F1)){
        deQueue(F1,p);
        p->priority = 1;
        (*nbF1)--;
        return true;
    } else if (!isEmptyQueue(*F)){
        deQueue(F,p);
        p->priority = 0;
        (*nbF)--;
        return true;
    } else if (!isEmptyQueue(*F3)){
        deQueue(F3,p);
        p->priority = 2;
        (*nbF3)--;
        return true;
    }
    return false;
}

bool selecteTwo(Queue *F ,Queue *F1,Queue *F3,
                int *nbF, int *nbF1, int *nbF3,
                Player *p1,Player *p2){
    if (!getAPlayer(F,F1,F3,nbF,nbF1,nbF3,p1)) return false;
    if (!getAPlayer(F,F1,F3,nbF,nbF1,nbF3,p2)) return false;
    return true;
}

void part1Round(int roundNum,Player *p1,Player *p2){
    int randNum;
    int sum;
    int currentPlayer = 1;
    int i = 1;
    printf("========== Round %d (Part 1) ==========\n", roundNum);
    printf("Player 1: %s (score: %d) \t\t VS \t\t", p1->name, p1->score);
    printf("Player 2: %s (score: %d)\n\n", p2->name, p2->score);
    while (i <= 12 && abs(p1->score - p2->score) < 3){
        randNum = RandomVAlue();
        sum = digitSum(randNum);
        if (sum % 5 == 0 && currentPlayer == 1){
            p1->score++;
            printf("Player %s ,Random number = %d ,sum of digit = %d , New Score congratolation Your score now = %d \n",
                   p1->name,randNum,sum,p1->score);
        }
        else if (sum % 5 == 0 && currentPlayer == 2){
            p2->score++;
            printf("Player %s ,Random number = %d ,sum of digit = %d , New Score congratolation Your score now = %d \n",
                   p2->name,randNum,sum,p2->score);
        }
        else{
            printf("No point scored.\n");
        }
        i++;
        currentPlayer = 3- currentPlayer;
    }
    printf("\n----- End of Round %d -----\n", roundNum);
    printf("%s final score: %d\n", p1->name, p1->score);
    printf("%s final score: %d\n\n", p2->name, p2->score);
}

void updateAfterPart1(Player  *winner,Player *loser){
    // winner update
    winner->totalWins1++;
    if (winner->priority != 1){
       winner->streakWins1++;
       if (winner->streakWins1 >=3){
        winner->priority = 1;
       }
    }
    winner->totalScore+=winner->score;
    winner->score = 0;
    // loser update
    loser->totalLosses1++;
    loser->streakWins1 = 0;
    loser->totalScore+= loser->score;
    loser->score = 0;
}

void updateDrawPart1(Player *p1,Player *p2){
    p1->streakWins1 = 0;
    p2->streakWins1 = 0;
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

void placePlayerPart1(Player p,Queue *F,Queue *F1,Queue *F3,
                      int *nbF,int *nbF1, int *nbF3,
                      Node **LG, Node **LP){
    if (p.totalLosses1 >= 5){
        addToLp(LP,p);
        return;
    }
    if (p.totalWins1 >= 5){
        insertInDescendding(LG,p);
        return;
    }
    if (p.totalLosses1 >= 3){
        p.priority = 2;
        enQueue(F3,p);
        (*nbF3)++;
    }
    else if (p.priority == 1){
        enQueue(F1,p);
        (*nbF1)++;
    }
    else{
        p.priority = 0;
        enQueue(F,p);
        (*nbF)++;
    }
}

// Count players

int countQueue(Queue q){
    int count = 0;
    Node *current = q.head;
    while (current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

int allQueueCount(Queue F,Queue F1,Queue F3){
    return countQueue(F)+countQueue(F1)+countQueue(F3);
}

// Part 2

int gcd(int a,int b){
    if (a<0){
        a = -a;
    }
    if (b<0){
        b = -b;
    }
    while (b!= 0){
        int r = a%b;
        a = b;
        b = r;
    }
    return a;
}

bool containDigit(int x,int d){
    if (x < 0){ x = -x;}
    if (x == 0 && d == 0){ return true;}
    while (x > 0) {
        if (x % 10 == d) return true;
        x /= 10;
    }
    return false;
}

bool hasDigit(int g,int a,int b){
    int d;
    if (g < 0){
        g = -g;
    }
    if (g == 0){
        return containDigit(a , 0) || containDigit(b ,0);
    }

    while (g > 0){
        d =g % 10;
        if (containDigit(a,d) || containDigit(b,d)){
            return true;
        }
        g /=10;
    }
    return false;
}

void part2Round (int roundNum ,Player *p1 ,Player *p2){
    int a,b;
    int g;
    int cuurentPlayer = 1;
    int i =1;
    printf("========== Round %d (Part 2) ==========\n", roundNum);
    printf("Player 1: %s (score: %d) \t\t VS \t\t", p1->name,p1->score);
    printf("Player 2: %s (score: %d)\n\n", p2->name, p2->score);

    while (i <= 16 && abs(p1->score - p2->score) < 3){
        a = RandomVAlue();
        b = RandomVAlue();
        g = gcd(a,b);

        if (cuurentPlayer == 1){
            if (hasDigit(g,a,b)){
                p1->score++;
                printf("Player %s: x=%d, y=%d, gcd=%d -> New score = %d\n",
                       p1->name, a,b, g, p1->score);
            }
            else{
                printf("Player %s: x=%d, y=%d, gcd=%d -> No point. Score = %d\n",
                       p1->name,a,b, g, p1->score);
            }
        }
        else{
            if (hasDigit(g,a,b)){
                p2->score++;
                printf("Player %s: x=%d, y=%d, gcd=%d -> New score = %d\n",
                       p2->name, a,b, g, p2->score);
            }
            else{
                printf("Player %s: x=%d, y=%d, gcd=%d -> No point. Score = %d\n",
                       p2->name,a,b, g, p2->score);
            }
        }
        i++;
        cuurentPlayer = 3- cuurentPlayer;
    }
    printf("\n----- End of Round %d (Part 2) -----\n", roundNum);
    printf("%s final score: %d\n", p1->name, p1->score);
    printf("%s final score: %d\n\n", p2->name, p2->score);
}

void updateAfterPart2(Player  *winner,Player *loser){
    // winner update
    winner->totalWins2++;
    winner->streakWins2++;
    winner->totalScore+=winner->score;
    winner->score = 0;
    // loser update
    loser->totalLosses2++;
    loser->streakWins2 = 0;
    loser->totalScore+= loser->score;
    loser->score = 0;
}

void updateDrawPart2(Player *p1,Player *p2){
    p1->streakWins2 = 0;
    p2->streakWins2 = 0;
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

void placePlayerPart2(Player p,Queue *F,Queue *F1,Queue *F3, int *nbF,int *nbF1, int *nbF3,Node **LG, Node **LP){
    if (p.totalLosses2 >=2){
        addToLp(LP,p);
        return;
    }
    if (p.streakWins2 >=2){
        insertInDescendding(LG,p);
        return;
    }
    if (p.priority == 2){
        enQueue(F3,p);
        (*nbF3)++;
    }
    else if (p.priority == 1){
        enQueue(F1,p);
        (*nbF1)++;
    }
    else{
        enQueue(F,p);
        (*nbF)++;
    }
}

void updateMaxCasePart2(Player *p1, Player *p2){
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

// final part: stats

void displayTop3(Node *LG){
    printf("=== Top 3 winners (global) ===\n");
    if (LG == NULL){
        printf("No winners.\n");
        return;
    }
    int k = 0;
    Node *cur = LG;
    while (cur != NULL && k < 3){
        Player p = cur->info;
        printf("#%d: %s (num=%d, totalScore=%d, W1=%d, W2=%d)\n",
               k+1, p.name, p.number, p.totalScore, p.totalWins1, p.totalWins2);
        cur = cur->next;
        k++;
    }
}

void showNoWinPart1(Node *LG, Node *LP){
    printf("=== Players with 0 wins in Part I ===\n");

    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins1 == 0){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }

    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins1 == 0){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }
}

void showWinsPart1(Node *LG, Node *LP, int k){
    printf("=== Players with %d wins in Part I ===\n", k);

    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins1 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }

    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins1 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }
}

void showLossesPart1(Node *LG, Node *LP, int k){
    printf("=== Players with %d losses in Part I ===\n", k);

    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalLosses1 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }

    cur = LP;
    while (cur != NULL){
        if (cur->info.totalLosses1 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }
}

void showWinsPart2(Node *LG, Node *LP, int k){
    printf("=== Players with %d wins in Part II ===\n", k);

    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins2 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }

    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins2 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }
}

void showLossesPart2(Node *LG, Node *LP, int k){
    printf("=== Players with %d losses in Part II ===\n", k);

    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalLosses2 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }

    cur = LP;
    while (cur != NULL){
        if (cur->info.totalLosses2 == k){
            displayPlayer(cur->info);
            printf("\n");
        }
        cur = cur->next;
    }
}

int main(){
    srand(time(NULL));
    int n,roundNum;
    int nbF,nbF1,nbF3;
    bool hasWinner = false;
    Player p1,p2,winner,loser;
    time_t startTime,endTime;

    // The queues
    Queue F,F1,F3;
    initQueue(&F);
    initQueue(&F1);
    initQueue(&F3);

    // The lists
    Node *LG = NULL;
    Node *LP = NULL;

    gameStart(&n);

    // create the first queue F
    createTheQueue(&F,n);
    nbF = n;
    nbF1 = 0;
    nbF3 = 0;

    // Part 1
    roundNum = 1;
    while (roundNum <= 3*n && (!isEmptyQueue(F) || !isEmptyQueue(F1)|| !isEmptyQueue(F3))){

        startTime = time(NULL);

        if (!hasWinner){
           if (!selecteTwo(&F,&F1,&F3,&nbF,&nbF1,&nbF3,&p1,&p2)){
               break;
           }
        }
        else{
            p1 = winner;
            if (!getAPlayer(&F,&F1,&F3,&nbF,&nbF1,&nbF3,&p2)){
                placePlayerPart1(p1,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                hasWinner = false;
                break;
            }
        }

        part1Round(roundNum,&p1,&p2);

        endTime = time(NULL);

        displayStartTime(startTime);
        displayEndTime(endTime);

        if (p1.score > p2.score){
            winner = p1;
            loser  = p2;
            printf("The winner of this round is the player %s with score of %d \n",
                   winner.name,winner.score);
            updateAfterPart1(&winner,&loser);
            placePlayerPart1(loser,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);

            if (winner.totalWins1>= 5){
                placePlayerPart1(winner,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                hasWinner = false;
            }
            else{
                hasWinner = true;
            }
        }
        else if (p2.score > p1.score){
            winner = p2;
            loser  = p1;
            printf("The winner of this round is the player %s with score of %d \n",
                   winner.name,winner.score);
            updateAfterPart1(&winner,&loser);
            placePlayerPart1(loser,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);

            if (winner.totalWins1>= 5){
                placePlayerPart1(winner,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                hasWinner = false;
            }
            else{
                hasWinner = true;
            }
        }
        else{
            printf("This round is a draw both player got %d point \n", p1.score);
            updateDrawPart1(&p1,&p2);
            p1.priority = 0;
            p2.priority = 0;
            enQueue(&F,p1);
            enQueue(&F,p2);
            nbF+=2;
            hasWinner = false;
        }

        printf("===== State after round %d =====\n\n",roundNum);
        displayQueueF(F);  printf("\n");
        displayQueueF1(F1);printf("\n");
        displayQueueF3(F3);printf("\n");
        displayLG(LG);     printf("\n");
        displayLP(LP);     printf("\n");
        printf("===================================\n\n");
        roundNum++;
    }

    if (hasWinner){
        placePlayerPart1(winner,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
        hasWinner = false;
    }

    int remain = allQueueCount(F,F1,F3);
    if (remain == 1){ // one player remain
        Player lastPlayer;
        if (!isEmptyQueue(F1)){
            deQueue(&F1,&lastPlayer);
            nbF1--;
            insertInDescendding(&LG,lastPlayer);
        }
        else if(!isEmptyQueue(F)){
            deQueue(&F,&lastPlayer);
            nbF--;
            addToLp(&LP,lastPlayer);
        }
        else if (!isEmptyQueue(F3)){
            deQueue(&F3,&lastPlayer);
            nbF3--;
            addToLp(&LP,lastPlayer);
        }
    }
    else{ // part 2
        printf("===================================\n\n");
        printf("===== Change Strategy to Part II =====\n\n");

        roundNum = 1;
        while (roundNum <= 2*n && (!isEmptyQueue(F) || !isEmptyQueue(F1)|| !isEmptyQueue(F3))){

            startTime = time(NULL);

            if (!selecteTwo(&F,&F1,&F3,&nbF,&nbF1,&nbF3,&p1,&p2)){
                break;
            }
            part2Round(roundNum,&p1,&p2);

            endTime = time(NULL);
            displayStartTime(startTime);
            displayEndTime(endTime);

            if (abs(p1.score - p2.score) >=3 ){
                if (p1.score > p2.score){
                    winner = p1;
                    loser  = p2;
                }
                else{
                    winner = p2;
                    loser  = p1;
                }
                printf("The winner of this round (Part 2) is the player %s with score of %d \n",
                       winner.name, winner.score);
                updateAfterPart2(&winner ,&loser);
                winner.priority = 1;
                placePlayerPart2(winner,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                loser.priority = 2;
                placePlayerPart2(loser,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
            }
            else{
                if (p1.score > p2.score){
                    updateMaxCasePart2(&p1,&p2);
                    p1.priority = 0;
                    placePlayerPart2(p1,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                    p2.priority = 2;
                    placePlayerPart2(p2,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                }
                else if (p2.score > p1.score){
                    updateMaxCasePart2(&p1,&p2);
                    p2.priority = 0;
                    placePlayerPart2(p2,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                    p1.priority = 2;
                    placePlayerPart2(p1,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                }
                else{
                    printf("This round (Part 2) is a draw, both players got %d points\n", p1.score);
                    updateDrawPart2(&p1,&p2);
                    p1.priority = 0;
                    p2.priority = 0;
                    placePlayerPart2(p1,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                    placePlayerPart2(p2,&F,&F1,&F3,&nbF,&nbF1,&nbF3,&LG,&LP);
                }
            }

            printf("===== State after round %d (Part 2) =====\n\n",roundNum);
            displayQueueF(F);   printf("\n");
            displayQueueF1(F1); printf("\n");
            displayQueueF3(F3); printf("\n");
            displayLG(LG);      printf("\n");
            displayLP(LP);      printf("\n");
            printf("===================================\n\n");

            roundNum++;
        }

        if (roundNum > 2*n && (!isEmptyQueue(F) || !isEmptyQueue(F1)|| !isEmptyQueue(F3))){
            Player p;
            while (!isEmptyQueue(F1)){
                deQueue(&F1,&p);
                insertInDescendding(&LG,p);
            }
            while (!isEmptyQueue(F)){
                deQueue(&F,&p);
                addToLp(&LP,p);
            }
            while (!isEmptyQueue(F3)){
                deQueue(&F3,&p);
                addToLp(&LP,p);
            }
        }
    }

    printf("===== Final statistics =====\n");

    displayTop3(LG);
    showNoWinPart1(LG,LP);
    for(int i = 1; i < 4; i++){
        showWinsPart1(LG, LP, i);
        showLossesPart1(LG, LP, i);
    }
    for(int i = 1; i <= 2; i++){
        showWinsPart2(LG, LP, i);
        showLossesPart2(LG, LP, i);
    }

    return 0;
}
