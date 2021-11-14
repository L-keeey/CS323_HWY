%{
    #include "string.h"
    #include "symbolTable.cpp"
    void yyerror(const char*);

    struct Node* root;
%}
%locations
%union{
    struct Node* Node_value;
}
%nonassoc LOWER_ERROR
%nonassoc ERROR
%nonassoc <Node_value> LOWER_ELSE
%nonassoc <Node_value> ELSE
%token <Node_value> TYPE STRUCT
%token <Node_value> IF WHILE RETURN 

%token <Node_value> INT
%token <Node_value> FLOAT
%token <Node_value> CHAR
%token <Node_value> ID

%right <Node_value> ASSIGN
%left <Node_value> OR
%left <Node_value> AND
%left <Node_value> LT LE GT GE NE EQ 
%nonassoc LOWER_MINUS
%left <Node_value> PLUS MINUS 
%left <Node_value> MUL DIV
%right <Node_value> NOT 
%left <Node_value> LP RP LB RB DOT
%token <Node_value> SEMI COMMA 
%token <Node_value> LC RC

%type <Node_value> Program ExtDecList
%type <Node_value> ExtDef ExtDefList Specifier StructSpecifier VarDec 
%type <Node_value> FunDec VarList ParamDec CompSt StmtList Stmt DefList 
%type <Node_value> Def DecList Dec Args Exp
%%
/* high-level definition */
Program: 
    ExtDefList{
        $$ = new_Node_l("Program", @$.first_line);
        addChild($$, $1);
        root = $$;
    }
    ;
ExtDefList: 
     /* to allow empty input */ { $$ = new_Node_l_n("ExtDefList", @$.first_line);}
    | ExtDef ExtDefList         { $$ = new_Node_l("ExtDefList", @$.first_line); addChild($$, $1); addChild($$, $2);}
    ;
ExtDef: 
      Specifier ExtDecList SEMI { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);defVar($1,$2,@$.first_line);}
    | Specifier SEMI            { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2);}
    | Specifier FunDec CompSt   { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); defFun($1,$2,$3,@$.first_line);}
    | Specifier ExtDecList error {show_yyerror(MISSING_SEMI);}
    ;
ExtDecList:
      VarDec                    { $$ = new_Node_l("ExtDecList", @$.first_line); addChild($$, $1);}
    | VarDec COMMA ExtDecList   { $$ = new_Node_l("ExtDecList", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | VarDec ExtDecList error {show_yyerror(MISSING_COMMA);}
    ;

/* specifier */
Specifier: 
      TYPE              { $$ = new_Node_l("Specifier", @$.first_line); addChild($$, $1);}
    | StructSpecifier   { $$ = new_Node_l("Specifier", @$.first_line); addChild($$, $1);}
    ;
StructSpecifier: 
      STRUCT ID LC DefList RC   { $$ = new_Node_l("StructSpecifier", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | STRUCT ID                 { $$ = new_Node_l("StructSpecifier", @$.first_line); addChild($$, $1); addChild($$, $2);}               
    | STRUCT ID LC DecList error {show_yyerror(MISSING_RC);}
    ;
    
/* declarator */
VarDec: 
      ID                { $$ = new_Node_l("VarDec", @$.first_line); addChild($$, $1);}
    | VarDec LB INT RB  { $$ = new_Node_l("VarDec", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | VarDec LB INT error %prec LOWER_ERROR {show_yyerror(MISSING_RB);}
    ;
FunDec: 
      ID LP VarList RP  { $$ = new_Node_l("FunDec", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);addarg($3,@$.first_line);}
    | ID LP RP          { $$ = new_Node_l("FunDec", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ID LP VarList error {show_yyerror(MISSING_RP);}
    | ID LP error {show_yyerror(MISSING_RP);}
    ;
VarList: 
      ParamDec COMMA VarList    { $$ = new_Node_l("VarList", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ParamDec VarList error {show_yyerror(MISSING_COMMA);}
    | ParamDec                  { $$ = new_Node_l("VarList", @$.first_line); addChild($$, $1); }
    ;
ParamDec: Specifier VarDec      { $$ = new_Node_l("ParamDec", @$.first_line); addChild($$, $1); addChild($$, $2);}
    ;

/* statement */
CompSt: 
      LC DefList StmtList RC    { $$ = new_Node_l("CompSt", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    ;
StmtList: 
      /* to allow empty input */    { $$ = new_Node_l_n("StmtList", @$.first_line);}
    | Stmt StmtList                 { $$ = new_Node_l("StmtList", @$.first_line); addChild($$, $1); addChild($$, $2);}
    ;
Stmt: 
      Exp SEMI                              { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1); addChild($$, $2);}
    | CompSt                                { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1);}
    | RETURN Exp SEMI                       { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | IF LP Exp RP Stmt %prec LOWER_ELSE    { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}    
    | IF LP Exp RP Stmt ELSE Stmt           { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5); addChild($$, $6); addChild($$, $7);}
    | WHILE LP Exp RP Stmt                  { $$ = new_Node_l("Stmt", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | WHILE LP Exp error Stmt               {show_yyerror(MISSING_RP);}   
    | Exp error                             {show_yyerror(MISSING_SEMI);} 
    | RETURN Exp error                      {show_yyerror(MISSING_SEMI);}
    | IF LP Exp error Stmt                  {show_yyerror(MISSING_RP);}
    ;

/* local definition */
DefList: 
     /* to allow empty input */ { $$ = new_Node_l_n("DefList", @$.first_line);}
    | Def DefList                { $$ = new_Node_l("DefList", @$.first_line); addChild($$, $1); addChild($$, $2);}
    ;
Def: 
     Specifier DecList SEMI { $$ = new_Node_l("Def", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);defVar($1,$2,@$.first_line);}
    | Specifier DecList error {show_yyerror(MISSING_SEMI);}
    | error DecList SEMI {show_yyerror(MISSING_SPEC);}
    ;
DecList: 
      Dec                   { $$ = new_Node_l("DecList", @$.first_line); addChild($$, $1);}
    | Dec COMMA DecList     { $$ = new_Node_l("DecList", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Dec DecList error {show_yyerror(MISSING_COMMA);}
    ;
Dec: 
      VarDec                { $$ = new_Node_l("Dec", @$.first_line); addChild($$, $1);}
    | VarDec ASSIGN Exp     { $$ = new_Node_l("Dec", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;

/* Expression */
Exp: 
      Exp ASSIGN Exp        { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);check_rvalue($1,@$.first_line);checkAssignOperand($1,$3,@$.first_line);$$->type_value=$3->type_value;}
    | Exp AND Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);if(!(checkINTexp($1,1)&&checkINTexp($3,1))){printType7Error(@$.first_line);}$$->type_value=new_prim_type("bool");}
    | Exp OR Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);if(!(checkINTexp($1,1)&&checkINTexp($3,1))){printType7Error(@$.first_line);}$$->type_value=new_prim_type("bool");}
    | Exp LT Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp LE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp GT Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp GE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp NE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp EQ Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);typeAfterCalc($1,$3,@$.first_line);$$->type_value=new_prim_type("bool");}
    | Exp PLUS Exp          { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=typeAfterCalc($1,$3,@$.first_line);}
    | Exp MINUS Exp         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=typeAfterCalc($1,$3,@$.first_line);}
    | Exp MUL Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=typeAfterCalc($1,$3,@$.first_line);}
    | Exp DIV Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=typeAfterCalc($1,$3,@$.first_line);}
    | LP Exp RP             { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=$2->type_value;}
    | LP Exp error          {show_yyerror(MISSING_RP);}
    | MINUS Exp %prec LOWER_MINUS   { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2);$$->type_value=$2->type_value;}
    | NOT Exp               { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2);if(!checkINTexp($2,1)){printType7Error(@$.first_line);}$$->type_value=new_prim_type("bool");}
    | ID LP Args RP         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); invokeFun($$,$1,$3,@$.first_line);}
    | ID LP Args error      {show_yyerror(MISSING_RP);}
    | ID LP RP              { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); invokeFun($$,$1,NULL,@$.first_line);}
    | ID LP error           {show_yyerror(MISSING_RP);}
    | Exp LB Exp RB         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);if(!checkINTexp($3,0)){printType12Error(@$.first_line);} determineExpType($$,$1,@$.first_line);}
    | Exp LB Exp error      {show_yyerror(MISSING_RB);}
    | Exp DOT ID            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);$$->type_value=checkStructMember($1,$3,@$.first_line);}
    | ID                    { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);$$->type_value=findID($1->string_value,@$.first_line);}
    | INT                   { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);$$->type_value=new_prim_type("int");}
    | FLOAT                 { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);$$->type_value=new_prim_type("float");}
    | CHAR                  { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);$$->type_value=new_prim_type("char");}
    | Exp ERROR Exp {}
    | ERROR {}
    ;
Args: 
      Exp COMMA Args    { $$ = new_Node_l("Args", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp COMMA COMMA Args error {show_yyerror(EXTRC_COMMA);}
    | Exp               { $$ = new_Node_l("Args", @$.first_line); addChild($$, $1);}
    ;
%%
void yyerror(const char *s) {
    error_num += 1;
    fprintf(stdout, "Error type B at Line %d: ", yylloc.first_line - 1);
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
void slice(const char *str,char *result, int start,int end ){
    
}

