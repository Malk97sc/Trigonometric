#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#ifndef MPI
#define MPI 3.14159265358979323846
#endif

typedef enum{ //tokens
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_MULT, TOKEN_DIV,
    TOKEN_POW, TOKEN_LPAREN, TOKEN_RPAREN,
    TOKEN_SIN, TOKEN_COS, TOKEN_TAN,
    TOKEN_NUM, TOKEN_ID, TOKEN_END, TOKEN_ERROR
} TokenType;

typedef struct{ //token handler
    int index;
    char *input;
} TokenHnd; 

TokenType currentT;
TokenHnd handlerT;

double tokenValue; //global variable to store the value of the current number token
int hasVariable;   //flag to indicate if the expression contains variables

//function prototypes
TokenType nextToken();
void match(TokenType expct);
void printToken(TokenType token);
double degreesToRadians(double deg);
double evalTrig(double arg, TokenType trigFunc);

void iniParse(const char *str);
double parse_E();
void parse_E_prime(double *inherited);
double parse_T();
void parse_T_prime(double *inherited);
double parse_P();
void parse_P_prime(double *inherited);
double parse_F();
double parse_Trig();

int main(int argc, char **argv){
    if (argc < 2) {
        printf("Send expresion.\n");
        return EXIT_FAILURE;
    }

    iniParse(argv[1]);

    return EXIT_SUCCESS;
}

//-------------------------------------------------------------------------

// Convert degrees to radians
double degreesToRadians(double deg){
    return deg * (MPI / 180.0);
}

// Initialize parsing
void iniParse(const char *str){
    handlerT.input = (char*)str;
    handlerT.index = 0; //start index
    hasVariable = 0; //reset variable flag

    currentT = nextToken();
    double result = parse_E(); //start parsing

    if(currentT == TOKEN_END){
        if(hasVariable){
            printf("Accepted, but the expression contains variables; cannot fully evaluate.\n");
        }else{
            printf("Result: %lf\n", result);
        }
    }else{
        printf("Reject\n");
    }
}

//parsing functions
double parse_E(){
    double result = parse_T();
    parse_E_prime(&result);
    return result;
}

void parse_E_prime(double *inherited){
    if(currentT == TOKEN_PLUS){
        match(TOKEN_PLUS);
        double term = parse_T();
        *inherited += term;
        //printf("After addition, value = %lf\n", *inherited);
        parse_E_prime(inherited);
    }else if(currentT == TOKEN_MINUS){
        match(TOKEN_MINUS);
        double term = parse_T();
        *inherited -= term;
        //printf("After subtraction, value = %lf\n", *inherited);
        parse_E_prime(inherited);
    }
    //else epsilon 
}

double parse_T(){
    double result = parse_P();
    parse_T_prime(&result);
    return result;
}

void parse_T_prime(double *inherited){
    if(currentT == TOKEN_MULT){
        match(TOKEN_MULT);
        double factor = parse_P();
        *inherited *= factor;
        //printf("After multiplication, value = %lf\n", *inherited);
        parse_T_prime(inherited);
    }else if(currentT == TOKEN_DIV){
        match(TOKEN_DIV);
        double factor = parse_P();
        if(factor == 0){
            printf("Error: Division by zero\n");
            exit(1);
        }
        *inherited /= factor;
        //printf("After division, value = %lf\n", *inherited);
        parse_T_prime(inherited);
    }
    //else epsilon 
}

double parse_P(){
    double base = parse_F();
    parse_P_prime(&base);
    return base;
}

void parse_P_prime(double *inherited){
    if(currentT == TOKEN_POW){
        match(TOKEN_POW);
        double exponent = parse_P(); //Correctly parse the exponent
        //printf("Exponentiation: base = %lf, exponent = %lf\n", *inherited, exponent);
        *inherited = pow(*inherited, exponent);
        //printf("Result of exponentiation: %lf\n", *inherited);
    }
    //else epsilon 
}

double parse_F(){
    if(currentT == TOKEN_LPAREN){ // '(' E ')'
        match(TOKEN_LPAREN);
        double value = parse_E();
        match(TOKEN_RPAREN);
        return value;
    }else if(currentT == TOKEN_NUM){ // NUM
        double numValue = tokenValue;
        match(TOKEN_NUM);
        return numValue;
    }else if(currentT == TOKEN_ID){ // variable
        hasVariable = 1; // Set variable flag
        match(TOKEN_ID);
        return 0; // Return 0 or any default value since we can't evaluate variables
    }else if(currentT == TOKEN_SIN || currentT == TOKEN_COS || currentT == TOKEN_TAN){
        return parse_Trig();
    }else{
        printf("Error: Unexpected token in parse_F\n");
        exit(1);
    }
}

double parse_Trig(){
    TokenType trigFunc = currentT;
    match(trigFunc);
    match(TOKEN_LPAREN);
    double arg = parse_E();
    match(TOKEN_RPAREN);
    return evalTrig(arg, trigFunc);
}

//Evaluate trigonometric functions
double evalTrig(double arg, TokenType trigFunc){
    if(hasVariable){
        return 0; // Can't evaluate if the argument contains variables
    }
    arg = degreesToRadians(arg); // Convert degrees to radians
    switch(trigFunc){
        case TOKEN_SIN: return sin(arg);
        case TOKEN_COS: return cos(arg);
        case TOKEN_TAN: return tan(arg);
        default:
            printf("Error: Unknown trigonometric function\n");
            exit(1);
    }
}

//Match function
void match(TokenType expct){
    if(currentT == expct){
        currentT = nextToken();
    }else if(currentT == TOKEN_ERROR){
        printf("Error: Invalid token encountered\n");
        exit(1);
    }else{
        printf("Error: Expected different token\n");
        exit(1);
    }
}

// Tokenizer function
TokenType nextToken(){
    while(isspace(handlerT.input[handlerT.index])) handlerT.index++;
    if(handlerT.input[handlerT.index] == '\0') return TOKEN_END; //end of input

    char curr = handlerT.input[handlerT.index];

    //numbers
    if(isdigit(curr) || curr == '.'){ //handle numbers
        char buffer[50];//extract the full number
        int i = 0;
        while(isdigit(handlerT.input[handlerT.index]) || handlerT.input[handlerT.index] == '.'){
            buffer[i++] = handlerT.input[handlerT.index++];
            if(i > 50){
                printf("Error: Number too long\n");
                exit(1);
            }
        }
        buffer[i] = '\0';
        tokenValue = atof(buffer); //store the number's value in global variable
        return TOKEN_NUM;
    }

    // Identifiers and functions
    if(isalpha(curr)){
        char buffer[50]; //extract the full identifier
        int i = 0;
        while(isalpha(handlerT.input[handlerT.index])){
            buffer[i++] = handlerT.input[handlerT.index++];
            if(i > 50){
                printf("Error: Identifier too long\n");
                exit(1);
            }
        }
        buffer[i] = '\0';
        if(!strcasecmp(buffer, "sin")){ //equal to (strcasecmp(buffer, "sin") == 0)
            return TOKEN_SIN;
        }else if(!strcasecmp(buffer, "cos")){
            return TOKEN_COS;
        }else if(!strcasecmp(buffer, "tan")){
            return TOKEN_TAN;
        }else{
            return TOKEN_ID; //else, is a ID
        }
    }

    // Operators and parentheses
    handlerT.index++; // Advance the index
    switch(curr){
        case '+': return TOKEN_PLUS;
        case '-': return TOKEN_MINUS;
        case '*': return TOKEN_MULT;
        case '/': return TOKEN_DIV;
        case '^': return TOKEN_POW;
        case '(': return TOKEN_LPAREN;
        case ')': return TOKEN_RPAREN;
        default:
            return TOKEN_ERROR;
    }
}
