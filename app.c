#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>
#include<windows.h>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct record
{
    char name[30];
    long long int number;
    char address[30];
};
struct record_Link
{
    char name[30];
    long long int number;
    char address[30];
    struct record_Link *link;
};
typedef struct record RECORD;
typedef struct record_Link RECLINK;

void menu();
void create_Record(void);
void modify_Record(void);
void delete_Record(void);
void search_Record(void);
FILE *open_File(char);
RECLINK *create_List(FILE *, RECLINK *);
RECLINK *delete_List(RECLINK *, RECORD);
RECLINK *insert_List(RECLINK *, RECORD);
RECLINK *modify_List(RECLINK *, RECORD);
FILE *update_File(FILE *, RECLINK *);
RECLINK *empty_List(RECLINK *start)
{
    RECLINK *temp;
    while(start != NULL)
    {
        temp = start;
        start = start->link;
        free(temp);
    }
    return start;
}
RECLINK *print(RECLINK *start)
{
    RECLINK *p = start;
    printf("\n");
    printf(" ____________________________________________________________________ \n");
    printf("|=======================PHONEBOOK RECORD LIST========================|\n");
    printf("|____________________________________________________________________|\n");
    printf("|          Name           |      Number        |       Address       |\n");
    printf("|_________________________|____________________|_____________________|\n");
    while(p!=NULL)
    {
        printf("\t%s\t", p->name);
        printf("\t%lld\t", p->number);
        printf("\t%s\t\n", p->address);
        printf("|_________________________|____________________|_____________________|\n");
        p = p->link;
    }
    //printf("\n");
    return start;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    char c;
    menu();
    scanf("%c", &c);
    while(c/*(c=getchar())*/!='q')
    {
        switch(c)
        {
            case 'a':
                fflush(stdin);
                create_Record();
                break;
            case 'c':
                fflush(stdin);
                delete_Record();
                break;
            case 'b':
                fflush(stdin);
                modify_Record();
                break;
            case 'd':
                fflush(stdin);
                search_Record();
                break;
            case 'e':
                fflush(stdin);
                exit(0);
                break;
            default:
                printf("Invalid choice, Try again\n");
                break;
        }
        fflush(stdin);
        menu();
        scanf("%c", &c);
    }

}
void search_Record()
{
    char name[30];
    int i;
    long long int number;
    FILE *fp;
    RECLINK *start = NULL;
    RECLINK *p;
    int n, l, flag = 0;
    printf("Enter:\n1. To search by name\t\t2. To search by number\n\n");
    printf("Enter your choice: ");
    scanf("%d", &n);
    if(n == 1)
    {
        printf("Enter the name of the record you want to search.Enter either complete name, partial name followed by (*), of simply (*) to list all records\n");
        printf("Enter: ");
        fflush(stdin);
        scanf("%[^\n]s", name);
        strcpy(name, strupr(name));
        if(name[0] == '*')
        {
            for(i=65; i<=90; i++)
            {
                fp = open_File((char)i);
                start = create_List(fp, start);
                fclose(fp);
                if(start == NULL)
                {
                    continue;
                }
                print(start);
                start = empty_List(start);
            }
            return;
        }
        fp = open_File(name[0]);
        start = create_List(fp, start);
        p = start;
        l = strlen(name);
        if(name[l-1] == '*')
        {
            printf(" ____________________________________________________________________ \n");
            printf("|=======================PHONEBOOK RECORD LIST========================|\n");
            printf("|____________________________________________________________________|\n");
            printf("|          Name           |      Number        |       Address       |\n");
            printf("|_________________________|____________________|_____________________|\n");
            while(p != NULL)
            {
                if(strncmp(p->name, name, l-1) == 0)
                {
                    printf("\n");
                    printf("\t%s\t", p->name);
                    printf("\t%lld\t", p->number);
                    printf("\t%s\t\n", p->address);
                    printf("|_________________________|____________________|_____________________|\n");
                }
                p = p->link;
            }
            fclose(fp);
            return;
        }
        printf(" ____________________________________________________________________ \n");
        printf("|=======================PHONEBOOK RECORD LIST========================|\n");
        printf("|____________________________________________________________________|\n");
        printf("|          Name           |      Number        |       Address       |\n");
        printf("|_________________________|____________________|_____________________|\n");
        while(p != NULL)
        {
            if(strcmp(p->name, name) == 0)
            {
                printf("\n");
                printf("\t%s\t", p->name);
                printf("\t%lld\t", p->number);
                printf("\t%s\t\n", p->address);
                printf("|_________________________|____________________|_____________________|\n");
            }
            p = p->link;
        }
        fclose(fp);
        return;
    }
    else if(n == 2)
    {
        printf("Enter the number of the contact you want to search: ");
        scanf("%lld", &number);
        for(i=65; i<=90; i++)
        {
            fp = open_File((char)i);
            start = create_List(fp, start);
            fclose(fp);
            p = start;

            while(p != NULL)
            {
                if(p->number == number)
                {
                    printf(" ____________________________________________________________________ \n");
                    printf("|=======================PHONEBOOK RECORD LIST========================|\n");
                    printf("|____________________________________________________________________|\n");
                    printf("|          Name           |      Number        |       Address       |\n");
                    printf("|_________________________|____________________|_____________________|\n");
                    flag = 1;
                    printf("\n");
                    printf("\t%s\t", p->name);
                    printf("\t%lld\t", p->number);
                    printf("\t%s\t\n", p->address);
                    printf("|_________________________|____________________|_____________________|\n");
                }
                p = p->link;
            }
            start = empty_List(start);
        }
        if(flag == 0)
        {
            printf("\nContact not found in the directory\n");
        }
        return;
    }
    else
    {
        printf("Invalid choice\n");
        return;
    }
}
void modify_Record()
{
    FILE *fp;
    char c[2];
    RECORD contact;
    RECLINK *start = NULL;
    printf("Enter details of the contact to be modified:\n");
    printf("Enter the name of contact: ");
    scanf("%[^\n]s", contact.name);
    fflush(stdin);
    printf("Enter number of contact: ");
    scanf("%lld", &(contact.number));
    fflush(stdin);
    printf("Enter address of contact: ");
    scanf("%[^\n]s", contact.address);
    fflush(stdin);
    strcpy(contact.name, strupr(contact.name));
    strcpy(contact.address, strupr(contact.address));
    fp = open_File(contact.name[0]);
    if(fp == NULL)
    {
        return;
    }
    start = create_List(fp, start);
    start = modify_List(start, contact);
    fclose(fp);
    c[0] = contact.name[0];
    c[1] = '\0';
    fopen(c, "wb+");
    //fseek(fp, 0L, 0);
    fp = update_File(fp, start);
    fclose(fp);
    return;
}
RECLINK *modify_List(RECLINK *start, RECORD contact)
{
    RECLINK *p = start;
    RECLINK *temp;
    char ch;
    if(start == NULL)
    {
        printf("Contact not found in the directory\n");
        printf("Do you want to create a contact with new details?\nEnter 'y' for YES 'n' for NO: ");
        ch = getch();
        if(ch == 'y')
        {
            printf("\nEnter the details for new contact:\n");
            printf("Enter the name of contact: ");
            scanf("%[^\n]s", contact.name);
            fflush(stdin);
            printf("Enter number of contact: ");
            scanf("%lld", &(contact.number));
            fflush(stdin);
            printf("Enter address of contact: ");
            scanf("%[^\n]s", contact.address);
            fflush(stdin);
            strcpy(contact.name, strupr(contact.name));
            strcpy(contact.address, strupr(contact.address));
            start = insert_List(start, contact);
            return start;
        }
        printf("\nContact not added to directory\n");
        return start;
    }
    if((strcmp(start->name, contact.name) == 0) && (start->number == contact.number))
    {
        temp = start;
        start = start->link;
        free(temp);
        printf("Enter the details of the modified contact:\n");
        printf("Enter the name of contact: ");
        scanf("%[^\n]s", contact.name);
        fflush(stdin);
        printf("Enter number of contact: ");
        scanf("%lld", &(contact.number));
        fflush(stdin);
        printf("Enter address of contact: ");
        scanf("%[^\n]s", contact.address);
        fflush(stdin);
        strcpy(contact.name, strupr(contact.name));
        strcpy(contact.address, strupr(contact.address));
        start = insert_List(start, contact);
        printf("\nContact successfully modified\n");
        return start;
    }
    while(p->link != NULL)
    {
        if((strcmp((p->link)->name, contact.name) == 0) && ((p->link)->number == contact.number))
        {
            temp = p->link;
            p->link = temp->link;
            free(temp);
            printf("Enter the details of the modified contact:\n");
            printf("Enter the name of contact: ");
            scanf("%[^\n]s", contact.name);
            fflush(stdin);
            printf("Enter number of contact: ");
            scanf("%lld", &(contact.number));
            fflush(stdin);
            printf("Enter address of contact: ");
            scanf("%[^\n]s", contact.address);
            fflush(stdin);
            strcpy(contact.name, strupr(contact.name));
            strcpy(contact.address, strupr(contact.address));
            start = insert_List(start, contact);
            printf("\nContact successfully modified\n");
            return start;
        }
        p = p->link;
    }
    printf("Contact not found in the directory\n");
    printf("Do you want to create a contact with new details?\nEnter 'y' for YES 'n' for NO: ");
    ch = getch();
    if(ch == 'y')
    {
        printf("\nEnter the details for new contact:\n");
        printf("Enter the name of contact: ");
        scanf("%[^\n]s", contact.name);
        fflush(stdin);
        printf("Enter number of contact: ");
        scanf("%lld", &(contact.number));
        fflush(stdin);
        printf("Enter address of contact: ");
        scanf("%[^\n]s", contact.address);
        fflush(stdin);
        strcpy(contact.name, strupr(contact.name));
        strcpy(contact.address, strupr(contact.address));
        start = insert_List(start, contact);
        return start;
    }
    printf("\nContact not added to directory\n");
    return start;
}
void delete_Record()
{
    FILE *fp;
    char c[2];
    RECORD contact;
    RECLINK *start = NULL;
    printf("Enter the name of contact: ");
    scanf("%[^\n]s", contact.name);
    fflush(stdin);
    printf("Enter number of contact: ");
    scanf("%lld", &(contact.number));
    fflush(stdin);
    printf("Enter address of contact: ");
    scanf("%[^\n]s", contact.address);
    fflush(stdin);
    strcpy(contact.name, strupr(contact.name));
    strcpy(contact.address, strupr(contact.address));
    fp = open_File(contact.name[0]);
    if(fp == NULL)
    {
        return;
    }
    start = create_List(fp, start);
    start = delete_List(start, contact);
    fclose(fp);
    c[0] = contact.name[0];
    c[1] = '\0';
    fopen(c, "wb+");
    //fseek(fp, 0L, 0);
    fp = update_File(fp, start);
    fclose(fp);
    return;

}
RECLINK *delete_List(RECLINK *start, RECORD contact)
{
    RECLINK *temp;
    char ch;
    RECLINK *p = start;
    if(start == NULL)
    {
        printf("Contact not found in the directory\n");
        printf("Do you want to create a new record with the given details?\nEnter 'y' for YES 'n' for NO: ");
        ch = getch();
        if(ch == 'y')
        {
            start = insert_List(start, contact);
            return start;
        }
        printf("\nContact not added to directory\n");
        return start;
    }
    if((strcmp(start->name, contact.name) == 0) && (start->number == contact.number))
    {
        temp = start;
        start = start->link;
        free(temp);
        printf("Contact successfully deleted from directory\n");
        return start;
    }
    while(p->link != NULL)
    {
        if((strcmp((p->link)->name, contact.name) == 0) && ((p->link)->number == contact.number))
        {
            temp = p->link;
            p->link = temp->link;
            free(temp);
            printf("Contact successfully deleted from directory\n");
            return start;
        }
        p = p->link;
    }
    printf("Contact not found in the directory\n");
    printf("Do you want to create a new record with the given details?\nEnter 'y' for YES 'n' for NO: ");
    ch = getch();
    if(ch == 'y')
    {
        start = insert_List(start, contact);
        return start;
    }
    printf("\nContact not added to directory\n");
    return start;
}
void create_Record(void)
{
    FILE *fp;
    RECORD contact;
    RECLINK *start = NULL;
    //fflush(stdin);
    printf("Enter the name of contact: ");
    scanf("%[^\n]s", contact.name);
    fflush(stdin);
    printf("Enter number of contact: ");
    scanf("%lld", &(contact.number));
    fflush(stdin);
    printf("Enter address of contact: ");
    scanf("%[^\n]s", contact.address);
    fflush(stdin);
    strcpy(contact.name, strupr(contact.name));
    strcpy(contact.address, strupr(contact.address));
    fp = open_File(contact.name[0]);
    if(fp == NULL)
    {
        return;
    }
    start = create_List(fp, start);
    start = insert_List(start, contact);
    fseek(fp, 0L, 0);
    fp = update_File(fp, start);
    fclose(fp);
    return;
}
RECLINK *create_List(FILE *fp, RECLINK *start)
{
    RECORD contact;
    RECLINK *temp;
    RECLINK *p;
    while(fread(&contact, sizeof(RECORD), 1, fp) == 1)
    {
        temp = (RECLINK *)malloc(sizeof(RECLINK));
        strcpy(temp->name, contact.name);
        strcpy(temp->address, contact.address);
        temp->number = contact.number;
        //start = insert_List(start, contact);
        if(start == NULL)
        {
            temp->link = NULL;
            start = temp;
            continue;
            //p = start;
        }
        p = start;
        while(p->link != NULL)
        {
            p = p->link;
        }
        temp->link = p->link;
        p->link = temp;
        //p = p->link;
    }
    return start;
}
RECLINK *insert_List(RECLINK *start, RECORD contact)
{
    RECLINK *temp;
    RECLINK *p = start;
    temp = (RECLINK *)malloc(sizeof(RECLINK));
    strcpy(temp->name, contact.name);
    strcpy(temp->address, contact.address);
    temp->number = contact.number;
    if(start == NULL)
    {
        temp->link = NULL;
        start = temp;
        printf("\nNew contact added successfully\n");
        return start;
    }
    if(strcmp(start->name, contact.name) == 0)
    {
        if(start->number == contact.number)
        {
            printf("Contact record already present\n");
            return start;
        }
        temp->link = start;
        start = temp;
        printf("\nNew contact added successfully\n");
        return start;
    }
    if(strcmp(start->name, contact.name) > 0)
    {
        temp->link = start;
        start = temp;
        printf("\nNew contact added successfully\n");
        return start;
    }
    while(p->link != NULL)
    {
        if(strcmp((p->link)->name, contact.name) < 0)
        {
            p = p->link;
        }
        else
        {
            break;
        }
    }
    if(p->link == NULL)
    {
        temp->link = NULL;
        p->link = temp;
        printf("\nNew contact added successfully\n");
        return start;
    }
    if(strcmp((p->link)->name, contact.name) == 0)
    {
        if((p->link)->number == contact.number)
        {
            printf("Contact record already present\n");
            return start;
        }
        temp->link = p->link;
        p->link = temp;
        printf("\nNew contact added successfully\n");
        return start;
    }
    temp->link = p->link;
    p->link = temp;
    printf("\nNew contact added successfully\n");
    return start;
}
FILE *update_File(FILE *fp, RECLINK *start)
{
    RECLINK *temp;
    RECORD contact;
    while(start != NULL)
    {
        strcpy(contact.name, start->name);
        strcpy(contact.address, start->address);
        contact.number = start->number;
        fwrite(&contact, sizeof(RECORD), 1, fp);
        temp = start;
        start = start->link;
        free(temp);
    }
    return fp;
}
FILE *open_File(char c)
{
    FILE *fp;
    switch(c)
    {
    case 'A':
        {
            fp = fopen("A", "rb+");
            if(fp == NULL)
            {
                fp = fopen("A", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'B':
        {
            fp = fopen("B", "rb+");
            if(fp == NULL)
            {
                fp = fopen("B", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'C':
        {
            fp = fopen("C", "rb+");
            if(fp == NULL)
            {
                fp = fopen("C", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'D':
        {
            fp = fopen("D", "rb+");
            if(fp == NULL)
            {
                fp = fopen("D", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'E':
        {
            fp = fopen("E", "rb+");
            if(fp == NULL)
            {
                fp = fopen("E", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'F':
        {
            fp = fopen("F", "rb+");
            if(fp == NULL)
            {
                fp = fopen("F", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'G':
        {
            fp = fopen("G", "rb+");
            if(fp == NULL)
            {
                fp = fopen("G", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'H':
        {
            fp = fopen("H", "rb+");
            if(fp == NULL)
            {
                fp = fopen("H", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'I':
        {
            fp = fopen("I", "rb+");
            if(fp == NULL)
            {
                fp = fopen("I", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'J':
        {
            fp = fopen("J", "rb+");
            if(fp == NULL)
            {
                fp = fopen("J", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'K':
        {
            fp = fopen("K", "rb+");
            if(fp == NULL)
            {
                fp = fopen("K", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'L':
        {
            fp = fopen("L", "rb+");
            if(fp == NULL)
            {
                fp = fopen("L", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'M':
        {
            fp = fopen("M", "rb+");
            if(fp == NULL)
            {
                fp = fopen("M", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'N':
        {
            fp = fopen("N", "rb+");
            if(fp == NULL)
            {
                fp = fopen("N", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'O':
        {
            fp = fopen("O", "rb+");
            if(fp == NULL)
            {
                fp = fopen("O", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'P':
        {
            fp = fopen("P", "rb+");
            if(fp == NULL)
            {
                fp = fopen("P", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'Q':
        {
            fp = fopen("Q", "rb+");
            if(fp == NULL)
            {
                fp = fopen("Q", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'R':
        {
            fp = fopen("R", "rb+");
            if(fp == NULL)
            {
                fp = fopen("R", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'S':
        {
            fp = fopen("S", "rb+");
            if(fp == NULL)
            {
                fp = fopen("S", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'T':
        {
            fp = fopen("T", "rb+");
            if(fp == NULL)
            {
                fp = fopen("T", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'U':
        {
            fp = fopen("U", "rb+");
            if(fp == NULL)
            {
                fp = fopen("U", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'V':
        {
            fp = fopen("V", "rb+");
            if(fp == NULL)
            {
                fp = fopen("V", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'W':
        {
            fp = fopen("W", "rb+");
            if(fp == NULL)
            {
                fp = fopen("W", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'X':
        {
            fp = fopen("X", "rb+");
            if(fp == NULL)
            {
                fp = fopen("X", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'Y':
        {
            fp = fopen("Y", "rb+");
            if(fp == NULL)
            {
                fp = fopen("Y", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    case 'Z':
        {
            fp = fopen("Z", "rb+");
            if(fp == NULL)
            {
                fp = fopen("Z", "wb+");
                if(fp == NULL)
                {
                    printf("Error in opening the file\n");
                    return NULL;
                }
            }
            break;
        }
    default:
        printf("Error! Invalid contact name\n");
        return NULL;
    }
    return fp;
}

void menu()
{
    system("color E");
    printf(" ________________________________________________ \n");
    printf("|==========PHONEBOOK RECORD MANAGEMENT===========|\n");
    printf("|________________________________________________|\n");
    printf("|\t\t\tMENU\t\t\t |\n");
    printf("|\ta.ADD  \t\tb.Modify  \tc.Delete |\n|\td.Search \te.Exit\t\t\t |\n");
    printf("|________________________________________________|\n");
    printf("Enter your choice: ");
}

