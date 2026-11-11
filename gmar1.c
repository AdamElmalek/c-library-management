#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define MEMNUM 300

typedef struct {
    int year;
    int month;
    int day;
}Date;


typedef struct {
    char* BookName;
    char* AuthorName;
    Date returnDate;
}Books;


typedef struct {
    char* Name;
    char id[10];
    Date dateOfBirth;
    int nBooks;
    Books LoanBook[4]; // Maximum of 4 loaned books per member
}LibMember;

void print_member(LibMember* Mem);
int search_id(LibMember* Mem, int nMems, char* id);
int menu();
int add_member(LibMember* Mem, int nMems);
void loan_books(LibMember* Mem, int nMems);
void return_book(LibMember* Mem, int nMems);
void check_books_overdue(LibMember* Mem, int nMems);
int delete_member(LibMember* Mem, int nMems);
void print_members(LibMember* Mem, int nMems);
void quit(LibMember* Mem, int nMems);
void sortById(LibMember* Mem, int nMems, char* id);


void main() {
    LibMember Mem[MEMNUM];
    int choice, nMems = 0;
    for (int i = 0; i < MEMNUM; i++) {
        Mem[i].id[0] = '\0';
        Mem[i].nBooks = 0;
        Mem[i].dateOfBirth.day = 0;
        Mem[i].dateOfBirth.month = 0;
        Mem[i].dateOfBirth.year = 0;
    }
    while (1) {
        choice = menu();
        switch (choice) {
        case 1:
            nMems = add_member(Mem, nMems);
            break;
        case 2:
            loan_books(Mem, nMems);
            break;
        case 3:
            return_book(Mem, nMems);
            break;
        case 4:
            check_books_overdue(Mem, nMems);
            break;
        case 5:
            nMems = delete_member(Mem, nMems);
            break;
        case 6:
            print_members(Mem, nMems);
            break;
        case 7:
            quit(Mem, nMems);
            return;
        default:
            break;
        }
        printf("------------------------------------\n");
    }
}

void print_member(LibMember* Mem) {
    printf("Full name: %s\n", Mem->Name);
    printf("Id: %s\n", Mem->id);
    printf("Birthday: %d/%d/%d\n", Mem->dateOfBirth.day, Mem->dateOfBirth.month, Mem->dateOfBirth.year);
    printf("How many loaned books: %d\n", Mem->nBooks);
    for (int i = 0; i < Mem->nBooks; i++) {
        printf("Book %d: Book name: %s, Author name: %s, Return date: %d/%d/%d\n", i + 1, Mem->LoanBook[i].BookName, Mem->LoanBook[i].AuthorName, Mem->LoanBook[i].returnDate.day, Mem->LoanBook[i].returnDate.month, Mem->LoanBook[i].returnDate.year);
    }
}

int search_id(LibMember* Mem, int nMems, char* id) {
    int counter = 0;
    for (int i = 0; i < nMems; i++) {
        for (int j = 0; Mem[i].id[j] != '\0'; j++) {
            if (Mem[i].id[j] == id[j])
                counter++;
        }
        if (counter == strlen(id))
            return i;
        counter = 0;
    }
    return -1;
}

int menu() {
    int choice = 0;
    while (1) {
        printf("Database System Menu:\n   1. Add Member\n   2. Loan books\n   3. Return books\n   4. Check books overdue\n   5. Delete Member\n   6. Display all members\n   7. Quit\n");
        printf("Please enter your choice: ");
        scanf("%d", &choice);
        if (choice > 0 && choice < 8)
            return choice;
        printf("You entered a wrong number!!!! please try agian\n");
    }
}

int add_member(LibMember* Mem, int nMems) {
    char name[100], id[100];
    int j = 0, counter = 0, year, month, day;

    while (1) { //Check name
        name[0] = '\0';
        counter = 0;
        printf("Please enter full name: ");
        int c = getchar();//gets() won't work without this line!
        gets(name);
        for (int i = 0; i < strlen(name); i++) {
            if (name[i] != ' ' && i != j) {
                if (name[i] >= 65 && name[i] <= 90 && j < i)
                    name[i] += 32;
                else if (name[i] < 97 || name[i] > 122) {
                    printf("The name you entered has something else than letters!!\n");
                    break;
                    i = strlen(name);
                }
                counter++;
            }
            else if (j == i) {
                if (name[i] >= 97 && name[i] <= 122) {
                    name[i] -= 32;
                }
                else if (name[i] < 65 || name[i] > 90) {
                    printf("The name you entered has something else than letters!!\n");
                    i = strlen(name);
                }
                counter++;
            }
            else {
                j = i + 1;
                counter++;
            }
        }
        if (counter == strlen(name))
            break;
    }
    printf("Please enter id: "); //enter id
    gets(id);
    if (strlen(id) != 9) {
        printf("the id you typed is wrong!!!\n");
        return nMems;
    }
    else {
        int idcheck = search_id(Mem, nMems, id);
        if (idcheck >= 0) {
            printf("this Member already exists can't be added again!!!\n");
            return nMems;
        }
    }
    printf("please enter year: ");
    scanf("%d", &year);
    printf("please enter month: ");
    scanf("%d", &month);
    printf("please enter day: ");
    scanf("%d", &day);
    if (year < 1900 || month < 1 || month > 12 || day < 1 || day > 30) {
        printf("This date is not valid!!!!!\n");
        return nMems;
    }
    Mem[++nMems - 1].Name = (char*)malloc((strlen(name) + 1) * sizeof(char));
    strcpy(Mem[nMems - 1].Name, name);
    strcpy(Mem[nMems - 1].id, id);
    Mem[nMems - 1].dateOfBirth.day = day;
    Mem[nMems - 1].dateOfBirth.month = month;
    Mem[nMems - 1].dateOfBirth.year = year;
    Mem[nMems - 1].nBooks = 0;
    for (int i = 0; i < 4; i++) {
        Mem[nMems - 1].LoanBook[i].AuthorName = (char*)calloc(1, sizeof(char));
        Mem[nMems - 1].LoanBook[i].BookName = (char*)calloc(1, sizeof(char));
        Mem[nMems - 1].LoanBook[i].returnDate.day = 0;
        Mem[nMems - 1].LoanBook[i].returnDate.month = 0;
        Mem[nMems - 1].LoanBook[i].returnDate.year = 0;
    }
    sortById(Mem, nMems, Mem[nMems - 1].id);
    printf("Member has been added successfully!!\n");
    return nMems;
}

void loan_books(LibMember* Mem, int nMems) {
    char id[100];
    printf("please enter member id: ");
    int c = getchar();//gets() won't work without this line!
    gets(id);
    int check = search_id(Mem, nMems, id);
    if (check == -1) {
        printf("the member doesn't exist!\n");
        return;
    }
    print_member(&Mem[check]);
    if (Mem[check].nBooks == 4) {
        printf("member has 4 loans!! we can't loan more than 4 books for one member!!\n");
        return;
    }
    printf("please enter author name: ");
    gets(id);
    Mem[check].LoanBook[Mem[check].nBooks].AuthorName = (char*)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(Mem[check].LoanBook[Mem[check].nBooks].AuthorName, id);
    printf("please enter book name: ");
    gets(id);
    Mem[check].LoanBook[Mem[check].nBooks].BookName = (char*)malloc((strlen(id) + 1) * sizeof(char));
    strcpy(Mem[check].LoanBook[Mem[check].nBooks].BookName, id);
    //Using time.h
    time_t now;
    time(&now);
    struct tm* current_time = localtime(&now);

    // Increment month by 1 because it starts with 0
    current_time->tm_mon += 1;
    if (current_time->tm_mon == 12) {
        current_time->tm_mon = 0; // Reset month to January
        current_time->tm_year += 1; // Increment year
    }
    Mem[check].LoanBook[Mem[check].nBooks].returnDate.day = current_time->tm_mday;
    Mem[check].LoanBook[Mem[check].nBooks].returnDate.month = current_time->tm_mon + 1;
    Mem[check].LoanBook[Mem[check].nBooks].returnDate.year = current_time->tm_year + 1900;
    //Time added using time.h
    Mem[check].nBooks++;
}

void return_book(LibMember* Mem, int nMems) {
    char id[100], Book_Name[100];
    int flag = -1;
    printf("please enter member id: ");
    int c = getchar();//To clear the buffer, gets() won't work without this line!
    gets(id);
    int check = search_id(Mem, nMems, id);
    if (check == -1) {
        printf("the member doesn't exist!\n");
        return;
    }
    print_member(&Mem[check]);
    if (Mem[check].nBooks == 0) {
        printf("this member doesn't have any loaned books!!\n");
        return;
    }
    printf("Please enter book name: ");
    gets(Book_Name);
    for (int i = 0; i < Mem[check].nBooks; i++) {
        if (!strcmp(Mem[check].LoanBook[i].BookName, Book_Name)) {
            free(Mem[check].LoanBook[i].AuthorName);
            free(Mem[check].LoanBook[i].BookName);
            flag = i;
        }
    }
    if (flag == -1) {
        printf("the book is not found!\n");
        return;
    }
    for (int i = flag, j = flag + 1; j < Mem[check].nBooks; i++) {
        Mem[check].LoanBook[i].AuthorName = (char*)calloc(Mem[check].LoanBook[i].AuthorName, strlen(Mem[check].LoanBook[j].AuthorName) + 1);
        strcpy(Mem[check].LoanBook[i].AuthorName, Mem[check].LoanBook[j].AuthorName);
        Mem[check].LoanBook[i].BookName = (char*)calloc(Mem[check].LoanBook[i].BookName, strlen(Mem[check].LoanBook[j].BookName) + 1);
        strcpy(Mem[check].LoanBook[i].BookName, Mem[check].LoanBook[j].BookName);
        Mem[check].LoanBook[i].returnDate.day = Mem[check].LoanBook[j].returnDate.day;
        Mem[check].LoanBook[i].returnDate.month = Mem[check].LoanBook[j].returnDate.month;
        Mem[check].LoanBook[i].returnDate.year = Mem[check].LoanBook[j].returnDate.year;
        free(Mem[check].LoanBook[j].BookName);
        free(Mem[check].LoanBook[j++].AuthorName);
    }
    Mem[check].nBooks--;
}

void check_books_overdue(LibMember* Mem, int nMems) {
    int flag = 0;
    time_t now;
    time(&now);
    struct tm* current_time = localtime(&now);
    // Increment month by 1 because it starts with 0
    current_time->tm_mon += 1;
    for (int i = 0; i < nMems; i++) {
        for (int j = 0; j < Mem[i].nBooks; j++) {
            if (Mem[i].LoanBook[j].returnDate.year < (current_time->tm_year + 1900)) {
                printf("Member Name: %s , Id: %s\n", Mem[i].Name, Mem[i].id);
                printf("Book over due: Book name: %s, Author name: %s\n", Mem[i].LoanBook[j].BookName, Mem[i].LoanBook[j].AuthorName);
                flag++;
            }
            else if (Mem[i].LoanBook[j].returnDate.year == (current_time->tm_year + 1900)) {
                if (Mem[i].LoanBook[j].returnDate.month < current_time->tm_mon) {
                    printf("Member Name: %s , Id: %s\n", Mem[i].Name, Mem[i].id);
                    printf("Book over due: Book name: %s, Author name: %s\n", Mem[i].LoanBook[j].BookName, Mem[i].LoanBook[j].AuthorName);
                    flag++;
                }
            }
        }
    }
    if (!flag)
        printf("There are no overdue books\n");
}

int delete_member(LibMember* Mem, int nMems) {
    char id[100];
    printf("please enter member id: ");
    int c = getchar();//To clear the buffer, gets() won't work without this line!
    gets(id);
    int check = search_id(Mem, nMems, id);
    if (check == -1) {
        printf("the member doesn't exist!\n");
        return;
    }
    print_member(&Mem[check]);
    free(Mem[check].Name);
    for (int i = 0; i < Mem[check].nBooks; i++) {
        free(Mem[check].LoanBook[i].BookName);
        free(Mem[check].LoanBook[i].AuthorName);
    }
    for (int i = check; i < nMems - 1; i++) {
        Mem[i].Name = (char*)malloc((strlen(Mem[i + 1].Name) + 1) * sizeof(char));
        strcpy(Mem[i].Name, Mem[i + 1].Name);
        strcpy(Mem[i].id, Mem[i + 1].id);
        free(Mem[i + 1].Name);
        Mem[i + 1].id[0] = '\0';
        Mem[i].dateOfBirth.day = Mem[i + 1].dateOfBirth.day;
        Mem[i + 1].dateOfBirth.day = 0;
        Mem[i].dateOfBirth.month = Mem[i + 1].dateOfBirth.month;
        Mem[i + 1].dateOfBirth.month = 0;
        Mem[i].dateOfBirth.year = Mem[i + 1].dateOfBirth.year;
        Mem[i + 1].dateOfBirth.year = 0;
        Mem[i].nBooks = Mem[i + 1].nBooks;
        Mem[i + 1].nBooks = 0;
        for (int j = 0; j < Mem[i].nBooks; j++) {
            Mem[i].LoanBook[j].AuthorName = (char*)malloc((strlen(Mem[i + 1].LoanBook[j].AuthorName) + 1) * sizeof(char));
            strcpy(Mem[i].LoanBook[j].AuthorName, Mem[i + 1].LoanBook[j].AuthorName);
            free(Mem[i + 1].LoanBook[j].AuthorName);
            Mem[i].LoanBook[j].BookName = (char*)malloc((strlen(Mem[i + 1].LoanBook[j].BookName) + 1) * sizeof(char));
            strcpy(Mem[i].LoanBook[j].BookName, Mem[i + 1].LoanBook[j].BookName);
            free(Mem[i + 1].LoanBook[j].BookName);
            Mem[i].LoanBook[j].returnDate.day = Mem[i + 1].LoanBook[j].returnDate.day;
            Mem[i].LoanBook[j].returnDate.month = Mem[i + 1].LoanBook[j].returnDate.month;
            Mem[i].LoanBook[j].returnDate.year = Mem[i + 1].LoanBook[j].returnDate.year;
        }
    }
    return nMems - 1;
}

void print_members(LibMember* Mem, int nMems) {
    for (int i = 0; i < nMems; i++)
        print_member(&Mem[i]);
}

void quit(LibMember* Mem, int nMems) {
    printf(" \tyou pressed 7 which is the quit button.\n \tThe program is gonna exit now.\n \tThank you for using the program.\n");
    for (int i = 0; i < nMems; i++) {
        free(Mem[i].Name);
        Mem[i].id[0] = '\0';
        for (int j = 0; j < Mem[i].nBooks; j++) {
            free(Mem[i].LoanBook[j].AuthorName);
            free(Mem[i].LoanBook[j].BookName);
        }
    }
}

void Swap_sortById(LibMember* Mem, LibMember* Tmp_Member, int flag, int flag2, int nMems) {
    for (int i = flag, k = flag2; i < nMems; i++, k++) {
        Tmp_Member[i].Name = (char*)malloc((strlen(Mem[k].Name) + 1) * sizeof(char));
        strcpy(Tmp_Member[i].Name, Mem[k].Name);
        free(Mem[k].Name);
        strcpy(Tmp_Member[i].id, Mem[k].id);
        Tmp_Member[i].dateOfBirth.day = Mem[k].dateOfBirth.day;
        Tmp_Member[i].dateOfBirth.month = Mem[k].dateOfBirth.month;
        Tmp_Member[i].dateOfBirth.year = Mem[k].dateOfBirth.year;
        Tmp_Member[i].nBooks = Mem[k].nBooks;
        for (int j = 0; j < Mem[i].nBooks; j++) {
            Tmp_Member[i].LoanBook[j].AuthorName = (char*)malloc((strlen(Mem[k].LoanBook[j].AuthorName) + 1) * sizeof(char));
            strcpy(Tmp_Member[i].LoanBook[j].AuthorName, Mem[k].LoanBook[j].AuthorName);
            free(Mem[k].LoanBook[j].AuthorName);
            Tmp_Member[i].LoanBook[j].BookName = (char*)malloc((strlen(Mem[k].LoanBook[j].BookName) + 1) * sizeof(char));
            strcpy(Tmp_Member[i].LoanBook[j].BookName, Mem[k].LoanBook[j].BookName);
            free(Mem[k].LoanBook[j].BookName);
            Tmp_Member[i].LoanBook[j].returnDate.day = Mem[k].LoanBook[j].returnDate.day;
            Tmp_Member[i].LoanBook[j].returnDate.month = Mem[k].LoanBook[j].returnDate.month;
            Tmp_Member[i].LoanBook[j].returnDate.year = Mem[k].LoanBook[j].returnDate.year;
        }
    }
}

void sortById(LibMember* Mem, int nMems, char* id) {
    LibMember Tmp_Member[MEMNUM];
    int flag = -1;
    for (int i = 0; i < nMems; i++) {
        if (strcmp(Mem[i].id, id) > 0) {
            flag = i;
            Tmp_Member[i].Name = (char*)malloc((strlen(Mem[nMems - 1].Name) + 1) * sizeof(char));
            strcpy(Tmp_Member[i].Name, Mem[nMems - 1].Name);
            free(Mem[nMems - 1].Name);
            strcpy(Tmp_Member[i].id, Mem[nMems - 1].id);
            Tmp_Member[i].dateOfBirth.day = Mem[nMems - 1].dateOfBirth.day;
            Tmp_Member[i].dateOfBirth.month = Mem[nMems - 1].dateOfBirth.month;
            Tmp_Member[i].dateOfBirth.year = Mem[nMems - 1].dateOfBirth.year;
            Tmp_Member[i].nBooks = Mem[nMems - 1].nBooks;
            for (int j = 0; j < Mem[i].nBooks; j++) {
                Tmp_Member[i].LoanBook[j].AuthorName = (char*)malloc((strlen(Mem[nMems - 1].LoanBook[j].AuthorName) + 1) * sizeof(char));
                strcpy(Tmp_Member[i].LoanBook[j].AuthorName, Mem[nMems - 1].LoanBook[j].AuthorName);
                free(Mem[nMems - 1].LoanBook[j].AuthorName);
                Tmp_Member[i].LoanBook[j].BookName = (char*)malloc((strlen(Mem[nMems - 1].LoanBook[j].BookName) + 1) * sizeof(char));
                strcpy(Tmp_Member[i].LoanBook[j].BookName, Mem[nMems - 1].LoanBook[j].BookName);
                free(Mem[nMems - 1].LoanBook[j].BookName);
                Tmp_Member[i].LoanBook[j].returnDate.day = Mem[nMems - 1].LoanBook[j].returnDate.day;
                Tmp_Member[i].LoanBook[j].returnDate.month = Mem[nMems - 1].LoanBook[j].returnDate.month;
                Tmp_Member[i].LoanBook[j].returnDate.year = Mem[nMems - 1].LoanBook[j].returnDate.year;
            }
            break;
        }
    }
    if (flag == -1)
        return;
    Swap_sortById(Mem, Tmp_Member, flag + 1, flag, nMems);
    Swap_sortById(Tmp_Member, Mem, flag, flag, nMems);
}