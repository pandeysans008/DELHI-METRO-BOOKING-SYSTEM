#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>
#include <stdbool.h>
#include <limits.h>

#include "FINAL.h"

#define MAX 50

int main()
{
    char src[MAX],dest[MAX];
    int ch,tic_num;
    int i=0;

    printf("Welcome to Delhi Metro!\n");
    printf("Enter Metro start station: ");
    gets(src);
    printf("Enter Metro destination station: ");
    gets(dest);
    callG(src, dest);

    printf("\n1 - To book a new Ticket\n");
    printf("2 - To view your Ticket details\n");
    printf("3 - Cancel Reservation\n");
    printf("4 - Exit");
    printf("\nEnter choice:");
    scanf("%d",&ch);
    getchar();


    while( ch !=4)
    {
        switch (ch)
        {
            case 1:
                printf("\nEnter Metro start station:");
                gets(src);
                printf("\nEnter Metro destination station:");
                gets(dest);
                callG(src, dest);
                break;
            case 2:
                printf("Enter your ticket number: ");
                scanf("%d", &tic_num);
                ticket_ptr p = searchticket(tic_num);
                if(p)
                {
                    printf("Ticket number: %d\n",p->ticket_no);
                    printf("Source: %s\n", p->src);
                    printf("Platform for source station: %d\n", p->platform);
                    printf("Destination: %s\n", p->dest);
                    printf("Fare: %d\n", p->fare_price);
                }
                else 
                    printf("\nTicket not found");
                break;
            
            case 3:
                printf("\nEnter your ticket number: ");
                scanf("%d", &tic_num);
                if(deleteticket(tic_num))
                    printf("\nReservation cancelled!");
                else
                    printf("Reservation does not exist.");
                break;

            case 4:
                break;

            default:
                printf("Invalid input. Please enter again");
                break;
            
        }
        printf("\n1 - To book a new Ticket\n");
        printf("2 - To view your Ticket details\n");
        printf("3 - Cancel Reservation\n");
        printf("4 - Exit");
        printf("\nEnter choice:");
        scanf("%d",&ch);
        getchar();
    } 
    return 0;
}