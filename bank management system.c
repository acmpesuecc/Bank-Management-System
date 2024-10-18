#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main_exit;

struct date {
    int month, day, year;
};

struct account {
    char name[60];
    int acc_no, age;
    char address[60];
    char citizenship[15];
    double phone;
    char acc_type[10];
    float amt;
    struct date dob;
    struct date deposit;
    struct date withdraw;
};

struct account add, upd, check, rem, transaction;

float interest(float t, float amount, int rate) {
    return (rate * t * amount) / 100.0;
}

void fordelay(int j) {
    int i, k;
    for (i = 0; i < j; i++) k = i;
}

void new_acc() {
    FILE *ptr;
    ptr = fopen("record.dat", "a+");
    int account_no;

    while (1) {
        system("cls");
        printf("\t\t\t\xB2\xB2\xB2 ADD RECORD  \xB2\xB2\xB2\xB2");
        printf("\n\n\nEnter today's date (mm/dd/yyyy): ");
        scanf("%d/%d/%d", &add.deposit.month, &add.deposit.day, &add.deposit.year);

        printf("\nEnter the account number: ");
        scanf("%d", &account_no);

        // Check for duplicate account numbers
        int exists = 0;
        while (fscanf(ptr, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d\n",
                      &add.acc_no, add.name, &add.dob.month, &add.dob.day, &add.dob.year,
                      &add.age, add.address, add.citizenship, &add.phone, add.acc_type, &add.amt,
                      &add.deposit.month, &add.deposit.day, &add.deposit.year) != EOF) {
            if (account_no == add.acc_no) {
                exists = 1;
                break;
            }
        }

        if (exists) {
            printf("Account no. already in use!");
            fordelay(1000000000);
            continue; // Go back to the start of the loop
        }

        add.acc_no = account_no;
        printf("\nEnter the name: ");
        scanf("%s", add.name);
        printf("\nEnter the date of birth (mm/dd/yyyy): ");
        scanf("%d/%d/%d", &add.dob.month, &add.dob.day, &add.dob.year);
        printf("\nEnter the age: ");
        scanf("%d", &add.age);
        printf("\nEnter the address: ");
        scanf("%s", add.address);
        printf("\nEnter the citizenship number: ");
        scanf("%s", add.citizenship);
        printf("\nEnter the phone number: ");
        scanf("%lf", &add.phone);
        printf("\nEnter the amount to deposit: $");
        scanf("%f", &add.amt);
        printf("\nType of account:\n\t#Saving\n\t#Current\n\t#Fixed1 (for 1 year)\n\t#Fixed2 (for 2 years)\n\t#Fixed3 (for 3 years)\n\n\tEnter your choice: ");
        scanf("%s", add.acc_type);

        fprintf(ptr, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d\n",
                add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year, add.age,
                add.address, add.citizenship, add.phone, add.acc_type, add.amt,
                add.deposit.month, add.deposit.day, add.deposit.year);

        fclose(ptr);
        printf("\nAccount created successfully!");

        // Main menu option
        while (1) {
            printf("\n\n\n\t\tEnter 1 to go to the main menu and 0 to exit: ");
            scanf("%d", &main_exit);
            system("cls");

            if (main_exit == 1) {
                return; // Return to the menu function
            } else if (main_exit == 0) {
                exit(0);
            } else {
                printf("\nInvalid!\a");
            }
        }
    }
}

void transact(void) {
    int choice, test = 0;
    FILE *old, *newrec;
    old = fopen("record.dat", "r");
    newrec = fopen("new.dat", "w");

    printf("Enter the account no. of the customer: ");
    scanf("%d", &transaction.acc_no);

    while (fscanf(old, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d",
                  &add.acc_no, add.name, &add.dob.month, &add.dob.day, &add.dob.year,
                  &add.age, add.address, add.citizenship, &add.phone, add.acc_type,
                  &add.amt, &add.deposit.month, &add.deposit.day, &add.deposit.year) != EOF) {
        if (add.acc_no == transaction.acc_no) {
            test = 1;
            if (strcmpi(add.acc_type, "fixed1") == 0 || strcmpi(add.acc_type, "fixed2") == 0 || strcmpi(add.acc_type, "fixed3") == 0) {
                printf("\a\a\a\n\nYOU CANNOT DEPOSIT OR WITHDRAW CASH IN FIXED ACCOUNTS!!!!!");
                fordelay(1000000000);
                fclose(old);
                fclose(newrec);
                system("cls");
                return; // Return to the menu function
            }

            printf("\n\nDo you want to\n1.Deposit\n2.Withdraw?\n\nEnter your choice(1 for deposit and 2 for withdraw): ");
            scanf("%d", &choice);

            if (choice == 1) {
                printf("Enter the amount you want to deposit: $ ");
                scanf("%f", &transaction.amt);
                // Validate the deposit amount
                if (transaction.amt <= 0) {
                    printf("Invalid amount! Please enter a positive value.\n");
                } else {
                    add.amt += transaction.amt;
                    fprintf(newrec, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d\n",
                            add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year,
                            add.age, add.address, add.citizenship, add.phone, add.acc_type,
                            add.amt, add.deposit.month, add.deposit.day, add.deposit.year);
                    printf("\n\nDeposited successfully!");
                }
            } else if (choice == 2) {
                printf("Enter the amount you want to withdraw: $ ");
                scanf("%f", &transaction.amt);
                // Validate the withdrawal amount
                if (transaction.amt <= 0) {
                    printf("Invalid amount! Please enter a positive value.\n");
                } else if (add.amt < transaction.amt) {
                    printf("\nInsufficient funds! Cannot withdraw $%.2f from your account with balance $%.2f.\n", transaction.amt, add.amt);
                } else {
                    add.amt -= transaction.amt;
                    fprintf(newrec, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d\n",
                            add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year,
                            add.age, add.address, add.citizenship, add.phone, add.acc_type,
                            add.amt, add.deposit.month, add.deposit.day, add.deposit.year);
                    printf("\n\nWithdrawn successfully!");
                }
            }
        } else {
            fprintf(newrec, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d\n",
                    add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year,
                    add.age, add.address, add.citizenship, add.phone, add.acc_type,
                    add.amt, add.deposit.month, add.deposit.day, add.deposit.year);
        }
    }

    fclose(old);
    fclose(newrec);
    remove("record.dat");
    rename("new.dat", "record.dat");

    if (test != 1) {
        printf("\n\nRecord not found!!");
        while (1) {
            printf("\n\n\nEnter 0 to try again, 1 to return to main menu and 2 to exit: ");
            scanf("%d", &main_exit);
            system("cls");
            if (main_exit == 0) {
                transact();
                return; // Return after trying again
            } else if (main_exit == 1) {
                return; // Return to the menu function
            } else if (main_exit == 2) {
                exit(0);
            } else {
                printf("\nInvalid!");
            }
        }
    } else {
        printf("\nEnter 1 to go to the main menu and 0 to exit: ");
        scanf("%d", &main_exit);
        system("cls");
        if (main_exit == 1)
            return; // Return to the menu function
        else
            exit(0);
    }
}

int main() {
    int choice;
    while (1) {
        printf("\n\t\t\t\xB2\xB2\xB2 BANK MANAGEMENT SYSTEM \xB2\xB2\xB2\xB2");
        printf("\n\n1. New Account");
        printf("\n2. Deposit / Withdraw");
        printf("\n3. Exit");
        printf("\n\nChoose an option: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                new_acc();
                break;
            case 2:
                transact();
                break;
            case 3:
                exit(0);
            default:
                printf("\nInvalid option! Please try again.");
        }
    }
    return 0;
}
