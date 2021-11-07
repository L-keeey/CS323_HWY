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
