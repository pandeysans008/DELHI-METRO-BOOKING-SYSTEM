#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <stdbool.h>
#include <limits.h>




//dictionary




#define dic_MAX 40

typedef struct dnode
{
    int key;
    char A[dic_MAX];
    struct dnode *next;
}dnode;

typedef dnode *DIC;

void initD(DIC *D);
void displayD(DIC D);
int appendD(DIC *D, char A[], int key);
void pushD(DIC *D, char A[], int key);
int search_for_key(DIC D, char A[]);
void callD(DIC *D);
char *search_for_station(DIC D, int key);




// graph




#define vertices 210

typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode* next;
} AdjListNode;

typedef struct AdjList
{
    AdjListNode* head;
} AdjList;

typedef struct Graph
{
    int V;
    AdjList* array;
} Graph;

typedef struct MinHeapNode
{
    int v;
    int dist;
} MinHeapNode;

typedef struct MinHeap
{
    int size;
    int capacity;
    int* pos;
    MinHeapNode** array;
} MinHeap;

typedef struct Result
{
    int ct;
    int *Rev_arr;
}Result;

void initResult(Result *R, int n);

AdjListNode* newAdjListNode(int dest, int weight);
Graph* createGraph(int V);
void addEdge(Graph* graph, int src, int dest, int weight);
MinHeap* createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b);
MinHeapNode* newMinHeapNode(int v, int dist);
void minHeapify(MinHeap* minHeap, int idx);
int isEmpty(MinHeap* minHeap);
MinHeapNode* extractMin(MinHeap* minHeap);
void decreaseKey(MinHeap* minHeap, int v, int dist);
bool isInMinHeap(MinHeap* minHeap, int v);
int dijkstra(Graph* graph, int src, int dest, Result *R);
void callG(char src[], char dest[]);



// tree



typedef struct node
{
    int data;
    struct node *fchild;
    struct node *next_sibling;
    struct node *prev_sibling;
} node;

typedef node *Tree;

typedef struct qnode
{
    struct qnode *next;
    node *data;
} qnode;

typedef struct queue
{
    qnode *front;
    qnode *rear;
} queue;



typedef struct Lnode{
    node *p;
    struct Lnode *next; 
} Lnode;

typedef Lnode *List;

void initList(List *L);
void appendL(List *L, node *p);
void pushL(List *L, node *p);

void initQ(queue *Q);
void enqueue(queue *Q, node *t);
int isEmptyQ(queue Q);
node *dequeue(queue *Q);

void initT(Tree *t);
void insert(Tree *t, int parent, int key);
node *search_line(Tree t, int key);
node *search_station(Tree t, int key);
void insert_platform(Tree *t, int Line, int station, int n, int A[]);
void traverse(Tree t);
node *search_station_2(Tree t, int station, List *L);
void callT(Result R, int src_value, int dest_value, int price, DIC D);




// hash



#define SIZE  100
// hash function for ticket
#define HASH_FUNCTION(ticket_no) (ticket_no % SIZE);


typedef struct ticket {
	struct ticket *next;
	int ticket_no;
	int fare_price;
	char *src;
	char *dest;
	int platform;
}ticket;

//pointer to ticket
typedef struct ticket*  ticket_ptr;

ticket_ptr searchticket(int ticketnum);
bool deleteticket(int ticketnum);
ticket_ptr createticket(int src_key, int dest_key, int platform_key, int fare, DIC D);
void init_ticketsystem();
void callH(int src_key, int dest_key, int fare, int platform, DIC D);