// RAILWAY RESERVATION SYSTEM
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRAINS 3
#define SEATS 3


struct Train {
    int no;
    char name[30];
    char route[30];
};

struct Train train[TRAINS] = {
    {101, "Rajdhani", "Delhi-Mumbai"},
    {102, "Shatabdi", "Delhi-Bhopal"},
    {103, "Duronto",  "Jammu-Delhi"}
};

int seat[TRAINS][SEATS];  
struct Passenger {
    int ticket;
    char name[20];
    int age;
    int t;          // train index
    int seatNo;     // 1,2,3...  (0 means waiting)
    struct Passenger *next;
};
struct Passenger *head = NULL;


struct Passenger *front = NULL;
struct Passenger *rear = NULL;


struct History {
    int ticket;
    char name[20];
    struct History *next;
};
struct History *top = NULL;

int ticketCounter = 1;


int getFreeSeat(int t) {
    int i;
    for (i = 0; i < SEATS; i++) {
        if (seat[t][i] == 0)
            return i + 1;
    }
    return 0;
}

// add passenger at end of linked list
void addToList(struct Passenger *p) {
    struct Passenger *temp;
    p->next = NULL;
    if (head == NULL) {
        head = p;
    } else {
        temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = p;
    }
}


void addToQueue(struct Passenger *p) {
    p->next = NULL;
    if (rear == NULL) {
        front = rear = p;
    } else {
        rear->next = p;
        rear = p;
    }
}

void push(int ticket, char name[]) {
    struct History *h = (struct History *)malloc(sizeof(struct History));
    h->ticket = ticket;
    strcpy(h->name, name);
    h->next = top;
    top = h;
}


void showTrains() {
    int i, j, free;
    printf("\nNo\tName\t\tRoute\t\tFree seats\n");
    for (i = 0; i < TRAINS; i++) {
        free = 0;
        for (j = 0; j < SEATS; j++)
            if (seat[i][j] == 0)
                free++;
        printf("%d\t%s\t%s\t%d\n", train[i].no, train[i].name, train[i].route, free);
    }
}


void searchTrain() {
    int no, i, found = 0;
    printf("Enter train number: ");
    scanf("%d", &no);
    for (i = 0; i < TRAINS; i++) {
        if (train[i].no == no) {
            printf("Found: %d %s %s\n", train[i].no, train[i].name, train[i].route);
            found = 1;
        }
    }
    if (found == 0)
        printf("Train not found\n");
}


void bookTicket() {
    int no, i, t = -1;
    struct Passenger *p;

    showTrains();
    printf("Enter train number: ");
    scanf("%d", &no);
    for (i = 0; i < TRAINS; i++)
        if (train[i].no == no)
            t = i;
    if (t == -1) {
        printf("Wrong train number\n");
        return;
    }

    p = (struct Passenger *)malloc(sizeof(struct Passenger));
    printf("Enter name: ");
    scanf("%s", p->name);
    printf("Enter age: ");
    scanf("%d", &p->age);
    p->ticket = ticketCounter++;
    p->t = t;
    p->seatNo = getFreeSeat(t);

    if (p->seatNo != 0) {
        seat[t][p->seatNo - 1] = p->ticket;
        addToList(p);
        printf("Ticket booked! Ticket no = %d, Seat no = %d\n", p->ticket, p->seatNo);
    } else {
        addToQueue(p);
        printf("Seats full. Added to waiting list. Ticket no = %d\n", p->ticket);
    }
}


void cancelTicket() {
    int tk, t, s;
    struct Passenger *cur = head, *prev = NULL;
    struct Passenger *w, *wprev = NULL;

    printf("Enter ticket number to cancel: ");
    scanf("%d", &tk);

    
    while (cur != NULL && cur->ticket != tk) {
        prev = cur;
        cur = cur->next;
    }
    if (cur == NULL) {
        printf("Ticket not found\n");
        return;
    }

    
    if (prev == NULL)
        head = cur->next;
    else
        prev->next = cur->next;

    t = cur->t;
    s = cur->seatNo;
    seat[t][s - 1] = 0;          // seat is free now
    push(cur->ticket, cur->name); // save in history stack
    printf("Ticket %d cancelled\n", cur->ticket);
    free(cur);

    
    w = front;
    while (w != NULL && w->t != t) {
        wprev = w;
        w = w->next;
    }
    if (w != NULL) {
        if (wprev == NULL)
            front = w->next;
        else
            wprev->next = w->next;
        if (w == rear)
            rear = wprev;

        w->seatNo = s;
        seat[t][s - 1] = w->ticket;
        addToList(w);
        printf("Waiting passenger %s got seat %d\n", w->name, s);
    }
}


void searchPassenger() {
    int tk, found = 0;
    struct Passenger *p;
    printf("Enter ticket number: ");
    scanf("%d", &tk);
    for (p = head; p != NULL; p = p->next) {
        if (p->ticket == tk) {
            printf("%d %s age %d train %d seat %d\n", p->ticket, p->name, p->age,
                   train[p->t].no, p->seatNo);
            found = 1;
        }
    }
    for (p = front; p != NULL; p = p->next) {
        if (p->ticket == tk) {
            printf("%d %s is in WAITING list\n", p->ticket, p->name);
            found = 1;
        }
    }
    if (found == 0)
        printf("Passenger not found\n");
}


void showBookings() {
    struct Passenger *arr[100], *temp;
    struct Passenger *p;
    int n = 0, i, j;

    for (p = head; p != NULL; p = p->next)
        arr[n++] = p;

    if (n == 0) {
        printf("No bookings\n");
        return;
    }

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (strcmp(arr[j]->name, arr[j + 1]->name) > 0) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }

    printf("\nTicket\tName\tAge\tTrain\tSeat\n");
    for (i = 0; i < n; i++)
        printf("%d\t%s\t%d\t%d\t%d\n", arr[i]->ticket, arr[i]->name, arr[i]->age,
               train[arr[i]->t].no, arr[i]->seatNo);
}


void showWaiting() {
    struct Passenger *p = front;
    if (p == NULL) {
        printf("Waiting list is empty\n");
        return;
    }
    while (p != NULL) {
        printf("%d %s (train %d)\n", p->ticket, p->name, train[p->t].no);
        p = p->next;
    }
}

void showHistory() {
    struct History *h = top;
    if (h == NULL) {
        printf("No cancelled tickets\n");
        return;
    }
    while (h != NULL) {
        printf("Ticket %d - %s\n", h->ticket, h->name);
        h = h->next;
    }
}


void saveFile() {
    FILE *fp = fopen("data.txt", "w");
    struct Passenger *p;
    if (fp == NULL) {
        printf("File error\n");
        return;
    }
    fprintf(fp, "%d\n", ticketCounter);
    for (p = head; p != NULL; p = p->next)
        fprintf(fp, "%d %s %d %d %d\n", p->ticket, p->name, p->age, p->t, p->seatNo);
    for (p = front; p != NULL; p = p->next)
        fprintf(fp, "%d %s %d %d %d\n", p->ticket, p->name, p->age, p->t, 0);
    fclose(fp);
    printf("Data saved\n");
}


void loadFile() {
    FILE *fp = fopen("data.txt", "r");
    struct Passenger *p;
    if (fp == NULL)
        return;
    fscanf(fp, "%d", &ticketCounter);
    while (1) {
        p = (struct Passenger *)malloc(sizeof(struct Passenger));
        if (fscanf(fp, "%d %s %d %d %d", &p->ticket, p->name, &p->age, &p->t, &p->seatNo) != 5) {
            free(p);
            break;
        }
        if (p->seatNo != 0) {
            seat[p->t][p->seatNo - 1] = p->ticket;
            addToList(p);
        } else {
            addToQueue(p);
        }
    }
    fclose(fp);
}

int main() {
    int choice;
    loadFile();
    do {
        printf("\n===== RAILWAY RESERVATION SYSTEM =====\n");
        printf("1. Show trains\n");
        printf("2. Search train\n");
        printf("3. Book ticket\n");
        printf("4. Cancel ticket\n");
        printf("5. Search passenger\n");
        printf("6. Show all bookings\n");
        printf("7. Show waiting list\n");
        printf("8. Cancel history\n");
        printf("9. Save and exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: showTrains(); break;
            case 2: searchTrain(); break;
            case 3: bookTicket(); break;
            case 4: cancelTicket(); break;
            case 5: searchPassenger(); break;
            case 6: showBookings(); break;
            case 7: showWaiting(); break;
            case 8: showHistory(); break;
            case 9: saveFile(); break;
            default: printf("Invalid choice\n");
        }
    } while (choice != 9);

    return 0;
}