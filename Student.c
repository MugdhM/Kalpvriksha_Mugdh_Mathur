#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NAME_LEN 50
#define SUBJECT_COUNT 3

#define GRADE_A_MIN 85
#define GRADE_B_MIN 70
#define GRADE_C_MIN 50
#define GRADE_D_MIN 35

typedef struct student {
    int rollNumber;
    char name[MAX_NAME_LEN];
    int marks[SUBJECT_COUNT];
    float averageMarks;
    int totalMarks;
    char grade;
} Student;

typedef struct node {
    Student data;
    struct node *next;
} Node;

char assignGrade(float averageMarks) {
    if (averageMarks >= GRADE_A_MIN) return 'A';
    if (averageMarks >= GRADE_B_MIN) return 'B';
    if (averageMarks >= GRADE_C_MIN) return 'C';
    if (averageMarks >= GRADE_D_MIN) return 'D';
    return 'F';
}

Node* addStudent(int rollNumber, char *name, int *marks) {
    struct node *newNode = (struct node*)malloc(sizeof(struct node));  // dynamic memory allocation
    newNode->next = NULL;

    newNode->data.rollNumber = rollNumber;
    strcpy(newNode->data.name, name);

    newNode->data.totalMarks = 0;

    for (int i = 0; i < SUBJECT_COUNT; i++) {
        newNode->data.marks[i] = *(marks + i);
        newNode->data.totalMarks += *(marks + i);
    }

    newNode->data.averageMarks = (float)newNode->data.totalMarks / SUBJECT_COUNT;
    newNode->data.grade = assignGrade(newNode->data.averageMarks);

    return newNode;
}

Node* inputStudent() {
    int rollNumber, marks[SUBJECT_COUNT];
    char inputBuffer[100], name[MAX_NAME_LEN];

    fgets(inputBuffer, 99, stdin);
    sscanf(inputBuffer, "%d %49s %d %d %d", &rollNumber, name, &marks[0], &marks[1], &marks[2]);

    return addStudent(rollNumber, name, marks);
}

Node* insertNode(Node *head, Node *newNode) {
    if (head == NULL) {
        head = newNode;
    } else {
        Node *temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
    return head;
}

Node* inputStudentList(Node *head) {
    int count;
    Node *temp = head;

    scanf("%d", &count);
    getchar(); // clear input buffer

    while (count-- > 0) {
        head = insertNode(head, inputStudent());
    }
    return head;
}

void printStudentList(Node *head) {
    printf("\n\n");
    if (head == NULL) {
        printf("Student list is empty ;w; \n");
        return;
    }

    Node *current = head;
    while (current != NULL) {
        printf("Roll: %d\n", current->data.rollNumber);
        printf("Name: %s\n", current->data.name);
        printf("Total: %d\n", current->data.totalMarks);
        printf("Average: %.2f\n", current->data.averageMarks);
        printf("Grade: %c\n", current->data.grade);

        switch (current->data.grade) {
            case 'A': printf("Performance: *****"); break;
            case 'B': printf("Performance: ****"); break;
            case 'C': printf("Performance: ***"); break;
            case 'D': printf("Performance: **"); break;
            default: break;
        }

        printf("\n\n");
        current = current->next;
    }
}

void printRollNumbersUsingRecursion(Node *head) {
    if (head == NULL) return;
    printf("%d  ", head->data.rollNumber);
    printRollNumbersUsingRecursion(head->next);
}

int main() {
    Node *studentList = NULL;

    studentList = inputStudentList(studentList);

    printStudentList(studentList);

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbersUsingRecursion(studentList);

    return 0;
}
