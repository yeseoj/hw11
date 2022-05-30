/**
 * ���ϸ�: graph-search.c
 * �й�: 2021041018
 * �̸�: �忹��
 * ����: undirected graph�� ���� ����Ʈ�� ǥ��
 * ������: 2022-05-28
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

/* ���� ����Ʈ head */
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


int initialize(headArray** h);                      /* �׷��� ���� */
int InsertVertex(headArray* h);                     /* vertex ���� */
int InsertEdge(headArray* h, int v1, int v2);       /* edge ���� */
int CheckEdge(headArray* h, int v1, int v2);        /* edge ���翩�� üũ */
void DepthFirstSearch(headArray* h, int v);         /* ���� �켱 Ž�� */
void BreathFirstSearch(headArray* h, int v);        /* �ʺ� �켱 Ž�� */
void resetVisited();                                /* visited �迭 �ʱ�ȭ */
void PrintGraph(headArray* h);                      /* �׷��� ��� */
void freeGraph(headArray* h);                       /* �Ҵ�� �޸� ���� */

int main() {
    int search_v, num1, num2;
    char command;
    headArray* head = NULL;

    printf("[-----[�忹��][2021041018]-----]\n");

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
            resetVisited(); /* ���� Ž���� ���� visited �迭 �ʱ�ȭ */
            break;
        case 'b': case 'B':
            printf("Enter Vertex to search for: ");
            scanf("%d", &search_v);
            BreathFirstSearch(head, search_v);
            resetVisited(); /* ���� Ž���� ���� visited �迭 �ʱ�ȭ */
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

/* graph ���� */
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

/* vertex ���� */
int InsertVertex(headArray* h) {
    /* 10�� ��� ���� �� */
    if (h->size == MAX_VERTICES) {
    printf("Vertex�� �ִ� ������ �����Ͽ����ϴ�.\n");
    return -1;
    }
    
    /** ���� ��ȿ�� vertex�� ������ ����
     * vertex�� 0������ ������ */
    h->size++;

    return 0;
}

int CheckEdge(headArray* h, int v1, int v2) {
    listNode* ptr = h->adjLists[v1];
    while (ptr != NULL) {
        if (ptr->data == v2) {
            printf("�̹� edge�� �����մϴ�.\n");
            return 1;
        }
        ptr = ptr->link;
    }

    return 0;
}

/** edge ����
 * ���� ���� ������������ ���Եȴ�.
*/
int InsertEdge(headArray* h, int v1, int v2) {
    /* �׷����� ���ǵ��� ���� ��� */
    if (h == NULL) {
        printf("�׷����� ���� ������ �ֽʽÿ�.\n");
        return -1;
    }
    /* edge�� ������ vertex�� �����Ǿ����� Ȯ�� */
    if (v1 >= h->size || v2 >= h->size) {
        printf("�Է��� vertex�� ���� �������� �ʾҽ��ϴ�.\n");
        return -1;
    }
    /* self loop ���� */
    if (v1 == v2) {
        printf("���� �������� ������ �� �����ϴ�.\n");
        return -1;
    }
    /* �ߺ� ���� ���� */
    if (CheckEdge(h, v1, v2))
        return -1;

    /** �� vertex�� �̸����� ���� ��� ����
     * node_v1: v2�� �̸����� ����, vertex v1�� ����
     * node_v2: v1�� �̸����� ����, vertex v2�� ����
     */
    listNode* node_v1 = (listNode*)malloc(sizeof(listNode));
    node_v1->data = v2;
    node_v1->link = NULL;
    listNode* node_v2 = (listNode*)malloc(sizeof(listNode));
    node_v2->data = v1;
    node_v2->link = NULL;

    /* vertex�� ����� edge�� ������ �ٷ� ���� */
    if (h->adjLists[v1] == NULL)
        h->adjLists[v1] = node_v1;
    /* �׷��� ���� ��� ������������ ���� */
    else {
        listNode* p = h->adjLists[v1];
        listNode* trail = NULL;
        while (p != NULL) {
            if (p->data > v2) {
                /* ù ��° ��� �������� �˻� */
                if (p == h->adjLists[v1]) {
                    h->adjLists[v1] = node_v1;
                    node_v1->link = p;
                }
                /* �߰��� �����ϴ��� �˻� */
                else {
                    node_v1->link = p;
                    trail->link = node_v1;
                }
                break;
            }
            trail = p;
            p = p->link;
        }
        /* ���������� ���Ҵٸ� �������� ���� */
        if (p == NULL)
            trail->link = node_v1;
    }
    /* vertex v2�� ���ؼ��� ���� */
    if (h->adjLists[v2] == NULL)
        h->adjLists[v2] = node_v2;
    else {
        listNode* p = h->adjLists[v2];
        listNode* trail = NULL;
        while (p != NULL) {
            if (p->data > v1) {
                /* ù ��° ��� �������� �˻� */
                if (p == h->adjLists[v2]) {
                    h->adjLists[v2] = node_v2;
                    node_v2->link = p;
                }
                /* �߰��� �����ϴ��� �˻� */
                else {
                    node_v2->link = p;
                    trail->link = node_v2;
                }
                break;
            }
            trail = p;
            p = p->link;
        }
        /* ���������� ���Ҵٸ� �������� ���� */
        if (p == NULL)
            trail->link = node_v2;
    }

    return 0;
}

void resetVisited() {
    for (int i = 0; i < MAX_VERTICES; i++)
        visited[i] = FALSE;
}

/** ���� �켱 Ž��
 * recursive ��� */
void DepthFirstSearch(headArray* h, int v) {
    /* �׷����� ���ǵ��� ���� ��� */
    if (h == NULL) {
        printf("�׷����� ���� ������ �ֽʽÿ�.\n");
        return;
    }
    /* �׷����� �������� ���� vertex�� �Է��� ��� */
    if (v >= h->size) {
        printf("�׷����� �������� �����Ƿ� ���� ������ �� �� �����ϴ�.\n");
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
/** �ʺ� �켱 Ž��
 * ���ڰ� ���� ������ �湮
 * iterative ��� */
void BreathFirstSearch(headArray* h, int v) {
    /* �׷����� ���ǵ��� ���� ��� */
    if (h == NULL) {
        printf("�׷����� ���� ������ �ֽʽÿ�.\n");
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

/* graph ��� */
void PrintGraph(headArray* h) {
    /* �׷����� ���ǵ��� ���� ��� */
    if (h == NULL) {
        printf("�׷����� ���� ������ �ֽʽÿ�.\n");
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

/* graph�� ���� �Ҵ�� �޸� ���� */
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
    /* ������ ������ ��� */
    if (top == MAX_STACK_SIZE)
        return;

    stack[++top] = n;
}

int pop() {
    /* ������ ����� ��� */
    if (top == -1) {
        return -1;
    }

    return stack[top--];
}

/* for queue */
void enQueue(int n) {
    /* ť�� ������ ��� */
    if ((rear + 1) % MAX_QUEUE_SIZE == front)
        return;
    
    rear = (rear + 1) % MAX_QUEUE_SIZE;
    queue[rear] = n;
}

int deQueue() {
    /* ť�� ����� ��� */
    if (front == rear)
        return -1;

    front = (front + 1) % MAX_QUEUE_SIZE;
    return queue[front];
}