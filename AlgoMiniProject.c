#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

// Player structure
typedef struct
{
    int number;
    int age;
    char name[50];
    
    // Part 1 variables
    int totalWins1;
    int totalLosses1;
    int streakWins1;
    int streakLosses1;
    
    // Part 2 variables
    int totalWins2;
    int totalLosses2;
    int streakWins2;
    
    // Scores
    int score;
    int totalScore;
    
    // Priority queue tracking
    int priority;
} Player;

// Round record
typedef struct {
    int roundNum;
    Player player1;
    Player player2;
    int part;
    int finalScore1;
    int finalScore2;
    time_t startTime;
    time_t endTime;
} Round;

typedef struct RoundNode {
    Round info;
    struct RoundNode *next;
} RoundNode;

// Queue node
typedef struct Node
{
    Player info;
    struct Node *next;
} Node;

// Queue structure
typedef struct
{
    Node *head;
    Node *tail;
} Queue;

//QUEUE FUNCTIONS 

void initQueue(Queue *q){
    q->head = NULL;
    q->tail = NULL;
}

bool isEmptyQueue(Queue q){
    return (q.head == NULL);
}

void enQueue(Queue *q, Player p){
    Node *newPlayer = malloc(sizeof(Node));
    if (newPlayer == NULL) return;
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

void deQueue(Queue *q, Player *p){
    if (isEmptyQueue(*q)) return;
    Node *temp = q->head;
    *p = temp->info;
    q->head = q->head->next;
    if (q->head == NULL){
        q->tail = NULL;
    }
    free(temp);
}

int countQueue(Queue q){
    int count = 0;
    Node *current = q.head;
    while (current != NULL){
        count++;
        current = current->next;
    }
    return count;
}

int allQueueCount(Queue F, Queue F1, Queue F3){
    return countQueue(F) + countQueue(F1) + countQueue(F3);
}

//DISPLAY FUNCTIONS

void displayPlayer(Player p){
    printf("(%d, %s, age=%d, W1=%d, L1=%d, W2=%d, L2=%d, totalScore=%d, prio=%d)",
           p.number, p.name, p.age, p.totalWins1, p.totalLosses1,
           p.totalWins2, p.totalLosses2, p.totalScore, p.priority);
}

void displayQueueF(Queue q){
    printf("=== Queue F ===\n");
    if (isEmptyQueue(q)){
        printf("Empty\n");
        return;
    }
    Node *current = q.head;
    while (current != NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayQueueF1(Queue q){
    printf("=== Queue F1 (Priority 1) ===\n");
    if (isEmptyQueue(q)){
        printf("Empty\n");
        return;
    }
    Node *current = q.head;
    while (current != NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayQueueF3(Queue q){
    printf("=== Queue F3 (Priority 3) ===\n");
    if (isEmptyQueue(q)){
        printf("Empty\n");
        return;
    }
    Node *current = q.head;
    while (current != NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayLG(Node *LG){
    printf("=== List LG (Winners) ===\n");
    if (LG == NULL){
        printf("Empty\n");
        return;
    }
    Node *current = LG;
    while (current != NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayLP(Node *LP){
    printf("=== List LP (Losers) ===\n");
    if (LP == NULL){
        printf("Empty\n");
        return;
    }
    Node *current = LP;
    while (current != NULL){
        displayPlayer(current->info);
        printf("\n");
        current = current->next;
    }
}

void displayStartTime(time_t t){
    struct tm *tm_info;
    char buffer[30];
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Round start: %s\n", buffer);
}

void displayEndTime(time_t t){
    struct tm *tm_info;
    char buffer[30];
    tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("Round end:   %s\n", buffer);
}

//ROUND RECORDING

void addRound(RoundNode **head, Round r){
    RoundNode *newNode = malloc(sizeof(RoundNode));
    if (!newNode) return;
    newNode->info = r;
    newNode->next = NULL;
    
    if (*head == NULL){
        *head = newNode;
    } else {
        RoundNode *cur = *head;
        while (cur->next != NULL){
            cur = cur->next;
        }
        cur->next = newNode;
    }
}

void displayAllRounds(RoundNode *head){
    printf("\n===== ALL RECORDED ROUNDS =====\n");
    if (head == NULL){
        printf("No rounds recorded.\n");
        return;
    }
    RoundNode *cur = head;
    while (cur != NULL){
        Round r = cur->info;
        printf("Round %d (Part %d): %s vs %s, scores %d - %d\n",
               r.roundNum, r.part,
               r.player1.name, r.player2.name,
               r.finalScore1, r.finalScore2);
        cur = cur->next;
    }
}

//INITIALIZATION

void gameStart(int *n){
    printf("\n===============================================\n");
    printf("        WELCOME TO THE GAME SYSTEM\n");
    printf("===============================================\n\n");
    printf("Enter the number of players (min 2): ");
    do {
        scanf("%d", n);
        if (*n <= 1) {
            printf("Please enter at least 2 players: ");
        }
    } while (*n <= 1);
}

int randomAge(int min, int max){
    return min + rand() % (max - min + 1);
}

void createTheQueue(Queue *F, int n){
    initQueue(F);
    for (int i = 0; i < n; i++){
        Player p;
        p.number = i + 1;
        p.age = randomAge(18, 50);
        sprintf(p.name, "Player%d", i + 1);
        
        p.totalLosses1 = 0;
        p.totalWins1 = 0;
        p.streakWins1 = 0;
        p.streakLosses1 = 0;
        
        p.totalWins2 = 0;
        p.totalLosses2 = 0;
        p.streakWins2 = 0;
        
        p.score = 0;
        p.totalScore = 0;
        p.priority = 0;
        enQueue(F, p);
    }
}

// LIST FUNCTIONS

void addToLp(Node **LP, Player p){
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) return;
    newNode->info = p;
    newNode->next = NULL;
    
    if (*LP == NULL){
        *LP = newNode;
    } else {
        Node *cur = *LP;
        while (cur->next != NULL){
            cur = cur->next;
        }
        cur->next = newNode;
    }
}

void insertInDescending(Node **LG, Player p){
    Node *newNode = malloc(sizeof(Node));
    if (newNode == NULL) return;
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

//PART 1 FUNCTIONS

int RandomValue(){
    return rand() % 100000;
}

int digitSum(int x){
    int sum = 0;
    if (x < 0) x = -x;
    while (x > 0){
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

bool handleSinglePlayerCase(Queue *F, Queue *F1, Queue *F3,int *nbF, int *nbF1, int *nbF3, Player *p1, Player *p2){
    int totalPlayers = *nbF + *nbF1 + *nbF3;
    
    if (totalPlayers < 2) return false;
    
    // Only 1 in F1, get second from F
    if (*nbF1 == 1 && *nbF > 0){
        deQueue(F1, p1);
        p1->priority = 1;
        (*nbF1)--;
        deQueue(F, p2);
        p2->priority = 0;
        (*nbF)--;
        return true;
    }
    
    // Only 1 in F, get second from F3
    if (*nbF == 1 && *nbF1 == 0 && *nbF3 > 0){
        deQueue(F, p1);
        p1->priority = 0;
        (*nbF)--;
        deQueue(F3, p2);
        p2->priority = 2;
        (*nbF3)--;
        return true;
    }
    
    return false;
}

bool getAPlayer(Queue *F, Queue *F1, Queue *F3,
                int *nbF, int *nbF1, int *nbF3, Player *p){
    if (!isEmptyQueue(*F1)){
        deQueue(F1, p);
        p->priority = 1;
        (*nbF1)--;
        return true;
    } else if (!isEmptyQueue(*F)){
        deQueue(F, p);
        p->priority = 0;
        (*nbF)--;
        return true;
    } else if (!isEmptyQueue(*F3)){
        deQueue(F3, p);
        p->priority = 2;
        (*nbF3)--;
        return true;
    }
    return false;
}

bool selectTwo(Queue *F, Queue *F1, Queue *F3,
               int *nbF, int *nbF1, int *nbF3,
               Player *p1, Player *p2){
    // Check special single-player cases first
    if (handleSinglePlayerCase(F, F1, F3, nbF, nbF1, nbF3, p1, p2)){
        return true;
    }
    
    // Normal case
    if (!getAPlayer(F, F1, F3, nbF, nbF1, nbF3, p1)) return false;
    if (!getAPlayer(F, F1, F3, nbF, nbF1, nbF3, p2)) return false;
    return true;
}

void part1Round(int roundNum, Player *p1, Player *p2){
    int randNum, sum;
    int currentPlayer = 1;
    int i = 1;
    
    printf("\n========== Round %d (Part 1) ==========\n", roundNum);
    printf("Player 1: %s (score: %d) \t VS \t Player 2: %s (score: %d)\n\n",
           p1->name, p1->score, p2->name, p2->score);
    
    while (i <= 12 && abs(p1->score - p2->score) < 3){
        randNum = RandomValue();
        sum = digitSum(randNum);
        
        if (currentPlayer == 1){
            if (sum % 5 == 0){
                p1->score++;
                printf("Turn %d - %s: random=%d, digitSum=%d -> SCORE! New score=%d\n",
                       i, p1->name, randNum, sum, p1->score);
            } else {
                printf("Turn %d - %s: random=%d, digitSum=%d -> No point (score=%d)\n",
                       i, p1->name, randNum, sum, p1->score);
            }
        } else {
            if (sum % 5 == 0){
                p2->score++;
                printf("Turn %d - %s: random=%d, digitSum=%d -> SCORE! New score=%d\n",
                       i, p2->name, randNum, sum, p2->score);
            } else {
                printf("Turn %d - %s: random=%d, digitSum=%d -> No point (score=%d)\n",
                       i, p2->name, randNum, sum, p2->score);
            }
        }
        i++;
        currentPlayer = 3 - currentPlayer;
    }
    
    printf("\n----- End of Round %d -----\n", roundNum);
    printf("%s final score: %d\n", p1->name, p1->score);
    printf("%s final score: %d\n", p2->name, p2->score);
}

void updateAfterPart1(Player *winner, Player *loser){
    // Winner update
    winner->totalWins1++;
    winner->streakLosses1 = 0;
    if (winner->priority != 1){
        winner->streakWins1++;
        if (winner->streakWins1 >= 3){
            winner->priority = 1;
        }
    }
    winner->totalScore += winner->score;
    winner->score = 0;
    
    // Loser update
    loser->totalLosses1++;
    loser->streakWins1 = 0;
    loser->streakLosses1++;
    loser->totalScore += loser->score;
    loser->score = 0;
}

void updateDrawPart1(Player *p1, Player *p2){
    p1->streakWins1 = 0;
    p2->streakWins1 = 0;
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

void placePlayerPart1(Player p, Queue *F, Queue *F1, Queue *F3,
                      int *nbF, int *nbF1, int *nbF3,
                      Node **LG, Node **LP){
    if (p.totalLosses1 >= 5){
        printf("  -> %s goes to LP (5 total losses)\n", p.name);
        addToLp(LP, p);
        return;
    }
    
    if (p.totalWins1 >= 5){
        printf("  -> %s goes to LG (5 total wins)\n", p.name);
        insertInDescending(LG, p);
        return;
    }
    
    if (p.totalLosses1 >= 3){
        p.priority = 2;
        printf("  -> %s goes to F3 (3+ losses)\n", p.name);
        enQueue(F3, p);
        (*nbF3)++;
    }
    else if (p.priority == 1){
        printf("  -> %s returns to F1\n", p.name);
        enQueue(F1, p);
        (*nbF1)++;
    }
    else {
        p.priority = 0;
        printf("  -> %s goes to F\n", p.name);
        enQueue(F, p);
        (*nbF)++;
    }
}

// PART 2 FUNCTIONS

int gcd(int a, int b){
    if (a < 0) a = -a;
    if (b < 0) b = -b;
    while (b != 0){
        int r = a % b;
        a = b;
        b = r;
    }
    return a;
}

bool containDigit(int x, int d){
    if (x < 0) x = -x;
    if (x == 0 && d == 0) return true;
    while (x > 0){
        if (x % 10 == d) return true;
        x /= 10;
    }
    return false;
}

bool hasDigit(int g, int a, int b){
    if (g < 0) g = -g;
    if (g == 0){
        return containDigit(a, 0) || containDigit(b, 0);
    }
    
    while (g > 0){
        int d = g % 10;
        if (containDigit(a, d) || containDigit(b, d)){
            return true;
        }
        g /= 10;
    }
    return false;
}

void part2Round(int roundNum, Player *p1, Player *p2){
    int a, b, g;
    int currentPlayer = 1;
    int i = 1;
    
    printf("\n========== Round %d (Part 2 - NEW STRATEGY) ==========\n", roundNum);
    printf("Player 1: %s (score: %d) \t VS \t Player 2: %s (score: %d)\n\n",
           p1->name, p1->score, p2->name, p2->score);
    
    while (i <= 16 && abs(p1->score - p2->score) < 3){
        a = RandomValue();
        b = RandomValue();
        g = gcd(a, b);
        
        if (currentPlayer == 1){
            if (hasDigit(g, a, b)){
                p1->score++;
                printf("Turn %d - %s: x=%d, y=%d, gcd=%d -> SCORE! New score=%d\n",
                       i, p1->name, a, b, g, p1->score);
            } else {
                printf("Turn %d - %s: x=%d, y=%d, gcd=%d -> No point (score=%d)\n",
                       i, p1->name, a, b, g, p1->score);
            }
        } else {
            if (hasDigit(g, a, b)){
                p2->score++;
                printf("Turn %d - %s: x=%d, y=%d, gcd=%d -> SCORE! New score=%d\n",
                       i, p2->name, a, b, g, p2->score);
            } else {
                printf("Turn %d - %s: x=%d, y=%d, gcd=%d -> No point (score=%d)\n",
                       i, p2->name, a, b, g, p2->score);
            }
        }
        i++;
        currentPlayer = 3 - currentPlayer;
    }
    
    printf("\n----- End of Round %d (Part 2) -----\n", roundNum);
    printf("%s final score: %d\n", p1->name, p1->score);
    printf("%s final score: %d\n", p2->name, p2->score);
}

void updateAfterPart2(Player *winner, Player *loser){
    winner->totalWins2++;
    winner->streakWins2++;
    winner->totalScore += winner->score;
    winner->score = 0;
    
    loser->totalLosses2++;
    loser->streakWins2 = 0;
    loser->totalScore += loser->score;
    loser->score = 0;
}

void updateDrawPart2(Player *p1, Player *p2){
    p1->streakWins2 = 0;
    p2->streakWins2 = 0;
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

void placePlayerPart2(Player p, Queue *F, Queue *F1, Queue *F3,
                      int *nbF, int *nbF1, int *nbF3,
                      Node **LG, Node **LP){
    if (p.totalLosses2 >= 2){
        printf("  -> %s goes to LP (2 losses in Part 2)\n", p.name);
        addToLp(LP, p);
        return;
    }
    if (p.streakWins2 >= 2){
        printf("  -> %s goes to LG (2 consecutive wins in Part 2)\n", p.name);
        insertInDescending(LG, p);
        return;
    }
    
    if (p.priority == 2){
        enQueue(F3, p);
        (*nbF3)++;
    }
    else if (p.priority == 1){
        enQueue(F1, p);
        (*nbF1)++;
    }
    else {
        enQueue(F, p);
        (*nbF)++;
    }
}

void updateMaxCasePart2(Player *p1, Player *p2){
    p1->totalScore += p1->score;
    p2->totalScore += p2->score;
    p1->score = 0;
    p2->score = 0;
}

// Display FUNCTIONS 

void displayTop3(Node *LG){
    printf("\n=== Top 3 Winners (Global) ===\n");
    if (LG == NULL){
        printf("No winners.\n");
        return;
    }
    int k = 0;
    Node *cur = LG;
    while (cur != NULL && k < 3){
        Player p = cur->info;
        printf("#%d: %s (num=%d, totalScore=%d, W1=%d, W2=%d)\n",
               k + 1, p.name, p.number, p.totalScore, p.totalWins1, p.totalWins2);
        cur = cur->next;
        k++;
    }
}

void showNoWinPart1(Node *LG, Node *LP){
    printf("\n=== Players with 0 wins in Part I ===\n");
    bool found = false;
    
    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins1 == 0){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    
    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins1 == 0){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    if (!found) printf("None\n");
}

void showWinsPart1(Node *LG, Node *LP, int k){
    printf("\n=== Players with %d win(s) in Part I ===\n", k);
    bool found = false;
    
    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins1 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    
    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins1 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    if (!found) printf("None\n");
}

void showLossesPart1(Node *LG, Node *LP, int k){
    printf("\n=== Players with %d loss(es) in Part I ===\n", k);
    bool found = false;
    
    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalLosses1 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    
    cur = LP;
    while (cur != NULL){
        if (cur->info.totalLosses1 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    if (!found) printf("None\n");
}

void showWinsPart2(Node *LG, Node *LP, int k){
    printf("\n=== Players with %d win(s) in Part II ===\n", k);
    bool found = false;
    
    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalWins2 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    
    cur = LP;
    while (cur != NULL){
        if (cur->info.totalWins2 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    if (!found) printf("None\n");
}

void showLossesPart2(Node *LG, Node *LP, int k){
    printf("\n=== Players with %d loss(es) in Part II ===\n", k);
    bool found = false;
    
    Node *cur = LG;
    while (cur != NULL){
        if (cur->info.totalLosses2 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    
    cur = LP;
    while (cur != NULL){
        if (cur->info.totalLosses2 == k){
            displayPlayer(cur->info);
            printf("\n");
            found = true;
        }
        cur = cur->next;
    }
    if (!found) printf("None\n");
}


int main(){
    srand(time(NULL));
    int n, globalRoundNum = 1;
    int nbF, nbF1, nbF3;
    bool hasWinner = false;
    Player p1, p2, winner, loser;
    time_t startTime, endTime;
    
    RoundNode *roundsHead = NULL;
    
    Queue F, F1, F3;
    initQueue(&F);
    initQueue(&F1);
    initQueue(&F3);
    
    Node *LG = NULL;
    Node *LP = NULL;
    
    gameStart(&n);
    
    createTheQueue(&F, n);
    nbF = n;
    nbF1 = 0;
    nbF3 = 0;
    
    printf("\n===== STARTING PART I =====\n");
    printf("Strategy: Sum of digits divisible by 5 = 1 point\n\n");
    
    // PART 1
    while (globalRoundNum <= 3 * n && (!isEmptyQueue(F) || !isEmptyQueue(F1) || !isEmptyQueue(F3))){
        
        startTime = time(NULL);
        
        if (!hasWinner){
            if (!selectTwo(&F, &F1, &F3, &nbF, &nbF1, &nbF3, &p1, &p2)){
                break;
            }
        } else {
            p1 = winner;
            if (!getAPlayer(&F, &F1, &F3, &nbF, &nbF1, &nbF3, &p2)){
                placePlayerPart1(p1, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                hasWinner = false;
                break;
            }
        }
        
        part1Round(globalRoundNum, &p1, &p2);
        
        endTime = time(NULL);
        displayStartTime(startTime);
        displayEndTime(endTime);
        
        // Save round
        Round r;
        r.roundNum = globalRoundNum;
        r.player1 = p1;
        r.player2 = p2;
        r.part = 1;
        r.finalScore1 = p1.score;
        r.finalScore2 = p2.score;
        r.startTime = startTime;
        r.endTime = endTime;
        addRound(&roundsHead, r);
        
        if (p1.score > p2.score){
            winner = p1;
            loser = p2;
            printf("\n** WINNER: %s with score %d **\n", winner.name, winner.score);
            updateAfterPart1(&winner, &loser);
            placePlayerPart1(loser, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
            
            if (winner.totalWins1 >= 5){
                placePlayerPart1(winner, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                hasWinner = false;
            } else {
                hasWinner = true;
            }
        }
        else if (p2.score > p1.score){
            winner = p2;
            loser = p1;
            printf("\n** WINNER: %s with score %d **\n", winner.name, winner.score);
            updateAfterPart1(&winner, &loser);
            placePlayerPart1(loser, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
            
            if (winner.totalWins1 >= 5){
                placePlayerPart1(winner, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                hasWinner = false;
            } else {
                hasWinner = true;
            }
        }
        else {
            printf("\n** DRAW: Both players got %d points **\n", p1.score);
            updateDrawPart1(&p1, &p2);
            p1.priority = 0;
            p2.priority = 0;
            enQueue(&F, p1);
            enQueue(&F, p2);
            nbF += 2;
            hasWinner = false;
        }
        
        printf("\n===== State after round %d =====\n", globalRoundNum);
        displayQueueF(F);
        displayQueueF1(F1);
        displayQueueF3(F3);
        displayLG(LG);
        displayLP(LP);
        printf("===================================\n");
        
        globalRoundNum++;
    }
    
    if (hasWinner){
        placePlayerPart1(winner, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
        hasWinner = false;
    }
    
    int remain = allQueueCount(F, F1, F3);
    
    // Handle single remaining player
    if (remain == 1){
        Player lastPlayer;
        if (!isEmptyQueue(F3)){
            deQueue(&F3, &lastPlayer);
            nbF3--;
            printf("\n** Last player %s in F3 declared LOSER **\n", lastPlayer.name);
            addToLp(&LP, lastPlayer);
        }
        else if (!isEmptyQueue(F1)){
            deQueue(&F1, &lastPlayer);
            nbF1--;
            insertInDescending(&LG, lastPlayer);
        }
        else if (!isEmptyQueue(F)){
            deQueue(&F, &lastPlayer);
            nbF--;
            addToLp(&LP, lastPlayer);
        }
        remain = 0;
    }
    
    // PART 2 
    if (remain > 1){
        printf("\n\n===============================================\n");
        printf("    CHANGING TO PART II STRATEGY\n");
        printf("    New Rule: GCD digit matching\n");
        printf("===============================================\n\n");
        
        int part2RoundNum = 1;
        while (part2RoundNum <= 2 * n && (!isEmptyQueue(F) || !isEmptyQueue(F1) || !isEmptyQueue(F3))){
            
            startTime = time(NULL);
            
            if (!selectTwo(&F, &F1, &F3, &nbF, &nbF1, &nbF3, &p1, &p2)){
                break;
            }
            
            part2Round(globalRoundNum, &p1, &p2);
            
            endTime = time(NULL);
            displayStartTime(startTime);
            displayEndTime(endTime);
            
            // Save round
            Round r2;
            r2.roundNum = globalRoundNum;
            r2.player1 = p1;
            r2.player2 = p2;
            r2.part = 2;
            r2.finalScore1 = p1.score;
            r2.finalScore2 = p2.score;
            r2.startTime = startTime;
            r2.endTime = endTime;
            addRound(&roundsHead, r2);
            
            if (abs(p1.score - p2.score) >= 3){
                if (p1.score > p2.score){
                    winner = p1;
                    loser = p2;
                } else {
                    winner = p2;
                    loser = p1;
                }
                printf("\n** WINNER (Part 2): %s with score %d **\n", winner.name, winner.score);
                updateAfterPart2(&winner, &loser);
                winner.priority = 1;
                placePlayerPart2(winner, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                loser.priority = 2;
                placePlayerPart2(loser, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
            }
            else {

                if (p1.score > p2.score){
                    updateMaxCasePart2(&p1, &p2);
                    p1.priority = 0;
                    p2.priority = 2;
                    printf("\n** Higher score: %s goes to F, %s goes to F3 **\n", p1.name, p2.name);
                    placePlayerPart2(p1, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                    placePlayerPart2(p2, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                }
                else if (p2.score > p1.score){
                    updateMaxCasePart2(&p1, &p2);
                    p2.priority = 0;
                    p1.priority = 2;
                    printf("\n** Higher score: %s goes to F, %s goes to F3 **\n", p2.name, p1.name);
                    placePlayerPart2(p2, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                    placePlayerPart2(p1, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                }
                else {
                    printf("\n** DRAW (Part 2): Both players got %d points **\n", p1.score);
                    updateDrawPart2(&p1, &p2);
                    p1.priority = 0;
                    p2.priority = 0;
                    placePlayerPart2(p1, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                    placePlayerPart2(p2, &F, &F1, &F3, &nbF, &nbF1, &nbF3, &LG, &LP);
                }
            }
            
            printf("\n===== State after round %d (Part 2) =====\n", globalRoundNum);
            displayQueueF(F);
            displayQueueF1(F1);
            displayQueueF3(F3);
            displayLG(LG);
            displayLP(LP);
            printf("===================================\n");
            
            part2RoundNum++;
            globalRoundNum++;
        }
        

        if (part2RoundNum > 2 * n && (!isEmptyQueue(F) || !isEmptyQueue(F1) || !isEmptyQueue(F3))){
            printf("\n** 2n rounds in Part 2 completed. Moving remaining players... **\n");
            Player p;
            while (!isEmptyQueue(F1)){
                deQueue(&F1, &p);
                printf("  %s from F1 -> LG\n", p.name);
                insertInDescending(&LG, p);
            }
            while (!isEmptyQueue(F)){
                deQueue(&F, &p);
                printf("  %s from F -> LP\n", p.name);
                addToLp(&LP, p);
            }
            while (!isEmptyQueue(F3)){
                deQueue(&F3, &p);
                printf("  %s from F3 -> LP\n", p.name);
                addToLp(&LP, p);
            }
        }
    }
    
    printf("\n\n");
    printf("===============================================\n");
    printf("           GAME OVER - FINAL RESULTS\n");
    printf("===============================================\n");
    
    displayAllRounds(roundsHead);
    
    printf("\n===== FINAL STATISTICS =====\n");
    displayTop3(LG);
    
    showNoWinPart1(LG, LP);
    
    for (int i = 1; i <= 3; i++){
        showWinsPart1(LG, LP, i);
        showLossesPart1(LG, LP, i);
    }
    
    for (int i = 1; i <= 2; i++){
        showWinsPart2(LG, LP, i);
        showLossesPart2(LG, LP, i);
    }
    
    printf("\n===============================================\n");
    printf("      Thank you for playing!\n");
    printf("===============================================\n");
    
    return 0;
}