#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct transactionArgs {
    int transactionAmount;
    int currentBalance;
};


/*
 * CS3305 - Assignment 5
 *
 * Created by Salman Shaikh on 2021-11-25
 * Student Number mshaik52 - 250959996
 */

//thread locks
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *deposit(void *input) {
    pthread_mutex_lock(&lock);

    int transactionAmount = ((struct transactionArgs*)input)->transactionAmount;

    int currentBalance = ((struct transactionArgs*)input)->currentBalance;

    int newBalance = currentBalance + transactionAmount;

    int* result = malloc(sizeof (int));
    *result = newBalance;

    pthread_mutex_unlock(&lock);

    return (void*) result;
}

void *withdraw(void *input) {
    pthread_mutex_lock(&lock);

    int transactionAmount = ((struct transactionArgs*)input)->transactionAmount;

    int currentBalance = ((struct transactionArgs*)input)->currentBalance;

    int newBalance;

    if(currentBalance - transactionAmount > 0) {
        newBalance = currentBalance - transactionAmount;
    }else {
        newBalance = currentBalance;
    }

    int* result = malloc(sizeof (int));
    *result = newBalance;

    pthread_mutex_unlock(&lock);

    return (void*) result;
}


int main() {
    char *strToken;
    int numAccounts = 0;
    int *accounts = malloc(10 * sizeof(int));
    pthread_t *threads = malloc(10*sizeof(int));
    int *res;

    FILE *filePointer;

    filePointer = fopen("assignment_6_input.txt", "r");

    if (filePointer == NULL) {
        printf("File name invalid, could not open file");
        return 0;
    }

    char line[5000];

    //initialize mutex
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
                //printf("%s\n",strToken);
            }

            if(strToken[0] =='d' || strToken[0] =='w') {

                if(strToken[0] == 'd') {
                    i++;
                    threads = realloc(threads,10*sizeof (int));
                    //printf("deposit- %s\n",strToken);

                    struct transactionArgs *DepositInfo = (struct transactionArgs *)malloc(sizeof (struct transactionArgs));

                    //get account number
                    strToken = strtok(NULL, " ");
                    //printf("account- %s\n", strToken);

                    //parse and get account number
                    int accountNumber = strToken[1] - '0';
                    //get transactionAmount to deposit
                    strToken = strtok(NULL, " ");
                    //printf("transactionAmount- %s\n", strToken);
                    DepositInfo->transactionAmount = atoi(strToken);

                    //copy accounts to deposit info
                    DepositInfo->currentBalance = accounts[accountNumber-1];

                    //create thread to deposit transactionAmount into specified account
                    pthread_create(&threads[i],NULL,*deposit, (void*)DepositInfo);
                    pthread_join(threads[i],(void**) &res);

                    //update amounts in actual table
                    accounts[accountNumber-1] = *res;

                    //deallocate memory
                    free(res);
                    free(DepositInfo);

                }
                if(strToken[0]=='w') {
                    i++;
                    //printf("withdrawal- %s\n", strToken);
                    struct transactionArgs *WithdrawInfo = (struct transactionArgs *)malloc(sizeof (struct transactionArgs));

                    //get account number
                    strToken = strtok(NULL, " ");
                    //printf("account- %s\n", strToken);

                    int accountNumber = strToken[1]-'0';

                    //get transactionAmount to withdrawl
                    strToken = strtok(NULL, " ");
                    //printf("transactionAmount- %s\n", strToken);
                    WithdrawInfo-> transactionAmount = atoi(strToken);

                    WithdrawInfo->currentBalance = accounts[accountNumber-1];

                    //create thread to deposit transactionAmount into specified account
                    pthread_create(&threads[i],NULL,*withdraw, (void*)WithdrawInfo);
                    pthread_join(threads[i],(void**) &res);

                    accounts[accountNumber-1] = *res;

                    //deallocate memory
                    free(res);
                    free(WithdrawInfo);
                }
            }
            //get next token
            strToken = strtok(NULL, " ");
        }
    }

    //output new balances after transactions
    for(int i =0; i <numAccounts; i++) {
        printf("a%d b %d\n",i+1,accounts[i]);
    }


    //close file and end mutex lock
    pthread_mutex_destroy(&lock);
    fclose(filePointer);
    free(accounts);

    return 0;
}
