enum CATE {PRIMITIVE, ARRAY, STRUCTURE};
enum PRIM {_INT, _FLOAT, _CHAR};

typedef struct Type{
    char name[32];
    CATE category;
    union 
    {
        PRIM primitive;
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
    prim->category = PRIMITIVE;
    
    if(strcmp(type, "int")==0)
        prim->primitive = _INT;
    else if (strcmp(type, "float")==0)
        prim->primitive = _FLOAT;
    else if (strcmp(type,"bool")==0){
        prim->primitive = _INT;
        strcpy(prim->name,"bool");
    }else
        prim->primitive = _CHAR;

    return prim;
}