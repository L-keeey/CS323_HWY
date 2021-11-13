typedef struct Type{
    char name[32];
    enum {PRIMITIVE, ARRAY, STRUCTURE} category;
    union 
    {
        enum {INT, FLOAT, CHAR} primitive;
        struct Array *array;
        struct FieldList *structure;
        
    };   
} Type;

typedef struct Array
{
    struct Type *base;
    int size;
} Array;

typedef struct FieldList
{
    char name[32];
    struct Type *type;
    struct FieldList *next;
} FieldList;

struct Type* new_prim_type(char* type){
    struct Type* prim = (struct Type*) malloc(sizeof(struct Type));
    prim->category = prim->PRIMITIVE;
    
    if(strcmp(type, "int"))
        prim->primitive = prim->INT;
    else if (strcmp(type, "float"))
        prim->primitive = prim->FLOAT;
    else if (strcmp(type,"bool")){
        prim->primitive = prim->INT;
        strcpy(prim->name,"bool");
    }else
        prim->primitive = prim->CHAR;

    return prim;
}