/**
 * 파일명: graph-search.c
 * 학번: 2021041018
 * 이름: 장예서
 * 개요: undirected graph를 인접 리스트로 표현
 * 수정일: 2022-05-28
 */

#include<stdio.h>
#include<stdlib.h>
#define FALSE 0
#define TRUE 1
#define MAX_VERTICES 10
#define MAX_QUEUE_SIZE 10

/* vertex */
typedef struct vertex {
    int data;
    struct vertex* link;
} listNode;

/* 인접 리스트 head */
typedef struct head {
    int size;
    struct vertex* adjLists[MAX_VERTICES];
} headArray;

short int visited[MAX_VERTICES];

/* for queue */
int queue[MAX_QUEUE_SIZE];
int front = -1;
int rear = -1;

void enQueue(int n);
int deQueue();


int initialize(headArray** h);                      /* 그래프 정의 */
int InsertVertex(headArray* h);                     /* vertex 삽입 */
int InsertEdge(headArray* h, int v1, int v2);       /* edge 삽입 */
int CheckEdge(headArray* h, int v1, int v2);        /* edge 존재여부 체크 */
void DepthFirstSearch(headArray* h, int v);         /* 깊이 우선 탐색 */
void BreathFirstSearch(headArray* h, int v);        /* 너비 우선 탐색 */
void resetVisited();                                /* visited 배열 초기화 */
void PrintGraph(headArray* h);                      /* 그래프 출력 */
void freeGraph(headArray* h);                       /* 할당된 메모리 해제 */

int main() {
    int search_v, num1, num2;
    char command;
    headArray* head = NULL;

    printf("[-----[장예서][2021041018]-----]\n");

    do {
        printf("\n----------------------------------------------------------------\n");
        printf("                    Graph Searches\n");
        printf("----------------------------------------------------------------\n");
        printf("Initialize Graph        = z\n");
        printf("Insert Vertex           = v     Insert Edge             = e\n");
        printf("Depth First Search      = d     Breath First Serach     = b\n");
        printf("Print Graph             = p     Quit                    = q\n");
        printf("----------------------------------------------------------------\n");
        printf("command = ");
        scanf(" %c", &command);

        switch(command) {
        case 'z': case 'Z':
            initialize(&head);
            break;
        case 'v': case 'V':
            InsertVertex(head);
            break;
        case 'e': case 'E':
            printf("Enter two vertices to connect: ");
            scanf("%d %d", &num1, &num2);
            InsertEdge(head, num1, num2);
            break;
        case 'd': case 'D':
            printf("Enter Vertex to search for: ");
            scanf("%d", &search_v);
            DepthFirstSearch(head, search_v);
            resetVisited(); /* 다음 탐색을 위해 visited 배열 초기화 */
            break;
        case 'b': case 'B':
            printf("Enter Vertex to search for: ");
            scanf("%d", &search_v);
            BreathFirstSearch(head, search_v);
            resetVisited(); /* 다음 탐색을 위해 visited 배열 초기화 */
            break;
        case 'p': case 'P':
            PrintGraph(head);
            break;
        case 'q': case 'Q':
            freeGraph(head);
            break;
        default:
            printf("Wrong command.\n");
            break;
        }
    } while(command != 'Q' && command != 'q');
    return 0;
}

/* graph 정의 */
int initialize(headArray** h) {
    if (*h != NULL) {
        freeGraph(*h);
    }
    *h = (headArray*)malloc(sizeof(headArray));
    for (int i = 0; i < MAX_VERTICES; i++) {
        (*h)->adjLists[i] = NULL;
        visited[i] = FALSE;
    }
    (*h)->size = 0;

    return 0;
}

/* vertex 삽입 */
int InsertVertex(headArray* h) {
    /* 10개 모두 삽입 시 */
    if (h->size == MAX_VERTICES) {
    printf("Vertex의 최대 개수에 도달하였습니다.\n");
    return -1;
    }
    
    /** 현재 유효한 vertex의 개수를 증가
     * vertex는 0번부터 순차적 */
    h->size++;

    return 0;
}

int CheckEdge(headArray* h, int v1, int v2) {
    listNode* ptr = h->adjLists[v1];
    while (ptr != NULL) {
        if (ptr->data == v2) {
            printf("이미 edge가 존재합니다.\n");
            return 1;
        }
        ptr = ptr->link;
    }

    return 0;
}

/** edge 삽입
 * 인접 노드는 오름차순으로 삽입된다.
*/
int InsertEdge(headArray* h, int v1, int v2) {
    /* 그래프가 정의되지 않은 경우 */
    if (h == NULL) {
        printf("그래프를 먼저 생성해 주십시오.\n");
        return -1;
    }
    /* edge를 연결할 vertex가 생성되었는지 확인 */
    if (v1 >= h->size || v2 >= h->size) {
        printf("입력한 vertex가 현재 생성되지 않았습니다.\n");
        return -1;
    }
    /* self loop 배제 */
    if (v1 == v2) {
        printf("같은 정점끼리 연결할 수 없습니다.\n");
        return -1;
    }
    /* 중복 연결 배제 */
    if (CheckEdge(h, v1, v2))
        return -1;

    /** 각 vertex의 이름값을 가진 노드 생성
     * node_v1: v2의 이름값을 가짐, vertex v1에 연결
     * node_v2: v1의 이름값을 가짐, vertex v2에 연결
     */
    listNode* node_v1 = (listNode*)malloc(sizeof(listNode));
    node_v1->data = v2;
    node_v1->link = NULL;
    listNode* node_v2 = (listNode*)malloc(sizeof(listNode));
    node_v2->data = v1;
    node_v2->link = NULL;

    /* vertex에 연결된 edge가 없으면 바로 연결 */
    if (h->adjLists[v1] == NULL)
        h->adjLists[v1] = node_v1;
    /* 그렇지 않은 경우 오름차순으로 삽입 */
    else {
        listNode* p = h->adjLists[v1];
        listNode* trail = NULL;
        while (p != NULL) {
            if (p->data > v2) {
                /* 첫 번째 노드 앞쪽인지 검사 */
                if (p == h->adjLists[v1]) {
                    h->adjLists[v1] = node_v1;
                    node_v1->link = p;
                }
                /* 중간에 삽입하는지 검사 */
                else {
                    node_v1->link = p;
                    trail->link = node_v1;
                }
                break;
            }
            trail = p;
            p = p->link;
        }
        /* 마지막까지 돌았다면 마지막에 삽입 */
        if (p == NULL)
            trail->link = node_v1;
    }
    /* vertex v2에 대해서도 동일 */
    if (h->adjLists[v2] == NULL)
        h->adjLists[v2] = node_v2;
    else {
        listNode* p = h->adjLists[v2];
        listNode* trail = NULL;
        while (p != NULL) {
            if (p->data > v1) {
                /* 첫 번째 노드 앞쪽인지 검사 */
                if (p == h->adjLists[v2]) {
                    h->adjLists[v2] = node_v2;
                    node_v2->link = p;
                }
                /* 중간에 삽입하는지 검사 */
                else {
                    node_v2->link = p;
                    trail->link = node_v2;
                }
                break;
            }
            trail = p;
            p = p->link;
        }
        /* 마지막까지 돌았다면 마지막에 삽입 */
        if (p == NULL)
            trail->link = node_v2;
    }

    return 0;
}

void resetVisited() {
    for (int i = 0; i < MAX_VERTICES; i++)
        visited[i] = FALSE;
}

/** 깊이 우선 탐색
 * recursive 방식 */
void DepthFirstSearch(headArray* h, int v) {
    /* 그래프가 정의되지 않은 경우 */
    if (h == NULL) {
        printf("그래프를 먼저 생성해 주십시오.\n");
        return;
    }
    /* 그래프에 생성되지 않은 vertex를 입력한 경우 */
    if (v >= h->size) {
        printf("그래프에 존재하지 않으므로 시작 정점이 될 수 없습니다.\n");
        return;
    }

    listNode* w;
    visited[v] = TRUE;
    printf("%5d", v);
    for (w = h->adjLists[v]; w; w = w->link) { 
        if (!visited[w->data])
            DepthFirstSearch(h, w->data);
    }
}
/** 너비 우선 탐색
 * 숫자가 작은 순으로 방문
 * iterative 방식 */
void BreathFirstSearch(headArray* h, int v) {
    /* 그래프가 정의되지 않은 경우 */
    if (h == NULL) {
        printf("그래프를 먼저 생성해 주십시오.\n");
        return;
    }

    listNode* w = NULL;
    printf("%5d", v);
    visited[v] = TRUE;
    enQueue(v);
    while (1) {
        v = deQueue();
        if (v == -1)
            break;
        for (w = h->adjLists[v]; w; w = w->link) {
            if (!visited[w->data]) {
                printf("%5d", w->data);
                enQueue(w->data);
                visited[w->data] = TRUE;
            }
        }
    }
}

/* graph 출력 */
void PrintGraph(headArray* h) {
    /* 그래프가 정의되지 않은 경우 */
    if (h == NULL) {
        printf("그래프를 먼저 생성해 주십시오.\n");
        return;
    }

    listNode* p = NULL;
    printf("Print Graph.\n");
    for (int i = 0; i < h->size; i++) {
        printf("vertex %d is connected to [ ", i);
        p = h->adjLists[i];
        while (p != NULL) {
            printf("%d ", p->data);
            p = p->link;
        }
        printf("]\n");
    }
}

/* graph에 동적 할당된 메모리 해제 */
void freeGraph(headArray* h) {
    listNode* p = NULL;
    listNode* prev = NULL;

    for (int i = 0; i < h->size; i++) {
        p = h->adjLists[i];
        while (p != NULL) {
            prev = p;
            p = p->link;
            free(prev);
        }
    }
    free(h);
}

/* for stack */
void push(int n) {
    /* 스택이 가득찬 경우 */
    if (top == MAX_STACK_SIZE)
        return;

    stack[++top] = n;
}

int pop() {
    /* 스택이 비었을 경우 */
    if (top == -1) {
        return -1;
    }

    return stack[top--];
}

/* for queue */
void enQueue(int n) {
    /* 큐가 가득찬 경우 */
    if ((rear + 1) % MAX_QUEUE_SIZE == front)
        return;
    
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    queue[rear] = n;
}

int deQueue() {
    /* 큐가 비었을 경우 */
    if (front == rear)
        return -1;

    front = (front + 1) % MAX_QUEUE_SIZE;
    return queue[front];
}