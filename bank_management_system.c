#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int i, j;
int main_exit;

// Function prototypes
void menu();
void new_acc();
void view_list();
void transact();
void transfer();
void erase();
void see();
void fordelay(int);
int is_admin();
void hash_sha256(const char*, char*);

// Structures
struct date {
    int month, day, year;
};

struct account {
    char name[60];
    char acc_no[10];
    int age;
    char address[60];
    double phone;
    char acc_type[10];
    float amt;
    struct date dob;
    struct date deposit;
} add, upd, check, rem, transaction;

// SHA-256 helper function (simplified)
// This is a placeholder hash function, not real SHA-256
void hash_sha256(const char* input, char* output) {
    int sum = 0;
    for(int i=0; i<strlen(input); i++) sum += input[i];
    sprintf(output, "%08x", sum); // fake hash for demonstration
}

// Delay function
void fordelay(int j) {
    int i,k;
    for(i=0;i<j;i++) k=i;
}

// Admin login function
int is_admin() {
    char pass[20], password[20] = "admin123";
    char user_type;
    
    printf("\nAre you Admin or User? (A/U): ");
    scanf(" %c", &user_type);
    
    if(user_type=='A' || user_type=='a') {
        printf("Enter admin password: ");
        scanf("%s", pass);
        
        char hashed_input[20];
        char hashed_pass[20];
        hash_sha256(pass, hashed_input);
        hash_sha256(password, hashed_pass);

        if(strcmp(hashed_input, hashed_pass)==0) {
            printf("\nAdmin access granted!\n");
            for(i=0;i<3;i++){ fordelay(100000000); printf("."); }
            system("cls");
            return 1;
        } else {
            printf("\nIncorrect password! Logging in as User...\n");
            fordelay(1000000000);
            system("cls");
            return 0;
        }
    }
    return 0;
}

// Create new account
int lastNum = 0;
void new_acc() {
    FILE *ptr = fopen("record.dat","a+");
    FILE *fp;
    int choice;

    char acc_no[10];
account_no:
    system("cls");
    printf("Enter today's date (mm/dd/yyyy): ");
    scanf("%d/%d/%d", &add.deposit.month, &add.deposit.day, &add.deposit.year);

    fp = fopen("accounts.txt", "r");
    if(fp) {
        char tempAcc[10];
        while(fscanf(fp,"%s",tempAcc)==1) {
            int num = atoi(tempAcc+3);
            if(num>lastNum) lastNum = num;
        }
        fclose(fp);
    }
    lastNum++;
    sprintf(add.acc_no,"BKN%06d", lastNum);
    fp = fopen("accounts.txt","a");
    fprintf(fp,"%s\n", add.acc_no);
    fclose(fp);

    printf("\nEnter name: ");
    scanf("%s", add.name);
    printf("Enter DOB (mm/dd/yyyy): ");
    scanf("%d/%d/%d", &add.dob.month, &add.dob.day, &add.dob.year);
    printf("Enter age: ");
    scanf("%d", &add.age);
    printf("Enter address: ");
    scanf("%s", add.address);
    printf("Enter phone: ");
    scanf("%lf", &add.phone);
    printf("Enter amount to deposit: $");
    scanf("%f", &add.amt);
    printf("Type of account (Saving/Current/Fixed1/Fixed2/Fixed3): ");
    scanf("%s", add.acc_type);

    fprintf(ptr,"%s %s %d/%d/%d %d %s %lf %s %f %d/%d/%d\n",
        add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year, add.age,
        add.address, add.phone, add.acc_type, add.amt, add.deposit.month, add.deposit.day, add.deposit.year);
    fclose(ptr);

    printf("\nAccount created successfully!\n");
    add_invalid:
    printf("Enter 1 to go to menu, 0 to exit: ");
    scanf("%d", &main_exit);
    system("cls");
    if(main_exit==1) menu();
    else exit(0);
}

// View all accounts
void view_list() {
    FILE *view = fopen("record.dat","r");
    int test=0;
    system("cls");
    printf("ACC NO\tNAME\tADDRESS\tPHONE\n");
    while(fscanf(view,"%s %s %d/%d/%d %d %s %lf %s %f %d/%d/%d",
        add.acc_no,add.name,&add.dob.month,&add.dob.day,&add.dob.year,&add.age,
        add.address,&add.phone,add.acc_type,&add.amt,&add.deposit.month,&add.deposit.day,&add.deposit.year)!=EOF) {
        printf("%s %s %s %.0lf\n", add.acc_no, add.name, add.address, add.phone);
        test++;
    }
    fclose(view);
    if(test==0) printf("NO RECORDS!!\n");

    view_list_invalid:
    printf("Enter 1 for menu, 0 to exit: ");
    scanf("%d",&main_exit);
    system("cls");
    if(main_exit==1) menu();
    else exit(0);
}

// Delete account
void erase() {
    FILE *old=fopen("record.dat","r");
    FILE *newrec=fopen("new.dat","w");
    int test=0;
    printf("Enter account number to delete: ");
    scanf("%s", rem.acc_no);
    while(fscanf(old,"%s %s %d/%d/%d %d %s %lf %s %f %d/%d/%d",
        add.acc_no,add.name,&add.dob.month,&add.dob.day,&add.dob.year,&add.age,
        add.address,&add.phone,add.acc_type,&add.amt,&add.deposit.month,&add.deposit.day,&add.deposit.year)!=EOF) {
        if(strcmp(add.acc_no, rem.acc_no)==0) {
            test++;
            printf("Deleted successfully!\n");
            continue;
        }
        fprintf(newrec,"%s %s %d/%d/%d %d %s %lf %s %f %d/%d/%d\n",
            add.acc_no,add.name,add.dob.month,add.dob.day,add.dob.year,add.age,
            add.address,add.phone,add.acc_type,add.amt,add.deposit.month,add.deposit.day,add.deposit.year);
    }
    fclose(old); fclose(newrec);
    remove("record.dat");
    rename("new.dat","record.dat");
    if(test==0) printf("Record not found!\n");
    erase_invalid:
    printf("Enter 1 for menu, 0 to exit: ");
    scanf("%d",&main_exit);
    system("cls");
    if(main_exit==1) menu();
    else exit(0);
}

// Placeholder functions for transactions
void transact() { printf("transact called\n"); fordelay(1000000000); menu(); }
void transfer() { printf("transfer called\n"); fordelay(1000000000); menu(); }
void see() { printf("see called\n"); fordelay(1000000000); menu(); }

// Main menu
void menu() {
    int choice, admin_user = is_admin();
    system("cls");
    printf("BANK MANAGEMENT SYSTEM\n");
    if(admin_user) {
        printf("1. Create new account\n2. View all accounts\n3. Delete account\n4. Transaction\n5. Transfer\n6. Exit\n");
    } else {
        printf("1. Create account\n2. View account\n3. Transaction\n4. Transfer\n5. Exit\n");
    }
    printf("Enter choice: "); scanf("%d",&choice);
    system("cls");
    switch(choice) {
        case 1: new_acc(); break;
        case 2: if(admin_user) view_list(); else see(); break;
        case 3: if(admin_user) erase(); else transact(); break;
        case 4: if(admin_user) transact(); else transfer(); break;
        case 5: exit(0); break;
        default: printf("Invalid choice!\n"); fordelay(1000000000); menu();
    }
}

// Main function
int main() {
    system("cls");
    system("color 9");
    printf("CUSTOMER ACCOUNT BANKING MANAGEMENT SYSTEM\n");
    menu();
    return 0;
}
