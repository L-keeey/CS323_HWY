#include <stdlib.h>
#include <stdio.h>

enum display_type{
    has_num,
    has_colon,
    type_only
};

typedef struct Node
{
    char token[100];
    enum display_type type;

    int line_No;
    char string_value[100];
    union{
        int int_value;
        float float_value;
    };

    // char to_print[100];
    int child_num;
    struct Node* child_list[7];

}Node;


//*
struct Node* new_Node_l(char* token, int line){
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_num;
    // char line_num[100];
    // sprintf(line_num, "(%d)", line);
    // strcpy(node->to_print, token);
    // strcat(node->to_print, " ");
    // strcat(node->to_print, line_num);

    return node;
}

// INT, *
struct Node* new_Node_i(char* token, int int_value, char* value)
{
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    node->int_value = int_value;
    strcpy(node->string_value, value);
    strcpy(node->token, token);
    node->child_num = 0;
    node->type = has_colon;
    // strcpy(node->to_print, token);
    // strcat(node->to_print, ": ");
    // strcat(node->to_print, value);

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