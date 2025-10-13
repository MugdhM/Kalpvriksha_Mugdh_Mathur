#include<stdio.h>
#include<string.h>
#include<stdlib.h>



typedef struct student {
    int rollNumber;
    char name[50];
    int marks[3];
    float averageMarks;
    int totalMarks;
    char grade;
} Student;

typedef struct node {
    Student data;
    struct node *next;
} Node;

char assignGrade(int averageMarks) {

    if(averageMarks >= 85) return 'A';
    if(averageMarks >= 70) return 'B';
    if(averageMarks >= 50) return 'C';
    if(averageMarks >= 35) return 'D';
    return 'F';
}

Node* addStudent(int rollNumber, char *name, int *marks) {

    struct node *newNode = (struct node*)malloc(sizeof(struct node));            //dynamic memeory allocation for new user
    newNode->next = NULL;

    newNode->data.rollNumber = rollNumber;
    strcpy(newNode->data.name,name);

    newNode->data.totalMarks = 0;

    for(int i = 0; i<3; i++) {
        newNode->data.marks[i] =  *(marks + i);
        newNode->data.totalMarks += *(marks + i);
    }

    newNode->data.averageMarks = (float)newNode->data.totalMarks/3.0;

    newNode->data.grade = assignGrade(newNode->data.averageMarks);

    return newNode;

}

Node* inputStudent() {
    int rollNumber,marks[3];
    char inputBuffer[100],name[50];

    fgets(inputBuffer,99,stdin);

    sscanf(inputBuffer,"%d %49s %d %d %d",&rollNumber,name,&marks[0],&marks[1],&marks[2]);


    return addStudent(rollNumber,name,marks);
}

Node* insertNode(Node *head,Node *newNode) {
    if(head == NULL) {
        head = newNode;
    } else {
        Node *temp = head;
        while(temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    return head;
}

Node* startInput(Node *head) {
    int count;                      //count of student
    Node *temp = head;

    scanf("%d",&count);
    getchar();                      //to clear the input buffer

    while(count-- > 0) {
        head = insertNode(head,inputStudent());   // inpuStudent() function will provide the pointer to the new node with dynamically allocated memory 
                                                // and this pointer will be passed to insert Node to be appended at the end of the linkedList of Nodes                  
    }

    return head;
}

void printStudentList(Node *head) {
    printf("\n\n");
    if(head == NULL) {
        printf("Student list is empty ;w; \n");
        return;
    }
    Node *current = head;
    while(current != NULL) {         //loop will run till no student is left
        printf("Roll: %d\n",current->data.rollNumber);
        printf("Name: %s\n",current->data.name);
        printf("Total: %d\n",current->data.totalMarks);
        printf("Average: %.2f\n",current->data.averageMarks);
        printf("Grade: %c\n",current->data.grade);
        
        switch(current->data.grade) {
            case 'A':printf("Performance: *****");
                    break;
            case 'B':printf("Performance: ****");
                    break;
            case 'C':printf("Performance: ***");
                    break;
            case 'D':printf("Performance: **");
                    break;
            default: 
        }

        printf("\n\n");
        current = current->next;
    }
}

void printRollNumbersUsingRecursion(Node *head) {
    if(head == NULL) {
        return; 
    }

    printf("%d  ",head->data.rollNumber);
    return printRollNumbersUsingRecursion(head->next);
    
}



int main() {
    Node *head = NULL;

    head = startInput(head);

    printStudentList(head);

    printf("List of Roll Numbers (via recursion): ");
    printRollNumbersUsingRecursion(head);

    return 0;
}
