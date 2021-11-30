#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

struct args {
    int accountNumber ;
    int amount;
};

/*
 * CS3305 - Assignment 5
 *
 * Created by Salman Shaikh on 2021-11-25
 * Student Number mshaik52 - 250959996
 */

//thread locks
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

#define initialSize 500

void *deposit(int accountNumber, int amount) {
    pthread_mutex_lock(&lock);


    pthread_mutex_unlock(&lock);
}

void *withdraw(int accountNumber, int amount) {
    pthread_mutex_lock(&lock);


    pthread_mutex_unlock(&lock);
}


int main() {
    char *strToken;
    int numAccounts = 0;
    int *accounts = malloc(1 * sizeof(int));

    FILE *filePointer;

    filePointer = fopen("assignment_6_input.txt", "r");

    if (filePointer == NULL) {
        printf("File name invalid, could not open file");
        return 0;
    }

    char line[5000];

    pthread_mutex_init(&lock, NULL);

    //while file not empty.
    while (!feof(filePointer)) {
        //put the line in line - or get a new line
        fgets(line, 5000, filePointer);
        char str[5000];
        strcpy(str, line);
        int i =0;

        //get first token from line
        strToken = strtok(line, " ");

        while (strToken != NULL) {
            //when there is an account ex starts with a1 and has b in it
            if (strToken[3] == 'b') {
                char accountBalance[50];
                strncpy(accountBalance, &line[5], 4);
                accounts[numAccounts] = atoi(accountBalance);
                numAccounts++;
                //allocate more memory to array
                if (numAccounts > 10) {
                    accounts = realloc(accounts, 20 * numAccounts * sizeof(int));
                }
            }

            //if client does transaction with bank account, if token starts with c
            if (strToken[0] == 'c') {
                pthread_t threads[numAccounts];
                printf("%s\n",strToken);
            }

            if(strToken[0] =='d' || strToken[0] =='w') {
                if(strToken[0] == 'd') {
                    printf("deposit- %s\n",strToken);

                    struct args *DepositInfo = (struct args *)malloc(sizeof (struct args));

                    //get account number
                    strToken = strtok(NULL, " ");
                    printf("account- %s\n", strToken);
                    DepositInfo->accountNumber = strToken;
                    //get amount to deposit
                    strToken = strtok(NULL, " ");
                    printf("amount- %s\n", strToken);
                    DepositInfo->amount = strToken;

                    //create thread to deposit amount into specified account
                    pthread_create();

                }
                if(strToken[0]=='w') {
                    printf("withdrawal- %s\n", strToken);
                    struct args *WithdrawInfo = (struct args *)malloc(sizeof (struct args));



                    //get account number
                    strToken = strtok(NULL, " ");
                    printf("account- %s\n", strToken);

                    //get amount to withdrawl
                    strToken = strtok(NULL, " ");
                    printf("amount- %s\n", strToken);

                    //create thread to deposit amount into specified account


                }

            }

            //get next token
            strToken = strtok(NULL, " ");
        }
    }


    //close file and end mutex lock
    pthread_mutex_destroy(&lock);
    fclose(filePointer);
    free(accounts);
    return 0;
}
