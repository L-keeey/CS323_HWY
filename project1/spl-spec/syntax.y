%{
    #include "lex.yy.c"
    #include "string.h"
    void yyerror(const char*);

    struct Node* root;
    int error_num = 0;
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
      Specifier ExtDecList SEMI { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Specifier SEMI            { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2);}
    | Specifier FunDec CompSt   { $$ = new_Node_l("ExtDef", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
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
      ID LP VarList RP  { $$ = new_Node_l("FunDec", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
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
    | LC DefList StmtList DefList error {show_yyerror(MISSING_SPEC);}
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
    | WHILE LP Exp error Stmt {show_yyerror(MISSING_RP);}   
    | Exp error {show_yyerror(MISSING_SEMI);} 
    | RETURN Exp error {show_yyerror(MISSING_SEMI);}
    | IF LP Exp error Stmt {show_yyerror(MISSING_RP);}
    ;

/* local definition */
DefList: 
     /* to allow empty input */ { $$ = new_Node_l_n("DefList", @$.first_line);}
    | Def DefList                { $$ = new_Node_l("DefList", @$.first_line); addChild($$, $1); addChild($$, $2);}
    ;
Def: 
     Specifier DecList SEMI { $$ = new_Node_l("Def", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
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
      Exp ASSIGN Exp        { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp AND Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp OR Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LT Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GT Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp NE Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp EQ Exp            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp PLUS Exp          { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MINUS Exp         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MUL Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp DIV Exp           { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | LP Exp RP             { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | LP Exp error {show_yyerror(MISSING_RP);}
    | MINUS Exp %prec LOWER_MINUS   { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2);}
    | NOT Exp               { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2);}
    | ID LP Args RP         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | ID LP Args error {show_yyerror(MISSING_RP);}
    | ID LP RP              { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ID LP error {show_yyerror(MISSING_RP);}
    | Exp LB Exp RB         { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | Exp LB Exp error {show_yyerror(MISSING_RB);}
    | Exp DOT ID            { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ID                    { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);}
    | INT                   { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);}
    | FLOAT                 { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);}
    | CHAR                  { $$ = new_Node_l("Exp", @$.first_line); addChild($$, $1);}
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

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    char output_dir[100];
    strncpy(output_dir,argv[1],strlen(argv[1])-4);
    strcat(output_dir,".out1");
    freopen(output_dir,"w",stdout);
    yyparse();

    if(error_num == 0){
        pre_order_traversal(root, 0);
    }
    fclose(stdout);
    return 0;
}
