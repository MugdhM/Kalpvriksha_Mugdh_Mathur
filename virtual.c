#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define NAME_LENGTH 51
#define BLOCK_SIZE 8
#define TOTAL_BLOCKS 5000

#define True 1
#define False 0

typedef struct FreeNode {
    int index;
    struct FreeNode *next;
    struct FreeNode *previous;
} FreeNode;

typedef struct FileNode {
    char type;
    char name[NAME_LENGTH];
    int index;
    int totalBlocks;
    
    struct FileNode *next;
    struct FileNode *previous;
    struct FileNode *child;
    struct FileNode *parent;
} FileNode;

char Memory[TOTAL_BLOCKS][BLOCK_SIZE];        //virtual Memory
int allocations = 0;

FreeNode *start = NULL;

void appendFreeBlock(int index) {
    FreeNode *newNode = (FreeNode *) malloc(sizeof(FreeNode));
    newNode->index = index;
    
    if(start == NULL) {
        start = newNode;
        start->next = newNode;
        start->previous = newNode;
        return;
    }
    
    newNode->previous = start->previous;
    newNode->next = start;
    
    start->previous->next = newNode;
    start->previous = newNode;
}

void removeFreeBlock() {
    if(start == NULL) {
        return;
    }
    allocations++;
    if(start->next == start) {
        free(start);
        start = NULL;
        return;
    }
    FreeNode *temp = start;
    
    start->previous->next = start->next;
    start->next->previous = start->previous;
    
    start = start->next;
    
    free(temp);
}

int allocateFreeBlocks(int count) {
    if(count > TOTAL_BLOCKS - allocations) return -1;

    int index = start->index;

    while(count-- > 0) removeFreeBlock();

    return index;
}

void printFreeNodes() {
    FreeNode *temp = start;
    printf("\n");
    while(temp->next != start) {
        printf("%d -- ",temp->index);
        temp = temp->next;
    }
}

void initializeFreeBlocks() {
    for(int i = 0; i < TOTAL_BLOCKS; i++) {
        appendFreeBlock(i);
    }
}

// Doubly Circular List file System

FileNode *cwd = NULL;    //current working Directory
FileNode *rootDirectory = NULL;

FileNode* appendFileNode(FileNode *root,FileNode* newNode) {  //placing of newNode
    
    if(root == cwd) {
        root = newNode;
        root->next = newNode;
        root->previous = newNode;
        
        return root;
    }
    
    newNode->previous = root->previous;
    newNode->next = root;
    
    root->previous->next = newNode;
    root->previous = newNode;
    
    return root;
}
FileNode* findNode(char *name) {

    if(cwd->child == cwd) {
        return NULL;
    }

    FileNode *temp = cwd->child;

    while(temp->next != cwd->child) {
        if(strcmp(temp->name,name) == 0) return temp;
        temp = temp->next;
    }

    if(strcmp(temp->name,name) == 0) return temp;

    return cwd;
}

FileNode* getFileNode(char *name) {

    FileNode *foundNode = findNode(name);

    if(foundNode == NULL) {
        printf("\nDirectory is empty");
        return cwd;
    }

    if(foundNode == cwd) {
        printf("\n%s not found",name);
        return cwd;
    }

    return foundNode;
}

void insertChild(char *name,char type) {

    if(cwd != NULL) {
        if(cwd->child != cwd) {
            if(findNode(name) != cwd) {
                printf("\n%s already exists",name);
                return;
            }
        }
    }

    FileNode *newNode = (FileNode *)malloc(sizeof(FileNode));
    

    strcpy(newNode->name,name);
    newNode->type = type;
    newNode->index = -1;
    newNode->totalBlocks = 0;

    if(cwd == NULL) { //case : root
        cwd = newNode;
    } else cwd->child = appendFileNode(cwd->child,newNode);
    

    newNode->parent = cwd;
    newNode->child = newNode;
}



FileNode* removeFileNode(FileNode *root,FileNode *temp) {
    if(temp == root) {
        free(temp);
        return cwd;
    }
    temp->previous->next = temp->next;
    temp->next->previous = temp->previous;

    free(temp);

    return root;
}



void removeChild(FileNode *badChild) {
    cwd->child = removeFileNode(cwd->child,badChild);
}

void clearFile(FileNode *temp) {
    for(int i = 0; i < temp->totalBlocks; i++) {
        appendFreeBlock(temp->index + i);
        allocations--;
    }
}

void removeFile(char *name) {
    FileNode *temp = getFileNode(name);
    
    if(temp != cwd) {
        if(temp->type != 'f') {
            printf("%s is not a File",name);
        } else {
            clearFile(temp);
            removeChild(temp);
            printf("removed %s",name);
        }
    }
}

void removeDirectory(char *name) {
    FileNode *temp = getFileNode(name);

    if(temp != cwd) {
        if(temp->type != 'd') {
            printf("\n%s is not a Directory",name);
        } else if(temp->child != temp) {
            printf("\n %s Directory is not Empty",name);
        } else {
            removeChild(temp);
            printf("removed %s",name);
        }
    }
}

void writeFile(char *name,char *string) {
    FileNode *file = getFileNode(name);

    if(file != cwd) {

        if(file->type != 'f') {
            printf("%s is not a file",name);
            return;
        }

        clearFile(file);
        int length = strlen(string);

        file->totalBlocks = length/BLOCK_SIZE;

        file->index = allocateFreeBlocks(file->totalBlocks);

        if(file->index != -1) {
            int index = 0;
            for(int i = file->index;i<TOTAL_BLOCKS; i++) {
                for(int j = 0; j<BLOCK_SIZE; j++) {

                    Memory[i][j] = string[index];

                    if(string[index] == '\0') return;
                    index++;
                }   
            }
        } else {
            printf("memory overflow, cannot write in file");
            file->totalBlocks = 0;
        }
    }
}

void readFile(char *name) {
    FileNode *file = getFileNode(name);

    if(file != cwd) {

        if(file->type != 'f') {
            printf("%s is not a file",name);
            return;
        }

        printf("\n%s",Memory[file->index]);
    }
}

void changeDirectory(char *name) {
    if(!strcmp(name,"..")) {
        cwd = cwd->parent;
        return;
    }

    FileNode *directory = getFileNode(name);

    if(directory != cwd) {
        if(directory->type != 'd') {
            printf("%s is not a directory",name);
        } else {
            cwd = directory;
        }
    }
}

void listDirectory() {
    if(cwd->child == cwd) {
        printf("(empty)");
    } else  {
        FileNode *iterator = cwd->child;
        
        printf("\n ");
        while(iterator->next != cwd->child) {
            printf("%s  ",iterator->name);
            iterator = iterator->next;
        }
        printf("%s\n",iterator->name);
    } 
}


void CurrentWorkingDirectory(FileNode *current) {
    if(current == rootDirectory) {
        printf("/");
        return;
    }
    CurrentWorkingDirectory(current->parent);
    printf("%s/",current->name);
    return;
}

void presentWorkingDirectory() {
    printf("\n");
    CurrentWorkingDirectory(cwd);
}

void clearInput() {
    int something;
    while((something = getchar()) != '\n' && something != EOF);
}

char* getName() {
    char *name = (char *) malloc(NAME_LENGTH + 10 * sizeof(char));

    scanf("%60s",name);
    
    if(strlen(name) > 50 ) {
        printf("error : name length cannot exceed 50");
        clearInput();
        free(name);
        return NULL;
    }

    return name;
}

void moveDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        changeDirectory(directory);
        free(directory);
    }
}

void makeDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        insertChild(directory,'d');
        free(directory);
    }
}

void deleteDirectory() {
    char *directory = getName();

    if(directory != NULL) {
        removeDirectory(directory);
        free(directory);
    } 
}

void createFile() {
    char *name = getName();

    if(name != NULL) {
        insertChild(name,'f');
        free(name);
    }
} 

void deleteFile() {
    char *name = getName();

    if(name != NULL)  {
        removeFile(name);
        free(name);
    }
}

void writeInFile() {
    char *name = getName();

    if(name != NULL) {
        char string[512];
        fgets(string,511,stdin);

        writeFile(name,string);
        free(name);
    }
}

void readFromFile() {
    char *name = getName();
    
    if(name != NULL) {
        readFile(name);
        free(name);
    }
}

void diskInfo() {
    printf("\nTotal Blocks: %d",TOTAL_BLOCKS);
    printf("\nUsed Blocks: %d",allocations);
    printf("\nFree Blocks: %d",TOTAL_BLOCKS - allocations);
    printf("\nDisk Usage: %.2f%c",(float)(((float)allocations/(float)TOTAL_BLOCKS)*100),'%');
}

void execute(char *command) {
    if(strcmp(command,"ls") == 0) listDirectory();
    else if(strcmp(command,"pwd") == 0) presentWorkingDirectory();
    else if(strcmp(command,"cd") == 0) moveDirectory();
    else if(strcmp(command,"mkdir") == 0) makeDirectory();
    else if(strcmp(command,"rmdir") == 0) deleteDirectory();
    else if(strcmp(command,"create") == 0) createFile();
    else if(strcmp(command,"delete") == 0) deleteFile();
    else if(strcmp(command,"df") == 0) diskInfo();
    else if(strcmp(command,"write") == 0) writeInFile();
    else if(strcmp(command,"read") == 0) readFromFile();
    else printf("%s is not a valid command",command);
}

void getInput() {
    while(True) {
        char command[20];
        printf("\n@-@ %s >",cwd->name);
        scanf("%s",command);

        if(strlen(command) > 9) {
            printf("\n%s is not a valid command");
            clearInput();
        } else if(strcmp(command,"exit") == 0) {
            return;
        } else {
            execute(command);
        }
    }
}

int main() {
    insertChild("/",'d');
    rootDirectory = cwd;

    initializeFreeBlocks();

    getInput();

    return 0;

}
