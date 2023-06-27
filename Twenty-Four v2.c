/*
============================================================================
 Name/Description   : Twenty-Four Game evaluated using Stack substitutes
 Date      : Spring 2023
 System      : Replit
 Author      : Luke Hartzell
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//creation of node format using struct
typedef struct Node *NodePtr;
struct Node {
    int data;
    NodePtr pNext;
};

///*Stack Operations*///
//check if the list is empty
bool isEmpty(NodePtr head) {
    return head == NULL;
}

/*creates a new node and prepends the node with the passed integer value to the linked list passed by reference through its head*/
void push(NodePtr *head, int value) {
    NodePtr newNode = (NodePtr) malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->pNext = (*head);
    (*head) = newNode;
}

/*moves the head pointer to pointn tothe second element in the passed-by-reference list, pops off the first item and returns the value that was in that popped node*/
int pop(NodePtr *head) {
    if (isEmpty(*head)) { //if list is empty
        return -1;
    } else {
        int poppedValue;
        NodePtr tempNode = *head;
        (*head) = (*head)->pNext;
        poppedValue = tempNode->data;
        free(tempNode);
        return poppedValue;
    }
}

/*frees a linked list*/
void freeList(NodePtr head) {
    NodePtr current = head;
    NodePtr next;

    while (current != NULL) {
        next = current->pNext;
        free(current);
        current = next;
    }
    free(head);
}

/*frees a linked list but does not free the head pointer*/
void freeListExceptHead(NodePtr head) {
    NodePtr current = head;
    NodePtr next;

    while (current != NULL) {
        next = current->pNext;
        free(current);
        current = next;
    }
}

/*returns the value of the most recent node in the passed list, to be used for comparison before an element is added to a list*/
int peek(NodePtr head) {
    if (isEmpty(head)) { //if list is empty
        return -1;
    } else {
        return head->data;
    }
}

///*General Functions*///
/*reads in a puzzle file, depending on the passed difficultyLevel and stores it in puzzleList*/
int getPuzzles(char puzzleList[50][10], char *difficultyLevel) {
    int totalNums = 0; //will be used to store the total amount of numbers in the array of puzzles
    int index = 0; //will be used to keep track of the index of each number so that the 2D array can be filled
    char *filename = difficultyLevel;
    FILE *filePtr = fopen(filename, "r"); // "r" means we open the file for reading

    // Check that the file was able to be opened
    if (filePtr == NULL) {
        printf("Error: could not open %s for reading\n", filename);
        exit(-1);
    }

    //Read each word from file, and print them one by one
    while (1) { //
        char temp;

        temp = fgetc(filePtr); // reading the file
        if (temp == EOF) {
            break;
        }
        //reads in only the numbers and stores them in puzzleList
        if (temp != ' ') {
            if (temp != '\n') {
                puzzleList[totalNums / 4][index] = temp;; //after 4 numbers read in, first index increases by 1
                totalNums += 1;
                index++;
                if (index == 4) { //resets second index once 4 numbers are read in
                    index = 0;
                }
            }
        }
    }
    // Close the file
    fclose(filePtr);
    return 0;
}

/*returns a string with the filename of a difficulty level depending on the passed character; also changes the difficultyNumber for easy use of the difficulty level in other functions, difficultyNumber is passed by reference*/
char *getDifficultyFileSetDifficulty(char difficultyInput, int *difficulterNumber) {
    if (difficultyInput == 'E') {
        *difficulterNumber = 1; //sets a difficulty num for easier use in other functions
        return "easy.txt";
    } else if (difficultyInput == 'M') {
        *difficulterNumber = 2;
        return "medium.txt";
    } else if (difficultyInput == 'H') {
        *difficulterNumber = 3;
        return "hard.txt";
    } else { //defaults to easy if none of the other choices are chosen
        *difficulterNumber = 1;
        return "easy.txt";
    }
}

/*chooses a random puzzle from puzzleSet based on a passed numberForDifficulty and stores this puzzle into randomPuzzle)*/
int chooseAndDisplayPuzzle(int numberForDifficulty, char puzzleSet[50][10], char randomPuzzle[4]) {
    int randomIndex;

    //get the random index depending on the amount of puzzles in each difficulty file
    if (numberForDifficulty == 1) {
        randomIndex = rand() % 12;
    } else if (numberForDifficulty == 2) {
        randomIndex = rand() % 24;
    } else if (numberForDifficulty == 3) {
        randomIndex = rand() % 11;
    }

    /*print all of the numbers inside of the random puzzle*/
    printf("The numbers to use are: %c, %c, %c, %c.\n", puzzleSet[randomIndex][0], puzzleSet[randomIndex][1],
           puzzleSet[randomIndex][2], puzzleSet[randomIndex][3]);
    for (int i = 0; i < 4; i++) { //stores the randomly selected puzzle into randomPuzzle
        randomPuzzle[i] = puzzleSet[randomIndex][i];
    }
    return 0;
}

/*removes all spaces in the passed string; directly alters the string, does not return anything*/
void removeSpaces(char *str) {
    int index = 0;
    int length = strlen(str);

    //changes the elements of the string to be replaced with all of its non-space characters, starting from the beginning
    for (int i = 0; i < length; i++) {
        if (str[i] != ' ') {
            str[index] = str[i];
            index++;
        }
    }
    //clears the rest of the string after all of the non-space characters
    for (int j = index; j < length; j++) {
        str[index] = '\0';
    }
}

/*prints the calculation step*/
void printCalculation(int firstNum, char op, int secondNum, int result) {
    printf("%d %c %d = %d.\n", firstNum, op, secondNum, result);
}

/*returns boolean based on if all of the characters within the passed string are valid (0-9, operators, and parenthesis)*/
bool checkValidChars(char *input) {
    int length = strlen(input);
    bool valid = true;
    //check for valid characters
    for (int i = 0; i < length; i++) {
        //uses ASCII values of numbers and the allowed symbols to check for valid characters
        if (!(input[i] >= '0' && input[i] <= '9' || input[i] > 39 && input[i] < 44 || input[i] == 45 ||
              input[i] == 47 || input[i] == '\0')) { //if not valid
            valid = false;
            break;
        }
    }
    return valid;
}

/*returns boolean based on if all of the numbers in a "puzzle" are used exactly once in a passed string*/
bool checkNumUse(char puzzle[4], char *input) {
    int index = 0;
    char inputNums[strlen(input)];
    strcpy(inputNums, input);
    int length = strlen(inputNums);
    bool used[4] = {false, false, false, false}; //an array to keep track of which elements have been used

    for (int i = 0; i < length; i++) {
        if (inputNums[i] >= '0' && inputNums[i] <= '9') { //if it is a digit
            inputNums[index] = inputNums[i]; //store the digit at the beginning of the array
            index++;
        }
    }
    inputNums[index] = '\0'; //"cuts off" the rest of the string and excess elements

    length = strlen(inputNums);
    if (length != 4) { //if there are not exactly 4 digits in the input
        return false;
    }
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < 4; j++) {
            //if a character of puzzle is found in inputNums and has not been accounted for yet
            if (puzzle[i] == inputNums[j] && !used[j]) {
                used[j] = true;
                break;
            }
        }
    }
    for (int i = 0; i < 4; i++) {
        if (!used[i]) { //if there are any elements that have not been used
            return false;
        }
    }
    return true; //if the code reaches here, then all elements in puzzle have been used exactly once
}

/*evaluates the passed expression string and returns that value that it was evaluated as*/
int evaluateExpression(char *expression) {
    NodePtr numbers = NULL;
    NodePtr operators = NULL;
    int length = strlen(expression);
    int openParen = 0;
    int closeParen = 0;
    int nums = 0;
    int ops = 0;

    for (int index = 0; index < length; index++) {
        if (expression[index] == '(') {
            push(&operators, expression[index]); //push the open paren then continue with evaluation
            openParen++;
            ops++;
        } else if (expression[index] == ')') {
            //keep solving until the left paren is reached and there are operators in the stack
            while (peek(operators) != '(' && peek(operators) != -1) {
                int operand2 = pop(&numbers);
                int operand1 = pop(&numbers);
                char op = pop(&operators);
                int result;
                nums -= 2;
                ops--;
                switch (op) {
                    case '*':
                        result = operand1 * operand2;
                        break;
                    case '/':
                        result = operand1 / operand2;
                        break;
                    case '+':
                        result = operand1 + operand2;
                        break;
                    case '-':
                        result = operand1 - operand2;
                        break;
                }
                printCalculation(operand1, op, operand2, result);
                push(&numbers, result);
                nums++;
            }
            if (closeParen++ >= openParen) { //if there are more closing parenthesis that opening parenthesis
                printf("Error! Too many closing parentheses in the expression.\n\n");
                freeList(operators);
                freeListExceptHead(numbers);
                return -1;
            }
            pop(&operators); // Remove the '(' from the stack
            ops--;
        } else if (expression[index] == '*' || expression[index] == '/') { //if the next character is '*' or '/'
            if (peek(operators) == '*' || peek(operators) == '/') { //and the previous is also either '*' or '/'
                int operand2 = pop(&numbers);
                int operand1 = pop(&numbers);
                char op = pop(&operators);
                int result;
                nums -= 2;
                ops++;
                switch (op) { //perform the calculation with the operator and numbers already in the stack
                    case '*':
                        result = operand1 * operand2;
                        break;
                    case '/':
                        result = operand1 / operand2;
                        break;
                }
                printCalculation(operand1, op, operand2, result);
                push(&numbers, result);
                nums++;
            }
            push(&operators, expression[index]);
            ops++;
        } else if (expression[index] == '+' || expression[index] == '-') { //if the next character is '+' or '-'
            while (!isEmpty(operators) &&
                   peek(operators) != '(') { //and the operator stack is not empty or an opening paren
                int operand2 = pop(&numbers);
                int operand1 = pop(&numbers);
                char op = pop(&operators);
                int result;
                nums -= 2;
                ops--;
                switch (op) { //perform the calculation based on the numbers and operator already in the stack
                    case '*':
                        result = operand1 * operand2;
                        break;
                    case '/':
                        result = operand1 / operand2;
                        break;
                    case '+':
                        result = operand1 + operand2;
                        break;
                    case '-':
                        result = operand1 - operand2;
                        break;
                }
                printCalculation(operand1, op, operand2, result);
                push(&numbers, result);
                nums++;
            }
            push(&operators, expression[index]);
            ops++;
        } else {
            if (expression[index] >= '0' && expression[index] <= '9') { //if the character is a digit
                push(&numbers, atoi(&expression[index]));
                nums++;
            }
        }
        if (nums >= (ops + 2)) { //if there are 2 or more numbers in comparison to the operators, there are too many values
            printf("Error! Too many values in the expression.\n\n");
            // freeList(numbers);
            freeListExceptHead(numbers);
            free(operators);
            return -1;
        }
    }
    while (!isEmpty(operators)) { //completes any extraneous calculations that are left over until there are no more operators
        int operand2 = pop(&numbers);
        int operand1 = pop(&numbers);
        char op = pop(&operators);
        int result;
        nums -= 2;
        ops--;
        switch (op) {
            case '*':
                result = operand1 * operand2;
                break;
            case '/':
                result = operand1 / operand2;
                break;
            case '+':
                result = operand1 + operand2;
                break;
            case '-':
                result = operand1 - operand2;
                break;
        }
        printCalculation(operand1, op, operand2, result);
        push(&numbers, result);
        nums++;
    }
    int total = pop(&numbers);
    freeList(operators); //free the linked lists before return
    freeList(numbers);
    return total;
}

/*actual game part: chooses random puzzle until valid input, asks for expression
 then evaluates the expression, then prints the appropriate end message; no meaningful return value*/
int game(int difficultyInteger, char puzzleChars[50][10], char chosenPuzzle[4]) {
    int total = 0;
    bool validInput = false;
    bool validChars = false;
    bool validNumUse = false;
    char inputtedExpression[100];

    while (!validInput) { //loops until validInput (valid chars and valid number usage), then can continue with game
        chooseAndDisplayPuzzle(difficultyInteger, puzzleChars, chosenPuzzle);
        printf("Enter your solution: ");
        fgets(inputtedExpression, 100, stdin); //scan in the size of inputtedExpression
        inputtedExpression[strcspn(inputtedExpression, "\n")] = '\0'; //removes new line char
        removeSpaces(inputtedExpression);
        validChars = checkValidChars(inputtedExpression);
        if (!validChars) {
            printf("Error! Invalid symbol entered. Please try again.\n\n");
            continue;
        }
        validNumUse = checkNumUse(chosenPuzzle, inputtedExpression);
        if (!validNumUse) {
            printf("Error! You must use all four numbers, and use each one only once. Please try again. \n\n");
            continue;
        }
        validInput = validChars && validNumUse;
    }
    total = evaluateExpression(inputtedExpression); //evaluates expression
    if (total == -1) { //if the expression could not successfully be evaluated
        return 0;
    } else if (total == 24) {
        printf("Well done! You are a math genius.\n\n");
    } else {
        printf("Sorry. Your solution did not evaluate to 24.\n\n");
    }
    return 0;
}

//=============================================================================================================
int main() {
    char puzzleChars[50][10];
    char chosenPuzzle[4];
    char inputtedDifficulty;
    char inputtedExpression[100];
    int difficultyInteger = 0;
    srand(1);
    char garbage;
    int menuOption = 0;

    /*initialize all the array values to null character*/
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 10; j++) {
            puzzleChars[i][j] = '\0';
        }
    }

    /*introduction messages and prompts*/
    printf("Welcome to the game of TwentyFour Part Two!\n");
    printf("Use each of the four numbers shown exactly once, \n");
    printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
    printf("to yield the value twenty-four.\n");
    printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
    printf("Your choice --> ");
    scanf("%c", &inputtedDifficulty);
    scanf("%c", &garbage); //eats up the extra blank space
  
    //get initial set of puzzles - difficulty determined by user input
    getPuzzles(puzzleChars, getDifficultyFileSetDifficulty(inputtedDifficulty, &difficultyInteger));
    //user plays the actual game where they are trying to make an expression that evaluates to 24
    game(difficultyInteger, puzzleChars, chosenPuzzle);

    /*user is prompted with choices once the game has completed - play again, play again with a different difficulty, exit           program*/
    while (menuOption != 3) { //while the user has not chosen to exit the program
        int clearSpace;

        printf("Enter: \t1 to play again,\n");
        printf("\t\t2 to change the difficulty level and then play again, or\n");
        printf("\t\t3 to exit the program.\n");
        printf("Your choice --> ");
        scanf(" %d", &menuOption);

        switch (menuOption) {
            case 1: //play again with same difficulty
                while ((clearSpace = getchar()) != '\n' && clearSpace != EOF); //clears any leftover whitespace
                game(difficultyInteger, puzzleChars, chosenPuzzle);
                break;

            case 2: //play again but change difficulty, reprompts for difficulty
                while ((clearSpace = getchar()) != '\n' && clearSpace != EOF); //clears any leftover whitespace
                printf("Choose your difficulty level: E for easy, M for medium, and H for hard (default is easy).\n");
                printf("Your choice --> ");
                scanf("%c", &inputtedDifficulty);
                scanf("%c", &garbage); //eats up the extra blank space

                getPuzzles(puzzleChars, getDifficultyFileSetDifficulty(inputtedDifficulty, &difficultyInteger));
                game(difficultyInteger, puzzleChars, chosenPuzzle);
                break;

            case 3: //exits the program
                printf("\nThanks for playing!\n");
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid menu option. Retry, \n");
                continue;
        }
    }
    return 0;
}

