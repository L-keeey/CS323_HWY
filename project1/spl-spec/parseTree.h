#include <stdlib.h>
#include <stdio.h>
#include <string.h>


enum display_type{
    has_num,
    has_colon,
    type_only
};

typedef struct Node
{
    char token[100];
    enum display_type type;
    int not_print;

    int line_No;
    char string_value[100];
    union{
        int int_value;
        float float_value;
    };

    int child_num;
    struct Node* child_list[7];

}Node;

//*
struct Node* new_Node_l_n(char* token, int line){
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_num;
    node->line_No = line;
    node->not_print = 1;

    return node;
}

//*
struct Node* new_Node_l(char* token, int line){
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_num;
    node->line_No = line;

    return node;
}

// HEX
struct Node* new_Node_h(char* token, int value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->int_value = value;
    sprintf(string_value, "%d", value);
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_colon;

    return node;
}

// INT
struct Node* new_Node_i(char* token, int int_value, char* value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->int_value = int_value;
    strcpy(node->string_value, value);
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_colon;

    return node;
}

// FLOAT
struct Node* new_Node_f(char* token, float float_value, char* value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_colon;
    node->float_value = float_value;
    strcpy(node->string_value, value);
    return node;
}

// ID, TYPE
struct Node* new_Node_s(char* token, char* value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_colon;
    strcpy(node->string_value, value);

    return node;
}

// COMMA, RP, LP, RC, LC, RB, LB, ASSIGN, SEMI, IF, GT
struct Node* new_Node(char* token)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = type_only;
        
    return node;
}

void addChild(struct Node* parent, struct Node* child)
{
    parent->child_list[parent->child_num]=child;
    parent->child_num++;
}

void print_info(struct Node* node, int space)
{
    if(node->not_print == 0)
    {
        for (int i = 0; i < space; i++)
        {
            printf("  ");
        }

        if (node->type == has_colon)
        {
            printf("%s: %s\n", node->token, node->string_value);
        }
        else if(node->type == has_num)
        {
            printf("%s (%d)\n", node->token, node->line_No);
        }
        else
        {
            printf("%s\n", node->token);
        }
    }
}

enum ERROR_TYPE
{
    MISSING_SEMI=1, // que shi fen hao
    MISSING_RC,
    MISSING_RB,
    MISSING_RP,
    MISSING_LC,
    MISSING_LB,
    MISSING_LP,
    MISSING_COMMA,
    MISSING_SPEC,
    EXTRC_SEMI,
    EXTRC_COMMA
}error_type;

void show_yyerror(enum ERROR_TYPE type) {
    char msg[50];
    switch (type)
    {
    case /* constant-expression */MISSING_SEMI:
        /* code */
        strcpy(msg, "Missing semicolon \';\'");
        break;

    case MISSING_SPEC:
        strcpy(msg, "Missing specifier");
        break;

    case MISSING_COMMA:
        strcpy(msg, "Missing comma \',\'");
        break;

    case MISSING_LB:
        strcpy(msg, "Missing left bracket \'[\'");
        break;

    case MISSING_RB:
        strcpy(msg, "Missing closing bracket \'[\'");
        break;

    case MISSING_LC:
        strcpy(msg, "Missing left curly braces \'{\'");
        break;

    case MISSING_RC:
        strcpy(msg, "Missing close curly braces \'}\'");
        break;

    case MISSING_LP:
        strcpy(msg, "Missing left parenthesis \'(\'");
        break;

    case MISSING_RP:
        strcpy(msg, "Missing closing parenthesis \')\'");
        break;

    case EXTRC_SEMI:
        strcpy(msg, "Redundant comma appears");
        break;

    case EXTRC_COMMA:
        strcpy(msg, "Redundant semicolon appears");
        break;

    default:
        strcpy(msg, "Unknown type of error.");
        break;
    }

    fprintf(stdout, "%s\n", msg);
} 