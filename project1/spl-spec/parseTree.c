#include "parseTree.h"

// INT, *
Node* new_Node_i(char* token, int value, enum display_type type)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->token = token;
    node->child_num = 0;
    node->type = type;
    if (type = has_num)
        node->line_No = value;
    else
        node->int_value = value;

    return node;
}

// FLOAT
Node* new_Node_f(char* token, float value)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->token = token;
    node->child_num = 0;
    node->type = has_colon;
    node->float_value = value;

    return node;
}

// ID, TYPE
Node* new_Node_s(char* token, char* value)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->token = token;
    node->child_num = 0;
    node->type = has_colon;
    node->string_value = value;

    return node;
}

// COMMA, RP, LP, RC, LC, RB, LB, ASSIGN, SEMI, IF, GT
Node* new_Node(char* token)
{
    Node* node = (Node*) malloc(sizeof(Node));
    node->token = token;
    node->child_num = 0;
    node->type = type_only;
        
    return node;
}

void addChild(struct Node* parent, struct Node* child)
{
    parent->child_list[parent->child_num]=child;
    parent->child_num++;
}