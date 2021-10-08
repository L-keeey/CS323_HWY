
struct Node
{
    /* data */
    char* name;
    int child_num;
    Node* list[7];
    int lineNo;
};

struct Node *newNode(char* name, int num);