#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <stdbool.h>
#include <limits.h>

#include "FINAL.h"





// dictionary 


void initD(DIC *D)
{
    *D=NULL;
    return;
}

int appendD(DIC *D, char A[], int key)
{
    dnode *nn = (dnode *)malloc(sizeof(dnode));
    if (!nn)
    {
        return 0;
    }
    nn->key = key;
    strcpy(nn->A,A);
    nn->next = NULL;
    if (!*D)
    {
        *D = nn;
        return 1;
    }
    dnode *p = *D;
    while (p->next)
    {
        if(strcasecmp(p->A,A)==0)
            return 0;
        p = p->next;
    }
    p->next = nn;
    return 1;
}

void pushD(DIC *D, char A[], int key)
{
    dnode *nn = (dnode *)malloc(sizeof(dnode));
    if (!nn)
    {
        return;
    }
    nn->key = key;
    nn->next=NULL;
    strcpy(nn->A,A);
    if (!*D)
    {
        *D = nn;
        return;
    }
    nn->next = *D;
    *D = nn;
}

int search_for_key(DIC D, char A[])
{
    dnode *p = D;
    int i=0;
    while(p)
    {
        int n = strcasecmp(p->A, A);
        if(n==0)
            return p->key;
        p=p->next;
        i++;
    }
    return -1;  //not found
}


char *search_for_station(DIC D, int key)
{
    dnode *p = D;
    while(p)
    {
        if(p->key == key)
            return p->A;
        p=p->next;
    }
    return NULL;  //not found
}


void displayD(DIC D)
{
    dnode *p;
    p = D;
    while (p)
    {
        printf("%d: %s\n", p->key,p->A);
        p = p->next;
    }
    return;
}


void callD(DIC *D)
{
    FILE *fptr;
    char A[dic_MAX];
    char *token;
    initD(D);
    fptr=fopen("stations1.txt","r");
    int n, i=0;
    while (fgets(A, sizeof(A), fptr) != NULL)
    {
        token = strtok(A, ",");
        int c = appendD(D,token,i);
        if(c==1)
            i++;
    }
    fclose(fptr);
}




// graph


void initResult(Result *R, int n)
{
    R->ct=0;
    R->Rev_arr=(int *)malloc(sizeof(int));
    return;
}

AdjListNode* newAdjListNode(int dest, int weight)
{
    AdjListNode* newNode = (AdjListNode*)malloc(sizeof(AdjListNode));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

Graph* createGraph(int V)
{
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->V = V;
    graph->array = (AdjList*)malloc(V * sizeof(AdjList));
    for (int i = 0; i < V; ++i)
        graph->array[i].head = NULL;

    return graph;
}

void addEdge(Graph* graph, int src, int dest, int weight)
{
    AdjListNode* newNode = newAdjListNode(dest, weight);
    newNode->next = graph->array[src].head;
    graph->array[src].head = newNode;
    newNode = newAdjListNode(src, weight);
    newNode->next = graph->array[dest].head;
    graph->array[dest].head = newNode;
}

MinHeap* createMinHeap(int capacity)
{
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->pos = (int*)malloc(capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    return minHeap;
}

void swapMinHeapNode(MinHeapNode** a, MinHeapNode** b)
{
    MinHeapNode* t = *a;
    *a = *b;
    *b = t;
}

MinHeapNode* newMinHeapNode(int v, int dist)
{
    MinHeapNode* minHeapNode = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

void minHeapify(MinHeap* minHeap, int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        MinHeapNode* smallestNode = minHeap->array[smallest];
        MinHeapNode* idxNode = minHeap->array[idx];

        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int isEmpty(MinHeap* minHeap)
{
    return minHeap->size == 0;
}

MinHeapNode* extractMin(MinHeap* minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    MinHeapNode* root = minHeap->array[0];

    MinHeapNode* lastNode = minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

void decreaseKey(MinHeap* minHeap, int v, int dist)
{
    int i = minHeap->pos[v];
    minHeap->array[i]->dist = dist;
    while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
    {
        minHeap->pos[minHeap->array[i]->v] = (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

bool isInMinHeap(MinHeap* minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}


int dijkstra(Graph* graph, int src, int dest, Result *R)
{
    int A[vertices];
    int price, time, i=0;
    int V = graph->V;
    int dist[V];
    int prev[V]; // Store the previous vertex in the shortest path
    MinHeap* minHeap = createMinHeap(V);

    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        prev[v] = -1; // Initialize previous vertex as -1
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    minHeap->array[src] = newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);
    minHeap->size = V;

    while (!isEmpty(minHeap))
    {
        MinHeapNode* minHeapNode = extractMin(minHeap);
        int u = minHeapNode->v;
        AdjListNode* pCrawl = graph->array[u].head;

        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            if (isInMinHeap(minHeap, v) && dist[u] != INT_MAX && pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;
                prev[v] = u; // Update the previous vertex
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // Print the shortest path
    // printf("Shortest path from vertex %d to vertex %d:\n", src, dest);
    if (dist[dest] == INT_MAX)
    {
        printf("No path exists.\n");
    }
    else
    {
        // printf("Path: ");
        int current = dest;

        while (current != src)
        {
            A[i]=current;
            current = prev[current];
            i++;
            R->ct++;
        }
        
        int k=0;
        R->Rev_arr[k++]=src;
        for(int j=R->ct-1; j>=0; j--)
        {
            R->Rev_arr[k]=A[j];
            k++;
        }
        R->ct++;

        // for(int q=0; q<R->ct; q++)
        // {
        //     printf("%d ", R->Rev_arr[q]);
        // }
        
        printf("\nDistance: %d\n", dist[dest]);
        price = dist[dest] * 0.5;
        printf("The cost for your journey will be: Rs.%d\n", price);
        time = dist[dest] * 0.2;
        printf("The estimated time of your journey is: %d mins\n", time);
    }
    return price;
}

void callG(char src[], char dest[])
{
    DIC D;
    Graph *G;
    Result R;
    initResult(&R, vertices);
    callD(&D);
    int st1, st2;
    int src_value = search_for_key(D,src);
    
    int dest_value = search_for_key(D,dest);

    int weight, price;
    G=createGraph(vertices);

    FILE *fptr_1 = fopen("edgesfinal.csv", "r");
    if (fptr_1 == NULL)
    {
        printf("Failed to open the file.\n");
        return;
    }
    while(fscanf(fptr_1, "%d,%d,%d",&st1, &st2,  &weight) != EOF)
    {
        addEdge(G, st1, st2, weight);
    }
    price=dijkstra(G, src_value, dest_value, &R);
    printf("Shortest Path:\n");
    for(int i=0; i < R.ct; i++)
    {
        printf("%s ",search_for_station(D, R.Rev_arr[i]));
    }
    callT(R, src_value, dest_value, price, D);
    return;
}



// tree



void initList(List *L)
{
    *L = NULL;
    return;
}

void appendL(List *L, node *p)
{
    Lnode *nn = (Lnode *)malloc(sizeof(Lnode));
    if (!nn)
    {
        return;
    }
    nn->p = p;
    nn->next = NULL;
    if (!*L)
    {
        *L = nn;
        return;
    }
    Lnode *m = *L;
    while (m->next)
    {
        m = m->next;
    }
    m->next = nn;
    return;
}

void pushL(List *L, node *p)
{
    Lnode *nn = (Lnode *)malloc(sizeof(Lnode));
    if (!nn)
    {
        return;
    }
    nn->p = p;
    nn->next=NULL;
    if (!*L)
    {
        *L = nn;
        return;
    }
    nn->next = *L;
    *L = nn; 
}


void initQ(queue *Q)
{
    Q->front = NULL;
    Q->rear = NULL;
    return;
}

void enqueue(queue *Q, node *t)
{
    qnode *nn = (qnode *)malloc(sizeof(qnode));
    if (!nn)
        return;
    nn->data = t;
    nn->next = NULL;
    if (!Q->front && !Q->rear)
    {
        Q->front = nn;
        Q->rear = nn;
        return;
    }
    (Q->rear)->next = nn;
    Q->rear = nn;
    return;
}

node *dequeue(queue *Q)
{
    if (!Q->front && !Q->rear)
    {
        return NULL;
    }
    node *e = (Q->front)->data;
    qnode *p = Q->front;
    if (p->next)
        Q->front = p->next;
    else
    {
        Q->front = NULL;
        Q->rear = NULL;
    }
    free(p);
    return e;
}

int isEmptyQ(queue Q)
{
    if (!(Q.front) && !(Q.rear))
        return 1;
    return 0;
}

void initT(Tree *t)
{
    *t = NULL;
    return;
}

node *search_line(Tree t, int key)
{
    if (!t)
        return NULL;
    node *p = t;
    p = p->fchild;
    while (p)
    {
        if (p->data == key)
            break;
        p = p->next_sibling;
    }
    return p;
}

void insert(Tree *t, int parent, int key)
{
    node *nn = (node *)malloc(sizeof(node));
    if (!nn)
        return;
    nn->data = key;
    nn->fchild = NULL;
    nn->next_sibling = NULL;
    nn->prev_sibling = NULL;
    if (!(*t))
    {
        *t = nn;
        return;
    }
    
    if (parent == -1) // inserting lines
    {

        node *p = *t;
        if (!p->fchild)
        {
            p->fchild = nn;
            return;
        }
        node *q = p->fchild;
        while (q->next_sibling)
            q = q->next_sibling;
        q->next_sibling = nn;
        nn->prev_sibling=q;
        return;
    }

    node *line = search_line(*t, parent);
    if (!line->fchild)
    {
        line->fchild = nn;
        return;
    }
    node *temp = line->fchild;
    while (temp->next_sibling)
    {
        temp = temp->next_sibling;
    }
    temp->next_sibling = nn;
    nn->prev_sibling = temp;
    return;
}

node *search_station(Tree t, int key) // platform
{
    if (!t)
        return NULL;
    queue Q;
    initQ(&Q);
    node *p = t;
    p = p->fchild;
    while (p) // enqueuing lines
    {
        enqueue(&Q, p);
        p = p->next_sibling;
    }
    while (!isEmptyQ(Q))
    {
        p = dequeue(&Q);
        node *q = p->fchild;
        while (q)
        {
            if (q->data == key)
                return q;
            q = q->next_sibling;
        }
    }
    return NULL;
}

node *search_station_2(Tree t, int station, List *L)
{
    initList(L);
    if (!t)
        return NULL;
    queue Q;
    initQ(&Q);
    node *p = t;
    p = p->fchild;
    while (p) // enqueuing lines
    {
        enqueue(&Q, p);
        p = p->next_sibling;
    }
    while (!isEmptyQ(Q))
    {
        p = dequeue(&Q);
        node *q = p->fchild;
        while (q)
        {
            if (q->data == station)
                appendL(L,q);
            q = q->next_sibling;
        }
    }
    Lnode *first=*L;
    Lnode *f=first;

    node *st=f->p;
    return st;
    // return NULL;
}

void insert_platform(Tree *t, int Line, int station, int n, int A[])
{
    node *p = search_line(*t, Line);
    p = p->fchild;
    while(p)
    {
        if(p->data == station)
            break;
        p=p->next_sibling;
    }
    // p is the station where platform is to be inserted
    int flag = 0;
    int j=0;
    for (int i = 1; i <= n; i++)
    {
        node *plat;
        node *nn = (node *)malloc(sizeof(node));
        node *nn2= (node *)malloc(sizeof(node));
        if (!nn || !nn2)
            return;
        nn->data = i+1000;
        nn2->data = A[j++];
        nn->fchild = NULL;
        nn->next_sibling = NULL;
        nn->prev_sibling = NULL;
        nn2->fchild = NULL;
        nn2->next_sibling = NULL;
        nn2->prev_sibling = NULL;
        if (!p->fchild && !flag)
        {
            p->fchild = nn;
            p = p->fchild;
            flag = 1;
        }
        else
        {
            p->next_sibling = nn;
            nn->prev_sibling = p;
            p = p->next_sibling;
        }
        if(nn2->data == -1)
        {
            p->fchild=NULL;
            free(nn2);
        }
        p->fchild=nn2;
    }
    return;
}

// int interchange_stations(Tree t, int B[])
// {
//     int count,flag=0;
//     int A[MAX];
//     int src = B[0];
//     List L;
//     search_station_2(t,src, &L);
//     int i=1,j=0;
//     Lnode *list_node=L;
//     node *p =list_node->p;
//     while(list_node)
//     {
//         while(i != count-1)
//         {

//             if((p->next_sibling)->data == B[i])
//             {
//                 flag=1;
//                 while(p && (p->next_sibling)->data == B[i])
//                 {
//                     i++;
//                     p=p->next_sibling;
//                 }
//             }
//             else if((p->prev_sibling)->data == B[i])
//             {
//                 flag=1;
//                 while(p && (p->prev_sibling)->data == B[i])
//                 {
//                     i++;
//                     p=p->prev_sibling;
//                 }
//             }
//             if(!flag)
//             {
//                 list_node=list_node->next;
//                 break;
//             }
//             A[j++]=B[i-1];
//         }
             
//     }
// }

int getplatforms(Tree t, int src, int src_next)
{
    List L;
    int platform;
    node *p=search_station_2(t, src, &L);
    p=p->fchild;
    while(p)
    {
        if((p->fchild)->data == src_next)
            break;
        p=p->next_sibling;
    }
    platform=p->data;
    return platform;
}

void traverse(Tree t) // preorder traversal
{

    if (!t)
        return;
    printf("%d ", t->data);
    node *child = t->fchild;
    while (child)
    {
        traverse(child);
        child = child->next_sibling;
    }
}

void callT(Result R, int src_value, int dest_value, int price, DIC D)
{
    int st, no_plat, src_platform;
    Tree T;
    initT(&T);
    insert(&T,-1, -1);
    for(int i = 500; i <=507; i++)
    {
        insert(&T, -1, i);
    }
    FILE *fptr = fopen("next_station_final.csv", "r");
    if (fptr == NULL) 
    {
        printf("Failed to open the file.\n");
        return;
    }
    //i is just for count. station and no. of platforms are scanned from csv file
    for(int i=0; i<=28; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d",&st,&no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 500, st);
        insert_platform(&T,500, st, no_plat, A);
    }


    for(int i=1; i<=37; i++)
    {
        int A[6];
        fscanf(fptr,"%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 501, st);
        insert_platform(&T,501, st, no_plat, A);
    }
    for(int i=1; i<=50; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 502, st);
        insert_platform(&T,502, st, no_plat, A);
    }
    for(int i=1; i<=8; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 503, st);
        insert_platform(&T,503, st, no_plat, A);
    }
    for(int i=1; i<=22; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 504, st);
        insert_platform(&T,504, st, no_plat, A);
    }
    for(int i=1; i<=34; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 505, st);
        insert_platform(&T,505, st, no_plat, A);
    }
    for(int i=1; i<=38; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        insert(&T, 506, st);
        insert_platform(&T,506, st, no_plat, A);
    }
    for(int i=1; i<=2; i++)
    {
        int A[6];
        fscanf(fptr, "%d,%d,%d,%d,%d,%d,%d,%d", &st, &no_plat, &A[0],&A[1],&A[2],&A[3],&A[4],&A[5]);
        
        insert(&T, 507, st);
        insert_platform(&T,507, st, no_plat, A);
    }
    int src_next_value=R.Rev_arr[1];

    src_platform = getplatforms(T,src_value,src_next_value);
    

    callH(src_value,dest_value,price,src_platform, D);
    return;

}





// hash



// hash table
ticket_ptr hash_table[SIZE];

// global ticketno.
int global_ticket_no = 1110;

// add ticket to hashlist
ticket_ptr createticket(int src_key, int dest_key, int platform_key, int fare, DIC D)
{
	int hash_value;
	ticket_ptr tic, cur_elem, prev_elem;

	tic = (ticket_ptr)malloc(sizeof(ticket));
	tic->ticket_no = global_ticket_no;
	tic->next = NULL;
	tic->fare_price = fare;
	tic->src = search_for_station(D, src_key);
	tic->dest = search_for_station(D, dest_key);
	tic->platform = platform_key - 1000;
	global_ticket_no++;

	hash_value = HASH_FUNCTION(tic->ticket_no);

	if (hash_table[hash_value] == NULL)
	{
		// first element
		hash_table[hash_value] = tic;
	}
	else
	{
		cur_elem = hash_table[hash_value];
		tic->next = cur_elem;
		hash_table[hash_value] = tic;

	}
	return (tic);
}

bool deleteticket(int ticketnum)
{
	ticket_ptr cur_elem, prev_elem;
	int hash_value;
	bool ticket_deleted = false;

	hash_value = HASH_FUNCTION(ticketnum);
	cur_elem = hash_table[hash_value];

	if (cur_elem != NULL)
	{
		if (cur_elem->ticket_no == ticketnum)
		{
			// if its first ticket
			hash_table[hash_value] = cur_elem->next;
			free(cur_elem);
			ticket_deleted = true;
		}
		else
		{
			prev_elem = cur_elem;
			cur_elem = cur_elem->next;
			while (cur_elem != NULL)
			{
				if (cur_elem->ticket_no == ticketnum)
				{
					// delete the ticket
					prev_elem->next = cur_elem->next;
					free(cur_elem);
					ticket_deleted = true;
					break;
				}
				prev_elem = cur_elem;
				cur_elem = cur_elem->next;
			}
		}
	}
	return (ticket_deleted);
}

ticket_ptr searchticket(int ticketnum)
{
	ticket_ptr cur_elem;
	int hash_value;

	hash_value = HASH_FUNCTION(ticketnum);

	cur_elem = hash_table[hash_value];
	while (cur_elem != NULL)
	{
		if (cur_elem->ticket_no == ticketnum)
		{
			break;
		}
		cur_elem = cur_elem->next;
	}

	return (cur_elem);
}

void init_ticketsystem(ticket_ptr hash_table[])
{
	// initialse the hash table to NULL values
	memset(hash_table, 0, sizeof(ticket_ptr) * SIZE);
}


void callH(int src_key, int dest_key, int fare, int platform, DIC D)
{
	ticket_ptr hash_table[SIZE];
	init_ticketsystem(hash_table);
	ticket_ptr q= createticket(src_key, dest_key, platform, fare, D);
	printf("\nTicket booked successfully!\n");
    printf("\nYour Ticket number: %d",q->ticket_no);
	return;
}
