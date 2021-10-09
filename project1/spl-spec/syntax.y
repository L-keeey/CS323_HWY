%{
    #include "lex.yy.c"
    void yyerror(const char*);

    struct Node root;
%}

%union{
    Node* Node_value;
    char* string_value;
    int int_value;
    float float_value;
}
%token <Node_value> STRUCT IF ELSE WHILE RETURN SEMI COMMA LC RC
%type <Node_value> Def DecList Dec DefList Stmt VarList CompSt StmtList Specifier ParamDec
%type <Node_value> FunDec VarDec StructSpecifier ExtDecList ExtDefList ExtDef Program
%token <int_value> INT
%token <string_value> TYPE
%token <string_value> ID
%token <float_value> FLOAT
%token <string_value> CHAR
%type <Node_value> Exp Args

%right <Node_value> ASSIGN
%nonassoc <Node_value> OR
%nonassoc <Node_value> AND
%left <Node_value> LT LE GT GE NE EQ 
%left <Node_value> PLUS MINUS 
%left <Node_value> MUL DIV
%right <Node_value> NOT 
%left <Node_value> LP RP LB RB DOT
%%

/* high-level definition */
Program: 
    ExtDefList{
        $$ = new_Node_i("Program", @$.first_line, has_num);
        addChild($$, $1);
        root = $$;
    }
    ;
ExtDefList: 
     /* to allow empty input */ { $$ = new_Node_i("ExtDefList", @$.first_line, has_num);}
    | ExtDef ExtDefList         { $$ = new_Node_i("ExtDefList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    ;
ExtDef: 
      Specifier ExtDecList SEMI { $$ = new_Node_i("ExtDef", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Specifier SEMI            { $$ = new_Node_i("ExtDef", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    | Specifier FunDec CompSt   { $$ = new_Node_i("ExtDef", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
ExtDecList:
      VarDec                    { $$ = new_Node_i("ExtDecList", @$.first_line, has_num); addChild($$, $1);}
    | VarDec COMMA ExtDecList   { $$ = new_Node_i("ExtDecList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;

/* specifier */
Specifier: 
      TYPE              { $$ = new_Node_i("Specifier", @$.first_line, has_num); addChild($$, $1);}
    | StructSpecifier   { $$ = new_Node_i("Specifier", @$.first_line, has_num); addChild($$, $1);}
    ;
StructSpecifier: 
      STRUCT ID LC DefList RC   { $$ = new_Node_i("EStructSpecifier", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | STRUCT ID                 { $$ = new_Node_i("EStructSpecifier", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}               
    ;
    
/* declarator */
VarDec: 
      ID                { $$ = new_Node_i("VarDec", @$.first_line, has_num); addChild($$, $1);}
    | VarDec LB INT RB  { $$ = new_Node_i("VarDec", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    ;
FunDec: 
      ID LP VarList RP  { $$ = new_Node_i("FunDec", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | ID LP RP          { $$ = new_Node_i("FunDec", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
VarList: 
      ParamDec COMMA VarList    { $$ = new_Node_i("VarList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ParamDec                  { $$ = new_Node_i("VarList", @$.first_line, has_num); addChild($$, $1); }
    ;
ParamDec: Specifier VarDec
    ;

/* statement */
CompSt: 
      LC DefList StmtList RC    { $$ = new_Node_i("CompSt", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
StmtList: 
      /* to allow empty input */    { $$ = new_Node_i("StmtList", @$.first_line, has_num);}
    | Stmt StmtList                 { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    ;
Stmt: 
      Exp SEMI                          { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    | CompSt                            { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1);}
    | RETURN Exp SEMI                   { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | IF LP Exp RP Stmt                 { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    | IF LP Exp RP Stmt ELSE Stmt       { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5); addChild($$, $6); addChild($$, $7);}
    | WHILE LP Exp RP Stmt              { $$ = new_Node_i("StmtList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4); addChild($$, $5);}
    ;

/* local definition */
DefList: 
     /* to allow empty input */ { $$ = new_Node_i("DefList", @$.first_line, has_num);}
    | Def DefList                { $$ = new_Node_i("DefList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    ;
Def: 
     Specifier DecList SEMI { $$ = new_Node_i("Def", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
DecList: 
      Dec                   { $$ = new_Node_i("DecList", @$.first_line, has_num);}
    | Dec COMMA DecList     { $$ = new_Node_i("DecList", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;
Dec: 
      VarDec                { $$ = new_Node_i("Dec", @$.first_line, has_num);}
    | VarDec ASSIGN Exp     { $$ = new_Node_i("Dec", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    ;

/* Expression */
Exp: 
      Exp ASSIGN Exp        { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp AND Exp           { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp OR Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LT Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LE Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GT Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp GE Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp NE Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp EQ Exp            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp PLUS Exp          { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MINUS Exp         { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp MUL Exp           { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp DIV Exp           { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | LP Exp RP             { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | MINUS Exp %prec NOT   { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    | NOT Exp               { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2);}
    | ID LP Args RP         { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | ID LP RP              { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp LB Exp RB         { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3); addChild($$, $4);}
    | Exp DOT ID            { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | ID                    { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1);}
    | INT                   { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1);}
    | FLOAT                 { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1);}
    | CHAR                  { $$ = new_Node_i("Exp", @$.first_line, has_num); addChild($$, $1);}
    ;
Args: 
      Exp COMMA Args    { $$ = new_Node_i("Args", @$.first_line, has_num); addChild($$, $1); addChild($$, $2); addChild($$, $3);}
    | Exp               { $$ = new_Node_i("Args", @$.first_line, has_num); addChild($$, $1);}
    ;
%%



void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}
int main() {
    yyparse();
}

