
struct Node
{
    /* data */
    char* name;
    int child_num;
    Node* list[7];
    int lineNo;
};


struct Node *newNode(char* name){
    Node node;
    node.name=name;
    node.child_num=0;
}
void addchild(Node* parent,Node* child){
    parent->list[parent->child_num]=child;
    parent->child_num++;
}