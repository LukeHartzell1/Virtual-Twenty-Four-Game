/*------------------------------------------------------------------------------------
Name: TwentyFour Game 
Date: Early Spring 2023
System: Replit
Author: Luke Hartzell
------------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

bool easyMode = true;
bool debugMode = false;
bool validOps = true;
char operators[1000];
double nums[4];
double checkDebugNums[4];
int checkDebugOps[3];
char yesOrNo = 'N';
char debugOps[] = {'+','-','*','/'};
int debugNums[] = {1,2,3,4,5,6,7,8,9};
int countSols = 0;
bool totalStatus = false;
bool printSteps = true;

struct numCombo {
    int num1, num2, num3, num4;
  };
struct numCombo easyModeCombos[] = {{3,9,4,1},{8,5,8,1},{6,1,5,1},
                               {2,8,7,8},{5,2,9,2},{2,6,8,4},{5,5,4,3},
                                {6,6,2,6},{8,4,2,6},{6,2,8,1}
  };
struct numCombo chosenNumSet; 
/*after finding and printing all solutions, its determined that there are 3188 solutions, so the set of solutions will be this set size*/
struct numCombo solutions[3188]; 
 
int debugAndHardMode(bool debugStatus, bool eMode);
int presentNums(bool eMode);
bool calculateDebugOutput(double numSet[], char operatorSet[]);
bool calculateOutput(bool validOperators, double numSet[], char operatorSet[]);
int playAgain(void);
int scanOps(void);
int play();
int checkOps(char ops[]);
int totalMessage(int totalStatus);
  /*======================================================================================================*/
int main(int argc, char *argv[]) {

  srand(1);// Initialize random number generator.
  
  /*Checks for command line arguments: debug and easy mode where default is easymode on and debug off*/
  if (argc > 1) {
  for(int j = 1; j < argc; j += 2) {
    if((argv[j][1] == 'e') && (atoi(argv[j + 1]) == 0)) { //checks for command e and value 0
      easyMode = false; 
           }
    if((argv[j][1] == 'd') && (atoi(argv[j + 1]) == 1)) { //no need to search for value 0 since default off
      debugMode = true;
          }   
        }
      }
  
  /*Introduction Message*/
  printf("Welcome to the game of TwentyFour. \n");
  printf("Use each of the four numbers shown below exactly once, \n");
  printf("combining them somehow with the basic mathematical operators (+,-,*,/) \n");
  printf("to yield the value twenty-four. \n");
  
  debugAndHardMode(debugMode, easyMode);
    play();
	return 0;
}

/*METHODS*/
/*generates all solutions and either stores them into an array or prints according to what mode its in (given by parameters debugStatus and eMode*/
int debugAndHardMode(bool debugStatus, bool eMode) {
  if(!eMode || debugStatus){
    int counter = 0;
    double checkNums[4];
    char checkDebug[] = {'+','-','*','/'};
    for(int n1 = 1; n1 < 10; n1++){
        for(int n2 = 1; n2 < 10; n2++){
            for(int n3 = 1; n3 < 10; n3++){
                for(int n4 = 1; n4 < 10; n4++){
                    for(int op1 = 0; op1 < 4; op1++){
                        for(int op2 = 0; op2 < 4; op2++){
                            for(int op3 = 0; op3 < 4; op3++){
                              double checkDebugNums[] = {n1,n2,n3,n4};
                              char checkDebugOps[] =   {checkDebug[op1],checkDebug[op2],checkDebug[op3]};
                                if(calculateDebugOutput( checkDebugNums, checkDebugOps)){
                                 if(!eMode) {
                                   solutions[counter].num1 = checkDebugNums[0];
                                   solutions[counter].num2 = checkDebugNums[1];
                                   solutions[counter].num3 = checkDebugNums[2];
                                   solutions[counter].num4 = checkDebugNums[3];
                                    }                                    
                                 if(debugStatus) {
                                  printf("%d. %d%c%d%c%d%c%d \n", counter + 1, (int)checkDebugNums[0],checkDebugOps[0],     
                                  (int)checkDebugNums[1], checkDebugOps[1], (int)checkDebugNums[2], checkDebugOps[2],
                                        (int)checkDebugNums[3]);
                    }
                                counter++;
                  }
                }
              }
            }
          }
        }
      }
    } 
  }
   return 0; 
}


/*method to choose and print the num set depending on if easymode is on or off*/
int presentNums(bool eMode){
if(eMode) {
  chosenNumSet = easyModeCombos[rand() % 10]; //chooses random set of easyMode nums
   }
 else {
  chosenNumSet = solutions[rand() % 3188];  //chooses from all solutions
  }
//prints all the nums in the chosen num set 
  printf("\nThe numbers to use are: ");
    printf("%d, %d, %d, %d. \n", chosenNumSet.num1, chosenNumSet.num2, chosenNumSet.num3, chosenNumSet.num4);
  return 0;
  }


/*scan for inputted operators, store in a string, and check how many operators were inputted*/
int scanOps(void){
  printf("Enter the three operators to be used, in order (+,-,*, or /): ");
  (void)!scanf("%s", operators); 
  if(!(strlen(operators) == 3)){ //checks if # of inputted ops is 3, if not, then prints error, starts new round
    printf("Error! The number of operators is incorrect. Please try again. \n\n");
    play(); 
    exit(0); 
  }
  return 0; 
}


 /*checks for invalid operators within the passed array*/
int checkOps(char ops[]) {
  validOps = true; //defaults true
  for(int l = 0; l < 3; l++){
    if(!(operators[l] == '+' || operators[l] == '-' || operators[l] == '/' || operators[l] == '*')) {
      validOps = false; //ops can only be these 4 characters
    }
  }
  return 0;
}


/*for debug mode - calculates the solution based on provided array of numbers and array of operators, returns whether the total is equal to 24, all operators inputted will be valid since they are controlled in debugging mode*/
bool calculateDebugOutput(double numSet[], char operatorSet[]) {
    double firstNum = numSet[0]; //sets first num in set to var firstNum
    double total = 0;
  for(int y = 0; y < 3; y++){
    char c = operatorSet[y];
    double secondNum = numSet[y+1]; //will always be second num in each equation
    //look for operator input and apply accordingly
    total = (c=='+')?(firstNum+secondNum):((c=='-')?(firstNum-secondNum):
             ((c=='*')?(firstNum*secondNum):((c=='/')?(firstNum/secondNum):0)));
    firstNum = total; //sets the total of each line to the first num for use in the next line of math
       }
    return(fabs( total - 24.0 ) < .1) ; 
  }


/*calculates the solution based on the provided array of numbers and array of operators and whether the the inputted operators (in form of bool validOperators) were valid, checks with ints*/
bool calculateOutput(bool validOperators, double numSet[], char operatorSet[]) {
  //runs calculation if ops are valid, incorrect num already checked
  if(validOps) {
    double firstNum = numSet[0]; //sets first num in set to var firstNum
    int total = 0;
  for(int y = 0; y < 3; y++){
    char c = operatorSet[y];
    double secondNum = numSet[y+1]; //will always be second num in each equation
    //look for operator input and apply accordingly
    total = (c=='+')?(firstNum+secondNum):((c=='-')?(firstNum-secondNum):
             ((c=='*')?(firstNum*secondNum):((c=='/')?(firstNum/secondNum):0)));
      //prints math done in each step
    printf("%d %c %d = %d.\n", (int)firstNum, c, (int)secondNum, total); 
    firstNum = total; //sets the total of each line to the first num for the next line of math
       }
    return(total == 24);
      }
   //if there are invalid ops 
  else { 
    printf("Error! Invalid operator entered. Please try again.\n");
    play();
    exit(0); //stops recursion
  }
  return 0;
}


/*prints statement based on whether or not the combination of numbers and operators evaluated to 24 (from boolean totalStatus) */
int totalMessage(int totalStatus) {
    if(totalStatus){
      printf("Well done! You are a math genius. \n\n");
    }
  else {
    printf("Sorry. Your solution did not evaluate to 24.\n\n");
  }
  return 0;
}


/*asks to play again, scans for answer, and responds accordingly*/
int playAgain(void) {
  printf("Would you like to play again? Enter N for no and any other character for yes.");
  (void)!scanf(" %c", &yesOrNo); 
  if(yesOrNo == 'N'){ //checks if the user inputted 'N' for no, if so then game is over 
    printf("\nThanks for playing!\n");
    return 0;
  }
  else { //if any other character is inputted, another round begins
    play();
    }
  return 0;
}


/*play function creates and implements "rounds" by utilizing other methods*/
int play() {
  presentNums(easyMode);//Choose and present numbers
  //populate an array with the chosen values for easier access in for loops and etc.
   double nums[] = {chosenNumSet.num1, chosenNumSet.num2, chosenNumSet.num3, chosenNumSet.num4};
  //Input of operators
  scanOps();
  checkOps(operators);
  totalMessage(calculateOutput(validOps, nums, operators));
  playAgain();
  return 0;
}