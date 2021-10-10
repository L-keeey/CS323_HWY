%{
    #include "lex.yy.c"
    void yyerror(const char*);

    struct Node* root;
    int error_num = 0;
%}
%locations
%union{
    struct Node* Node_value;
}
%token <Node_value> STRUCT IF ELSE WHILE RETURN SEMI COMMA LC RC

%token <Node_value> INT
%token <Node_value> TYPE
%token <Node_value> ID
%token <Node_value> FLOAT
%token <Node_value> CHAR
%type <Node_value> Exp Args

%right <Node_value> ASSIGN
%left <Node_value> OR
%left <Node_value> AND
%left <Node_value> LT LE GT GE NE EQ 
%left <Node_value> PLUS MINUS 
%left <Node_value> MUL DIV
%right <Node_value> NOT 
%left <Node_value> LP RP LB RB DOT
%type <Node_value> FunDec VarDec StructSpecifier ExtDecList ExtDefList ExtDef Program
%type <Node_value> Def DecList Dec DefList Stmt VarList CompSt StmtList Specifier ParamDec
%%
/* high-level definition */
Program: 
    ExtDefList{
        $$ = new_Node_l("Program", @$.last_line);
        addChild($$, $1);
        root = $$;
    }
    ;
ExtDefList: 
     /* to allow empty input */ { $$ = new_Node_l("ExtDefList", @$.last_line);}
    | ExtDef ExtDefList         { $$ = new_Node_l("ExtDefList", @$.last_line); addChild($$, $1); addChild($$, $2);}
    ;
ExtDef: 
      Specifier ExtDecList SEMI { $$ = new_Node_l("ExtDef", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Specifier SEMI            { $$ = new_Node_l("ExtDef", @$.last_line); addChild($$, $1); addChild($$, $2);}
    | Specifier FunDec CompSt   { $$ = new_Node_l("ExtDef", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
ExtDecList:
      VarDec                    { $$ = new_Node_l("ExtDecList", @$.last_line); addChild($$, $1);}
    | VarDec COMMA ExtDecList   { $$ = new_Node_l("ExtDecList", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;

/* specifier */
Specifier: 
      TYPE              { $$ = new_Node_l("Specifier", @$.last_line); addChild($$, $1);}
    | StructSpecifier   { $$ = new_Node_l("Specifier", @$.last_line); addChild($$, $1);}
    ;
StructSpecifier: 
      STRUCT ID LC DefList RC   { $$ = new_Node_l("StructSpecifier", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | STRUCT ID                 { $$ = new_Node_l("StructSpecifier", @$.last_line); addChild($$, $1); addChild($$, $2);}               
    ;
    
/* declarator */
VarDec: 
      ID                { $$ = new_Node_l("VarDec", @$.last_line); addChild($$, $1);}
    | VarDec LB INT RB  { $$ = new_Node_l("VarDec", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    ;
FunDec: 
      ID LP VarList RP  { $$ = new_Node_l("FunDec", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | ID LP RP          { $$ = new_Node_l("FunDec", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
VarList: 
      ParamDec COMMA VarList    { $$ = new_Node_l("VarList", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ParamDec                  { $$ = new_Node_l("VarList", @$.last_line); addChild($$, $1); }
    ;
ParamDec: Specifier VarDec      { $$ = new_Node_l("ParamDec", @$.last_line); addChild($$, $1); addChild($$, $2);}
    ;

/* statement */
CompSt: 
      LC DefList StmtList RC    { $$ = new_Node_l("CompSt", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
StmtList: 
      /* to allow empty input */    { $$ = new_Node_l("StmtList", @$.last_line);}
    | Stmt StmtList                 { $$ = new_Node_l("StmtList", @$.last_line); addChild($$, $1); addChild($$, $2);}
    ;
Stmt: 
      Exp SEMI                          { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1); addChild($$, $2);}
    | CompSt                            { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1);}
    | RETURN Exp SEMI                   { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | IF LP Exp RP Stmt                 { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | IF LP Exp RP Stmt ELSE Stmt       { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5); addChild($$, $6); addChild($$, $7);}
    | WHILE LP Exp RP Stmt              { $$ = new_Node_l("Stmt", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    ;

/* local definition */
DefList: 
     /* to allow empty input */ { $$ = new_Node_l("DefList", @$.last_line);}
    | Def DefList                { $$ = new_Node_l("DefList", @$.last_line); addChild($$, $1); addChild($$, $2);}
    ;
Def: 
     Specifier DecList SEMI { $$ = new_Node_l("Def", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
DecList: 
      Dec                   { $$ = new_Node_l("DecList", @$.last_line);}
    | Dec COMMA DecList     { $$ = new_Node_l("DecList", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
Dec: 
      VarDec                { $$ = new_Node_l("Dec", @$.last_line);}
    | VarDec ASSIGN Exp     { $$ = new_Node_l("Dec", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;

/* Expression */
Exp: 
      Exp ASSIGN Exp        { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp AND Exp           { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp OR Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LT Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LE Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GT Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GE Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp NE Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp EQ Exp            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp PLUS Exp          { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MINUS Exp         { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MUL Exp           { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp DIV Exp           { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | LP Exp RP             { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | MINUS Exp %prec NOT   { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2);}
    | NOT Exp               { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2);}
    | ID LP Args RP         { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | ID LP RP              { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LB Exp RB         { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | Exp DOT ID            { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ID                    { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1);}
    | INT                   { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1);}
    | FLOAT                 { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1);}
    | CHAR                  { $$ = new_Node_l("Exp", @$.last_line); addChild($$, $1);}
    ;
Args: 
      Exp COMMA Args    { $$ = new_Node_l("Args", @$.last_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp               { $$ = new_Node_l("Args", @$.last_line); addChild($$, $1);}
    ;
%%
void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

void pre_order_traversal(struct Node* root, int space)
{
    if(root){
        print_info(root, space);
        for (int i = 0; i < root->child_num; i++){
            pre_order_traversal(root->child_list[i], space+1);
        }
    }
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();

    if(error_num == 0){
        pre_order_traversal(root, 0);
    }
    return 0;
}
