#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadAccounts();
int findAccount(const char *cardNumber);
int verifyPin(int index, const char *pin);
int withdraw(int index, double amount);
void saveAccounts();

#define MAX_ACCOUNTS 100
#define MAX_LINE 256

typedef struct {
    char cardNumber[20];
    char pin[6];
    double balance;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

int main(void) {
    char cardNumber[20], pin[10];
    double amount;
    char option;

    loadAccounts("accounts.csv");

    printf("Enter card number: ");
    scanf("%s", cardNumber);

    int index = findAccount(cardNumber);
    if (index == -1) {
        printf("Invalid card number.\n");
        return 0;
    }

    printf("Enter PIN: ");
    scanf("%s", pin);

    if (!verifyPin(index, pin)) {
        printf("Invalid PIN.\n");
        return 0;
    }

    printf("A. Blance Inquiry\n");
    printf("B. Withdraw\n");
    printf("Choose an option: ");
    scanf(" %c", &option);

    if (option == 'A' || option == 'a') 
    {
        printf("Current balance: %.2f\n", accounts[index].balance);
        return 0;
    }
    else if (option == 'B' || option == 'b') 
    {
        printf("Enter withdrawal amount: ");
        scanf("%lf", &amount);

        if (withdraw(index, amount)) 
        {
            printf("Withdrawal successful. New balance: %.2f\n", accounts[index].balance);
            saveAccounts("accounts.csv");
        } 
        else 
        {
            printf("Insufficient balance or invalid amount.\n");
        }
    }
    else
    {
        printf("Invalid option selected.\n");
    }
    return 0;
}


void loadAccounts() 
{
    FILE *file = fopen("accounts.csv", "r");
    if (!file) 
    {
        printf("Error opening file.\n");
        exit(1);
    }
    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) 
    {
        Account acc;
        char *token = strtok(line, ",");
        strcpy(acc.cardNumber, token);

        token = strtok(NULL, ",");
        strcpy(acc.pin, token);

        token = strtok(NULL, ",");
        acc.balance = atof(token);

        accounts[accountCount] = acc;
        accountCount++;
    }
    fclose(file);
}

int findAccount(const char *cardNumber) {
    for (int i = 0; i < accountCount; i++) {
        if (strcmp(accounts[i].cardNumber, cardNumber) == 0) {
            return i;
        }
    }
    return -1;
}

int verifyPin(int index, const char *pin) {
    return strcmp(accounts[index].pin, pin) == 0;
}

int withdraw(int index, double amount) {
    if (amount <= 0) return 0;
    if (accounts[index].balance >= amount) {
        accounts[index].balance -= amount;
        return 1;
    }
    return 0;
}

void saveAccounts() {
    FILE *file = fopen("accounts.csv", "w");
    if (!file) {
        printf("Error saving file.\n");
        return;
    }
    for (int i = 0; i < accountCount; i++) {
        fprintf(file, "%s,%s,%.2f\n",
                accounts[i].cardNumber,
                accounts[i].pin,
                accounts[i].balance);
    }
    fclose(file);
}