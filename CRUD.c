#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DB_FILE "users.txt"

typedef struct {
    int uid;
    char uname[40];
    int uage;
} Person;

void addRecord() {
    FILE *fp = fopen(DB_FILE, "a");
    if (!fp) {
        printf("❌ Could not open file.\n");
        return;
    }

    Person p;
    printf("Enter ID: ");
    scanf("%d", &p.uid);
    printf("Enter Name: ");
    scanf("%s", p.uname);
    printf("Enter Age: ");
    scanf("%d", &p.uage);

    fprintf(fp, "%d %s %d\n", p.uid, p.uname, p.uage);
    fclose(fp);
    printf("✅ Record saved!\n");
}

void showRecords() {
    FILE *fp = fopen(DB_FILE, "r");
    if (!fp) {
        printf("⚠ No records found.\n");
        return;
    }

    Person p;
    printf("\n--- User Database ---\n");
    while (fscanf(fp, "%d %s %d", &p.uid, p.uname, &p.uage) == 3) {
        printf("ID: %d | Name: %s | Age: %d\n", p.uid, p.uname, p.uage);
    }
    fclose(fp);
}

void modifyRecord() {
    FILE *fp = fopen(DB_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (!fp || !tmp) {
        printf("❌ File error.\n");
        return;
    }

    int id, found = 0;
    Person p;

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &p.uid, p.uname, &p.uage) == 3) {
        if (p.uid == id) {
            found = 1;
            printf("Enter new Name: ");
            scanf("%s", p.uname);
            printf("Enter new Age: ");
            scanf("%d", &p.uage);
        }
        fprintf(tmp, "%d %s %d\n", p.uid, p.uname, p.uage);
    }

    fclose(fp);
    fclose(tmp);
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    if (found) printf("✅ Record updated!\n");
    else printf("⚠ No record with that ID.\n");
}

// REMOVE record
void removeRecord() {
    FILE *fp = fopen(DB_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    if (!fp || !tmp) {
        printf("❌ File error.\n");
        return;
    }

    int id, found = 0;
    Person p;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d", &p.uid, p.uname, &p.uage) == 3) {
        if (p.uid == id) {
            found = 1;
            continue; // skip writing -> delete
        }
        fprintf(tmp, "%d %s %d\n", p.uid, p.uname, p.uage);
    }

    fclose(fp);
    fclose(tmp);
    remove(DB_FILE);
    rename("temp.txt", DB_FILE);

    if (found) printf("✅ Record deleted!\n");
    else printf("⚠ No record with that ID.\n");
}

int main() {
    int opt;
    do {
        printf("\n=== User Management Menu ===\n");
        printf("1. Add Record\n");
        printf("2. Show Records\n");
        printf("3. Modify Record\n");
        printf("4. Remove Record\n");
        printf("5. Exit\n");
        printf("Choose option: ");
        scanf("%d", &opt);

        switch (opt) {
            case 1: addRecord(); break;
            case 2: showRecords(); break;
            case 3: modifyRecord(); break;
            case 4: removeRecord(); break;
            case 5: printf("👋 Goodbye!\n"); break;
            default: printf("❌ Invalid choice!\n");
        }
    } while (opt != 5);

    return 0;
}