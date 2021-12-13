#include "syntax.tab.c"
#include <sstream>
#define sout(msg) // std::cout << msg << std::endl

int var_num = 0;
int label_num = 0;
int place_num = 0;

struct TAC
{
    int id;
    union{
        std::string relop;
        int size;
    };
    std::string target[3];
};

std::vector<TAC*> output;
std::map<std::string,std::string> id_address_map;
std::vector<std::string> args;
std::vector<std::string> indexes;
std::vector<int> sizes;

void printOutput();
void printTAC(struct TAC* input);
void translate_Program(struct Node* root);
void translate_ExtDefList(struct Node* in);
void translate_ExtDef(struct Node* in);
void translate_ExtDecList(struct Node* in);
void translate_VarDec(struct Node* in, int TACid);
void translate_FunDec(struct Node* in);
void translate_VarList(struct Node* in);
void translate_ParamDec(struct Node* in);
void translate_CompSt(struct Node* in);
void translate_StmtList(struct Node* in);
void translate_Stmt(struct Node* in);
void translate_DefList(struct Node* in);
void translate_Def(struct Node* in);
void translate_DecList(struct Node* in);
void translate_Dec(struct Node* in);
void translate_Exp(struct Node* in, std::string place);
void translate_cond_Exp(Node* in, std::string lb_t, std::string lb_f);
void translate_Args(struct Node* in);
int calculateSize(Type* array);
std::string new_place();
std::string new_var();
std::string new_label();
std::string retrive_value(std::string addr);

template <typename T>
std::string new_const(T c_value);

void pre_order_traversal(struct Node* root, int space) {
    if (root) {
        print_info(root, space);
        for (int i = 0; i < root->child_num; i ++) {
            pre_order_traversal(root->child_list[i], space+1);
        }
    }
}

int main(int argc, char **argv){
    std::vector<Type*> read, write;
    Type* inttype = new_prim_type("int");
    write.push_back(NULL);
    write.push_back(inttype);
    read.push_back(inttype);
    function_table["read"] = read;
    function_table["write"] = write;
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    char output_dir[100];
    for (int i = 0; i < 100; i ++) {
        output_dir[i] = '\0';
    }
    strncpy(output_dir, argv[1], strlen(argv[1])-4);
    strcat(output_dir,".ir");
    freopen(output_dir,"w",stdout);

    yyparse();
    // if(error_num == 0){
    //     pre_order_traversal(root, 0);
    // }

    translate_Program(root);
    // printOutput();

    fclose(stdout);

    return 0;
}

void printOutput(){
    for (TAC* o : output){
        printTAC(o);
    }
}

void printTAC(struct TAC* input){
    switch (input->id)
    {
    case 0:
        std::cout<<"LABEL "<<(input->target[0])<<" :"<<std::endl;
        break;
    case 1:
        std::cout<<"FUNCTION "<<(input->target[0])<<" :"<<std::endl;
        break;
    case 2:
        std::cout<<(input->target[0]) << " := " << (input->target[1]) << std::endl;
        break;
    case 3:
        std::cout << (input->target[0]) <<  " := " << (input->target[1]) << " + " << (input->target[2])<<std::endl;
        break;
    case 4:
        std::cout << (input->target[0]) <<  " := " << (input->target[1]) << " - " << (input->target[2])<<std::endl;
        break;
    case 5:
        std::cout << (input->target[0]) <<  " := " << (input->target[1]) << " * " << (input->target[2])<<std::endl;
        break;
    case 6:
        std::cout << (input->target[0]) <<  " := " << (input->target[1]) << " / " << (input->target[2])<<std::endl;
        break;
    case 7:
        std::cout << (input->target[0]) << " := " << "&" << (input->target[1]) << std::endl;
        break;
    case 8:
        std::cout << (input->target[0]) << " := " << "*" << (input->target[1]) << std::endl;
        break;
    case 9:
        std::cout << "*" << (input->target[0]) << " := " << (input->target[1]) << std::endl;
        break;
    case 10:
        std::cout << "GOTO " << (input->target[0]) << std::endl;
        break;
    case 11:
        std::cout << "IF " << (input->target[0]) << " " << (input->relop) << " " << (input->target[1]) << " GOTO " << (input->target[2]) << std::endl;
        break;
    case 12:
        std::cout << "RETURN " << (input->target[0]) << std::endl;
        break;
    case 13:
        std::cout << "DEC " << (input->target[0]) << " " << (input->size) << std::endl;
        break;
    case 14:
        std::cout << "PARAM " << (input->target[0]) << std::endl;
        break;
    case 15 :
        std::cout << "ARG " << (input->target[0]) << std::endl; 
        break;
    case 16:
        std::cout << (input->target[0]) << " := CALL " << (input->target[1]) << std::endl;
        break;
    case 17:
        std::cout << "READ " << (input->target[0]) << std::endl;
        break;
    case 18:
        std::cout << "WRITE " << (input->target[0]) << std::endl;
    default:
        break;
    }
}

void translate_Program(struct Node* root){
    sout("Program");
    translate_ExtDefList(root->child_list[0]);
}
void translate_ExtDefList(struct Node* in){
    sout("ExtDefList");
    // sout(in->child_num);
    if(in->child_num!=0){
        translate_ExtDef(in->child_list[0]);
        translate_ExtDefList(in->child_list[1]);
    }
}
void translate_ExtDef(struct Node* in){
    sout("ExtDef");
    if(strcmp(in->child_list[1]->token,"ExtDecList")==0){
        //Specifier ExtDecList SEMI # variable definition
        translate_ExtDecList(in->child_list[1]);
    }else if(in->child_num==2){
        //Specifier SEMI # structure definition
    }else{
        //Specifier FunDec CompSt  # function definition
        translate_FunDec(in->child_list[1]);
        translate_CompSt(in->child_list[2]);
    }
}
void translate_ExtDecList(struct Node* in){
    sout("ExtDecList");
    translate_VarDec(in->child_list[0],13);
    if(in->child_num==3){
        translate_ExtDecList(in->child_list[2]);
    }
}
/*
void translate_Specifier(struct Node* in){

}
void translate_StructSpecifier(struct Node* in){

}
*/
//!! haven't deal with array
//TACid=14 PARAM x
//TACid=13 DEC x
void translate_VarDec(struct Node* in, int TACid){
    sout("VarDec");
    //translate the staff in var map 
    if(in->child_num == 1){
        Node* ID = in->child_list[0];
        std::string id = ID->string_value;
        sout(id);
        if(id_address_map.count(id) == 0){
            std::string varname=new_var();
            
            Type* t=variable_table[id];
            if(TACid==14){
                // PARAM x
                sout("PARAM x");
                id_address_map[id] = varname;
                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = varname;
                code->id = TACid;
                output.push_back(code);
                printTAC(code);
            }else{
                sout("Dec x");
                // DEC x
                //put &vn into id_address_map
                id_address_map[id] = varname;
                if(t->category==ARRAY){
                    id_address_map[id] = "&"+varname;
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = varname;
                    code->id = TACid;
                    code->size=calculateSize(t);
                    output.push_back(code);
                    printTAC(code);
                }
            }
            
        }
    }else{
        translate_VarDec(in->child_list[0],TACid);
    }
}
int calculateSize(Type* array){
    int total_size=array->array->size;
    Type* next=array->array->base;
    while(next->category==ARRAY){
        total_size*=next->array->size;
        next=next->array->base;
    }
    return total_size*4;
}
void translate_FunDec(struct Node* in){
    sout("FunDec");
    //translate the staff in function map
    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
    code->target[0] = in->child_list[0]->string_value;
    code->id = 1;
    output.push_back(code);
    printTAC(code);

    if(in->child_num==4){
        //ID LP VarList RP # with parameter
        translate_VarList(in->child_list[2]);
    }else{
        //ID LP RP # without parameter
    }
}

void translate_VarList(struct Node* in){
    sout("VarList");
    translate_ParamDec(in->child_list[0]);
    if(in->child_num==3){
        translate_VarList(in->child_list[2]);
    }
}

//haven't deal with array
void translate_ParamDec(struct Node* in){
    sout("ParamDec");
    //generate PARAM x
    //give the function the parameter
    //Specifier VarDec
    struct Node* VarDec=in->child_list[1];
    translate_VarDec(VarDec,14);
}

void translate_CompSt(struct Node* in){
    sout("ComSt");
    translate_DefList(in->child_list[1]);
    translate_StmtList(in->child_list[2]);
}

void translate_StmtList(struct Node* in){
    sout("StmtList");
    if(in->child_num>0){
        translate_Stmt(in->child_list[0]);
        translate_StmtList(in->child_list[1]);
    }
}

void translate_Stmt(struct Node* in){
    sout("Stmt");
    if (in->child_num == 1) {
        translate_CompSt(in->child_list[0]);
    } else if (in->child_num== 2) {
        // Stmt -> Exp SEMI
        /**
        std::string tp = new_place();
        sout("*******");
        // sout(in->child_list[0]->token);
        for (int t = 0; t < in->child_list[0]->child_num; t ++) {
            sout(in->child_list[0]->child_list[t]->token);
        }
        sout("*******");
        // translate_Exp(in->child_list[0], tp); **/
        Node* exp = in->child_list[0];
        std::string arg;
        if (exp->child_num == 1){
            Node* child = exp->child_list[0];
            if (strcmp(child->token, "ID")==0){
                std::string id = child->string_value;
                arg = id_address_map[id];
            }else{
                std::string value = child->string_value;
                arg = new_const(value);
            }
        }else{
            arg = new_place();
            translate_Exp(in->child_list[0], arg);
        }
    } else if (in->child_num == 3) { // meaningful statements.
        // Return Exp SEMI
        std::string tp = new_place();
        translate_Exp(in->child_list[1], tp);
        TAC* return_tac = (struct TAC*) malloc(sizeof(struct TAC));
        // Modify the tac;
        return_tac->id = 12;
        return_tac->target[0] = tp;
        output.push_back(return_tac);
        printTAC(return_tac);
    } else if (in->child_num == 5) {
        // There may be two conditions
        // WHILE LP Exp RP Stmt
        // IF LP Exp RP Stmt ... (The things agter the translation is ignore)
        if (strcmp(in->child_list[0]->token, "WHILE")==0) {
            std::string lb1 = new_label();
            std::string lb2 = new_label();
            std::string lb3 = new_label();
            TAC* code1 = (struct TAC*) malloc(sizeof(struct TAC));
            TAC* code2 = (struct TAC*) malloc(sizeof(struct TAC));
            TAC* code3 = (struct TAC*) malloc(sizeof(struct TAC));
            TAC* code4 = (struct TAC*) malloc(sizeof(struct TAC));
            code1->id = 0;
            code1->target[0] = lb1;
            output.push_back(code1);
            printTAC(code1);

            translate_cond_Exp(in->child_list[2], lb2, lb3);

            code2->id = 0;
            code2->target[0] = lb2;
            output.push_back(code2);
            printTAC(code2);

            translate_Stmt(in->child_list[4]);

            code3->id = 10;
            code3->target[0] = lb1;

            code4->id = 0;
            code4->target[0] = lb3;

            output.push_back(code3);
            output.push_back(code4);
            printTAC(code3);
            printTAC(code4);

        } else if (in->child_list[0]->token, "IF") {
            std::string lb1 = new_label();
            std::string lb2 = new_label();

            translate_cond_Exp(in->child_list[2],lb1,lb2);

            TAC* code2 = (struct TAC*) malloc(sizeof(struct TAC));
            code2->id = 0;
            code2->target[0] = lb1;
            output.push_back(code2);
            printTAC(code2);
            translate_Stmt(in->child_list[4]);

            TAC* code3 = (struct TAC*) malloc(sizeof(struct TAC));
            code3->id = 0;
            code3->target[0] = lb2;
            output.push_back(code3);
            printTAC(code3);
        } else {
            // Something wrong happen since the else branch won't be access.
            // debug part.
        }
    } else if (in->child_num == 7) {
        // IF LP Exp RP Stmt ELSE Stmt
        std::string lb1 = new_label();
        std::string lb2 = new_label();
        std::string lb3 = new_label();
        TAC* code1 = (struct TAC*) malloc(sizeof(struct TAC));
        TAC* code2 = (struct TAC*) malloc(sizeof(struct TAC));
        TAC* code3 = (struct TAC*) malloc(sizeof(struct TAC));
        TAC* code4 = (struct TAC*) malloc(sizeof(struct TAC));

        translate_cond_Exp(in->child_list[2], lb1, lb2);
        code1->target[0] = lb1;
        code1->id = 0;
        output.push_back(code1);
        printTAC(code1);

        translate_Stmt(in->child_list[4]);
        code2->target[0] = lb3;
        code2->id = 10;
        output.push_back(code2);
        printTAC(code2);

        code3->target[0] = lb2;
        code3->id = 0;
        output.push_back(code3);
        printTAC(code3);

        translate_Stmt(in->child_list[6]);
        code4->id = 0;
        code4->target[0] = lb3;
        output.push_back(code4);
        printTAC(code4);
    } else {
        // Something wrong happen since the else branch won't be access.
        // debug part.
    }
}

void translate_DefList(struct Node* in){
    sout("DefList");
    if(in->child_num>0){
        translate_Def(in->child_list[0]);
        translate_DefList(in->child_list[1]);
    }
}

void translate_Def(struct Node* in){
    sout("Def");
    translate_DecList(in->child_list[1]);
}

void translate_DecList(struct Node* in){
    sout("DecList");
    translate_Dec(in->child_list[0]);
    if(in->child_num==3){
        translate_DecList(in->child_list[2]);
    }
}

void translate_Dec(struct Node* in){
    sout("Dec");
    //call vardec, assign value to address
    
    if(in->child_num==1){
        //VarDec
        translate_VarDec(in->child_list[0],13);
    }else{
        //VarDec ASSIGN Exp
        // There do not exist a statement that the vardec is an array, since function cannot return array type
        std::string varname;
        Node* VarDec=in->child_list[0];
        if(VarDec->child_num == 1){
            Node* ID = VarDec->child_list[0];
            std::string id = ID->string_value;
            if(id_address_map.count(id) == 0){
                varname=new_var();
                sout(varname);
                id_address_map[id] = varname;
                sout(id_address_map[id]);
                /*
                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                std::cout<<"Dec newvar"<<std::endl;
                code->target[0] = varname;
                code->id = 13;
                code->size=8;
                output.push_back(code);
                */
            }
        }
        //std::cout<<"vardec assign exp"<<std::endl;
        std::string t1=new_place();
        translate_Exp(in->child_list[2],t1);
        //std::cout<<"before print"<<std::endl;
        //std::cout<<varname<<t1<<std::endl;
        TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
        code->target[0]=varname;
        code->target[1]=t1;
        code->id=2;
        output.push_back(code);
        printTAC(code);

    }
}


void translate_Exp(struct Node* in, std::string place){
    sout("Exp");
    if (in->child_num == 1){
        Node* child = in->child_list[0];
        // variable access:        
        // ID (declared) 
        if (strcmp(child->token, "ID")==0){
            sout(place);
            sout("id");
            std::string id = child->string_value;
            std::string var = id_address_map[id];

            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = place;
            code->target[1] = var;
            code->id = 2;

            output.push_back(code);
            printTAC(code);
        }
        // constant access:
        // INT | FLOAT | CHAR 
        else{
            sout("const");
            sout(child->token);
            sout(child->child_num);
            if(child->child_num!=0){
                sout(child->child_list[0]->string_value);
            }
            std::string value = child->string_value;
            std::string new_value = new_const(value);

            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = place;
            code->target[1] = new_value;
            code->id = 2;

            output.push_back(code);
            printTAC(code);
        }
    }
    // function access:
    // ID LP Args RP (s.c. write(x)) | ID LP RP (s.c. read())  
    else if (strcmp(in->child_list[0]->token, "ID")==0){
        Node* ID = in->child_list[0];
        if (in->child_num == 4){
            if(strcmp(ID->string_value, "write")==0){
                translate_Exp(in->child_list[2]->child_list[0], place);

                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = place;
                code->id = 18;

                output.push_back(code);
                printTAC(code);
            }else{
                std::string fun = ID->string_value;
                args.clear();

                translate_Args(in->child_list[2]);
                for (int i = 0; i < args.size(); i++){
                    std::string place_i = args[i];

                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place_i;
                    code->id = 15;

                    output.push_back(code);
                    printTAC(code);
                }

                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = place;
                code->target[1] = fun;
                code->id = 16;

                output.push_back(code);
                printTAC(code);
            }
        }else{
            if(strcmp(ID->string_value, "read")==0){
                sout("read");
                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = place;
                code->id = 17;

                output.push_back(code);
                printTAC(code);
            }else{
                std::string fun = ID->string_value;

                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = place;
                code->target[1] = fun;
                code->id = 16;

                output.push_back(code);
                printTAC(code);
            }
        }
    } 
    // MINUS Exp
    else if (in->child_num == 2){
        std::string tp = new_place();
        translate_Exp(in->child_list[1], tp);

        TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
        code->target[0] = place;
        code->target[1] = "#0";
        code->target[2] = tp;
        code->id = 4;

        output.push_back(code);
        printTAC(code);
    } 
    // array access:
    // Exp LB Exp RB 
    else if (in->child_num == 4){
        sout("array access");
        indexes.clear();
        Node* Exp = in;
        while (Exp->child_num == 4){
            Node* index = Exp->child_list[2];
            std::string idx;
            if (index->child_num == 1){
                Node* child = index->child_list[0];
                if (strcmp(child->token, "ID")==0){
                    std::string id = child->string_value;
                    idx = id_address_map[id];
                }else{
                    std::string value = child->string_value;
                    idx = new_const(value);
                }                
            }else{
                idx = new_place();
                translate_Exp(index->child_list[0], idx);
            }
            indexes.push_back(idx);
            Exp = Exp->child_list[0];
        }

        std::string id = Exp->child_list[0]->string_value;
        std::string t0 = id_address_map[id];
        sout(id);
        sout(t0);

        Type* arr = variable_table[id];
        sizes.clear();
        while (arr->category == ARRAY){
            sizes.insert(sizes.begin(), arr->array->size);
            arr = arr->array->base;
        }
        int base_size = arr->primitive == _CHAR ? 1 : 4;
        sout(sizes.size());
        sout(indexes.size());
        sout(base_size);

        std::string t1 = new_place();
        std::string new_cst = new_const(base_size);
        TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
        code->target[0] = t1;
        code->target[1] = indexes[0];
        code->target[2] = new_cst;
        code->id = 5;
        output.push_back(code);
        printTAC(code);
        base_size *= sizes[0];

        std::string offset = t1;
        if (indexes.size() > 1){
            std::string t2 = new_place();
            std::string new_cst = new_const(base_size);
            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = t2;
            code->target[1] = indexes[1];
            code->target[2] = new_cst;
            code->id = 5;
            output.push_back(code);
            printTAC(code);
            base_size *= sizes[1];

            std::string t_sum = new_place();
            code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = t_sum;
            code->target[1] = t1;
            code->target[2] = t2;
            code->id = 3;
            output.push_back(code);
            printTAC(code);

            for (int i = 2; i < indexes.size(); i++){
                std::string t_new = new_place();
                std::string new_cst = new_const(base_size);
                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = t_new;
                code->target[1] = indexes[i];
                code->target[2] = new_cst;
                code->id = 5;
                output.push_back(code);
                printTAC(code);
                base_size *= sizes[i];

                std::string t_pre = t_sum;
                std::string t_sum = new_place();
                code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = t_sum;
                code->target[1] = t_new;
                code->target[2] = t_pre;
                code->id = 3;
                output.push_back(code);
                printTAC(code);
            }
            
            offset = t_sum;
        }

        std::string addr = new_place();
        code = (struct TAC*) malloc(sizeof(struct TAC));
        code->target[0] = addr;
        code->target[1] = t0;
        code->target[2] = offset;
        code->id = 3;
        output.push_back(code);
        printTAC(code);

        code = (struct TAC*) malloc(sizeof(struct TAC));
        code->target[0] = place;
        code->target[1] = addr;
        code->id = 8;
        output.push_back(code);
        printTAC(code);

    } else if (in->child_num == 3){
        // TODO:
        // structure access:
        // Exp DOT ID  

        // LP Exp RP 
        if (strcmp(in->child_list[0]->token, "LP")==0){
            translate_Exp(in->child_list[1], place);
        } 
        // assignnment:
        // Exp ASSIGN Exp          

        // arithmetic operator:           
        // Exp PLUS Exp | Exp MINUS Exp | Exp MUL Exp | Exp DIV Exp
        else {
        
            Node* Exp1 = in->child_list[0];
            Node* Opt = in->child_list[1];
            Node* Exp2 = in->child_list[2];

            if (strcmp(Opt->token, "ASSIGN")==0){  
                sout("assign"); 
                Node* ID = Exp1->child_list[0];
                std::string left;
                if (Exp1->child_num == 1){
                    std::string id = ID->string_value;
                    left = id_address_map[id];
                    sout(id);
                    sout(id_address_map.count(id));
                }else if (Exp1->child_num == 4){
                    indexes.clear();
                    Node* Exp = Exp1;
                    while (Exp->child_num == 4){
                        Node* index = Exp->child_list[2];
                        std::string idx;
                        if (index->child_num == 1){
                            Node* child = index->child_list[0];
                            if (strcmp(child->token, "ID")==0){
                                std::string id = child->string_value;
                                idx = id_address_map[id];
                            }else{
                                std::string value = child->string_value;
                                idx = new_const(value);
                            }                
                        }else{
                            idx = new_place();
                            translate_Exp(index->child_list[0], idx);
                        }
                        indexes.push_back(idx);
                        Exp = Exp->child_list[0];
                    }

                    std::string id = Exp->child_list[0]->string_value;
                    std::string t0 = id_address_map[id];

                    Type* arr = variable_table[id];
                    sizes.clear();
                    while (arr->category == ARRAY){
                        sizes.insert(sizes.begin(), arr->array->size);
                        arr = arr->array->base;
                    }
                    int base_size = arr->primitive == _CHAR ? 1 : 4;

                    std::string t1 = new_place();
                    std::string new_cst = new_const(base_size);
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = t1;
                    code->target[1] = indexes[0];
                    code->target[2] = new_cst;
                    code->id = 5;
                    output.push_back(code);
                    printTAC(code);
                    base_size *= sizes[0];

                    std::string offset = t1;
                    if (indexes.size() > 1){
                        std::string t2 = new_place();
                         std::string new_cst = new_const(base_size);
                        TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                        code->target[0] = t2;
                        code->target[1] = indexes[1];
                        code->target[2] = new_cst;
                        code->id = 5;
                        output.push_back(code);
                        printTAC(code);
                        base_size *= sizes[1];

                        std::string t_sum = new_place();
                        code = (struct TAC*) malloc(sizeof(struct TAC));
                        code->target[0] = t_sum;
                        code->target[1] = t1;
                        code->target[2] = t2;
                        code->id = 3;
                        output.push_back(code);
                        printTAC(code);

                        for (int i = 2; i < indexes.size(); i++){
                            std::string t_new = new_place();
                            std::string new_cst = new_const(base_size);
                            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                            code->target[0] = t_new;
                            code->target[1] = indexes[i];
                            code->target[2] = new_cst;
                            code->id = 5;
                            output.push_back(code);
                            printTAC(code);
                            base_size *= sizes[i];

                            std::string t_pre = t_sum;
                            std::string t_sum = new_place();
                            code = (struct TAC*) malloc(sizeof(struct TAC));
                            code->target[0] = t_sum;
                            code->target[1] = t_new;
                            code->target[2] = t_pre;
                            code->id = 3;
                            output.push_back(code);
                            printTAC(code);
                        }
                        
                        offset = t_sum;
                    }

                    std::string addr = new_place();
                    code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = addr;
                    code->target[1] = t0;
                    code->target[2] = offset;
                    code->id = 3;
                    output.push_back(code);
                    printTAC(code);

                    left = retrive_value(addr);
                }
                sout(left);

                std::string tp = new_place();
                translate_Exp(Exp2, tp);

                TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = left;
                code->target[1] = tp;
                code->id = 2;
                output.push_back(code);
                printTAC(code);

                code = (struct TAC*) malloc(sizeof(struct TAC));
                code->target[0] = place;
                code->target[1] = left;
                code->id = 2;
                output.push_back(code);
                printTAC(code);

            } else {
                std::string t1 = new_place();
                std::string t2 = new_place();

                translate_Exp(Exp1, t1);
                translate_Exp(Exp2, t2);
                if (strcmp(Opt->token, "PLUS")==0){
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = t1;
                    code->target[2] = t2;
                    code->id = 3;
                    output.push_back(code);
                    printTAC(code);

                }else if (strcmp(Opt->token, "MINUS")==0){
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = t1;
                    code->target[2] = t2;
                    code->id = 4;
                    output.push_back(code);
                    printTAC(code);
                
                }else if (strcmp(Opt->token, "MUL")==0){
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = t1;
                    code->target[2] = t2;
                    code->id = 5;
                    output.push_back(code);
                    printTAC(code);
                    
                }else if (strcmp(Opt->token, "DIV")==0){
                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = t1;
                    code->target[2] = t2;
                    code->id = 6;
                    output.push_back(code);
                    printTAC(code);

                }else{
                    // condition Exp
                    std::string lb1 = new_label();
                    std::string lb2 = new_label();

                    TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = "#0";
                    code->id = 2;
                    output.push_back(code);
                    printTAC(code);

                    translate_cond_Exp(in, lb1, lb2);

                    code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = lb1;
                    code->id = 0;
                    output.push_back(code);
                    printTAC(code);

                    code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = place;
                    code->target[1] = "#1";
                    code->id = 2;
                    output.push_back(code);
                    printTAC(code);

                    code = (struct TAC*) malloc(sizeof(struct TAC));
                    code->target[0] = lb2;
                    code->id = 0;
                    output.push_back(code);
                    printTAC(code);
                }
            }
        }
    }
}

void translate_cond_Exp(Node* in, std::string lb_t, std::string lb_f){
    // NOT Exp 
    sout("cond_Exp");
    sout(in->child_num);
    if (in->child_num == 2){
        translate_cond_Exp(in, lb_f, lb_t);
    }else{
        // logic operators:    
        // Exp AND Exp | Exp OR Exp  
        // relation operators:
        // Exp LT Exp | Exp LE Exp | Exp GT Exp 
        // Exp GE Exp | Exp NE Exp | Exp EQ Exp
        Node* Exp1 = in->child_list[0];
        Node* Opt = in->child_list[1];
        Node* Exp2 = in->child_list[2];
        if (strcmp(Opt->token, "AND")==0){
            std::string lb1 = new_label();
            translate_cond_Exp(Exp1, lb1, lb_f);

            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = lb1;
            code->id = 0;
            output.push_back(code);
            printTAC(code);

            translate_cond_Exp(Exp2, lb_t, lb_f);
        }else if (strcmp(Opt->token, "OR")==0){
            std::string lb1 = new_label();
            translate_cond_Exp(Exp1, lb_t, lb1);

            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = lb1;
            code->id = 0;
            output.push_back(code);
            printTAC(code);

            translate_cond_Exp(Exp2, lb_t, lb_f);
        }else {
            std::string t1 = new_place();
            std::string t2 = new_place();
            translate_Exp(Exp1, t1);
            translate_Exp(Exp2, t2);
            TAC* code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = t1;
            code->target[1] = t2;
            code->target[2] = lb_t;
            code->id = 11;
          
            if (strcmp(Opt->token, "LT")==0){
                code->relop = "<";
            }else if (strcmp(Opt->token, "LE")==0){
                code->relop = "<=";
            }else if (strcmp(Opt->token, "GT")==0){
                code->relop = ">";
            }else if (strcmp(Opt->token, "GE")==0){
                code->relop = ">=";
            }else if (strcmp(Opt->token, "EQ")==0){
                code->relop = "==";
            }else if (strcmp(Opt->token, "NE")==0){
                code->relop = "!=";
            }
            output.push_back(code);
            printTAC(code);

            code = (struct TAC*) malloc(sizeof(struct TAC));
            code->target[0] = lb_f;
            code->id = 10;
            output.push_back(code);
            printTAC(code);
        }
    }
}

void translate_Args(struct Node* in){
    sout("Args");
    Node* Exp = in->child_list[0];
    std::string arg;
    if (Exp->child_num == 1){
        Node* child = Exp->child_list[0];
        if (strcmp(child->token, "ID")==0){
            std::string id = child->string_value;
            arg = id_address_map[id];
        }else{
            std::string value = child->string_value;
            arg = new_const(value);
        }
    }else{
        arg = new_place();
        translate_Exp(in->child_list[0], arg);
    }
    args.insert(args.begin(), arg);

    if(in->child_num > 1){
        translate_Args(in->child_list[2]);
    }
}

std::string new_place(){
    std::stringstream sstream;
    std::string result;

    place_num++;
    sstream << "t" << place_num;
    sstream >> result;

    return result;
}

std::string new_var(){
    std::stringstream sstream;
    std::string result;

    var_num++;
    sstream << "v" << var_num;
    sstream >> result;

    return result;
}

std::string new_label(){
    std::stringstream sstream;
    std::string result;

    label_num++;
    sstream << "label" << label_num;
    sstream >> result;

    return result;
}

template <typename T>
std::string new_const(T c_value){
    std::stringstream sstream;
    std::string result;
    sstream << "#" << c_value;
    sstream >> result;

    return result;
}

std::string retrive_value(std::string addr){
    std::stringstream sstream;
    std::string result;
    sstream << "*" << addr;
    sstream >> result;

    return result;
}