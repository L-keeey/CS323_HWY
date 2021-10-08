enum display_type{
    has_num,
    has_colon,
    type_only
};

typedef struct Node
{
    char* token;
    enum display_type type;

    int line_No;
    char* string_value;
    union{
        int int_value;
        float float_value;
    };

    int child_num;
    struct Node* child_list[7];

} Node;

// INT, *
Node* new_Node_i(char* token, int value, enum display_type type);

// FLOAT
Node* new_Node_f(char* token, float value);

// ID, TYPE
Node* new_Node_s(char* token, char* value);

// COMMA, RP, LP, RC, LC, RB, LB, ASSIGN, SEMI, IF, GT
Node* new_Node(char* token);

void addChild(struct Node* parent, struct Node* child);
