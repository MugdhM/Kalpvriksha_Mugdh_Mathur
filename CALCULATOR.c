#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char opStack[SIZE];
int opTop = -1;

void pushOp(char c) { opStack[++opTop] = c; }
char popOp() { return (opTop == -1) ? '\0' : opStack[opTop--]; }
char topOp() { return (opTop == -1) ? '\0' : opStack[opTop]; }

int precedence(char c) {
    switch (c) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default: return 0;
    }
}

void toPostfix(const char* infix, char* postfix) {
    int j = 0;
    for (int i = 0; infix[i] != '\0'; i++) {
        if (isdigit(infix[i])) {
            while (isdigit(infix[i])) {
                postfix[j++] = infix[i++];
            }
            postfix[j++] = ' ';
            i--; // recheck current
        } 
        else if (infix[i] == '(') {
            pushOp('(');
        }
        else if (infix[i] == ')') {
            while (topOp() != '(') {
                postfix[j++] = popOp();
                postfix[j++] = ' ';
            }
            popOp(); // discard '('
        }
        else { 
            while (precedence(topOp()) >= precedence(infix[i])) {
                postfix[j++] = popOp();
                postfix[j++] = ' ';
            }
            pushOp(infix[i]);
        }
    }
    while (opTop != -1) {
        postfix[j++] = popOp();
        postfix[j++] = ' ';
    }
    postfix[j] = '\0';
}

int evalPostfix(const char* postfix) {
    int numStack[SIZE], nTop = -1;
    for (int i = 0; postfix[i] != '\0'; i++) {
        if (isdigit(postfix[i])) {
            int val = 0;
            while (isdigit(postfix[i])) {
                val = val * 10 + (postfix[i] - '0');
                i++;
            }
            numStack[++nTop] = val;
        }
        else if (strchr("+-*/", postfix[i])) {
            int right = numStack[nTop--];
            int left = numStack[nTop--];
            switch (postfix[i]) {
                case '+': numStack[++nTop] = left + right; break;
                case '-': numStack[++nTop] = left - right; break;
                case '*': numStack[++nTop] = left * right; break;
                case '/': numStack[++nTop] = left / right; break;
            }
        }
    }
    return numStack[nTop];
}

int main() {
    char infix[SIZE], postfix[SIZE*2];
    printf("Enter Infix Expression: ");
    fgets(infix, SIZE, stdin);
    infix[strcspn(infix, "\n")] = '\0'; 

    toPostfix(infix, postfix);
    printf("Postfix Expression: %s\n", postfix);

    int result = evalPostfix(postfix);
    printf("Evaluated Result: %d\n", result);
    return 0;
}