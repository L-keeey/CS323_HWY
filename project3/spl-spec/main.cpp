#include "syntax.tab.c"

struct TAC
{
    int id;
    union{
        std::string relop;
        int size;
    };
    std::string target[3];
};

std::vector<TAC> output;
std::map<std::string,std::string> id_address_map;
int main(int argc, char **argv){
    std::vector<Type*> read, write;
    Type* inttype = new_prim_type("int");
    write.push_back(NULL);
    write.push_back(inttype);
    read.push_back(inttype);
    read.push_back(NULL);
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
    strcat(output_dir,".out");
    freopen(output_dir,"w",stdout);

    yyparse();

    if(error_num == 0){
        pre_order_traversal(root, 0);
    }
    fclose(stdout);
    return 0;
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
    translate_ExtDefList(root->child_list[0]);
}
void translate_ExtDefList(struct Node* in){
    if(in->child_num!=0){
        translate_ExtDef(in->child_list[0]);
        translate_ExtDefList(in->child_list[1]);
    }
}
void translate_ExtDef(struct Node* in){

}
void translate_ExtDecList(struct Node* in){
    translate_VarDec(in->child_list[0]);
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
void translate_VarDec(struct Node* in){
//translate the staff in var map 
}
void translate_FunDec(struct Node* in){
//translate the staff in function map
}
void translate_VarList(struct Node* in){
    translate_ParamDec(in->child_list[0]);
    if(in->child_num==3){
        translate_VarList(in->child_list[2]);
    }
}
void translate_ParamDec(struct Node* in){
//generate PARAM x
}
void translate_CompSt(struct Node* in){
    translate_DefList(in->child_list[1]);
    translate_StmtList(in->child_list[2]);
}
void translate_StmtList(struct Node* in){
    if(in->child_num>0){
        translate_Stmt(in->child_list[0]);
        translate_StmtList(in->child_list[1]);
    }
}
void translate_Stmt(struct Node* in){
//copy && add
}
void translate_DefList(struct Node* in){
    if(in->child_num>0){
        translate_Def(in->child_list[0]);
        translate_DefList(in->child_list[1]);
    }
}
void translate_Def(struct Node* in){
    translate_DecList(in->child_list[1]);
}
void translate_DecList(struct Node* in){
    translate_Dec(in->child_list[0]);
    if(in->child_num==3){
        translate_DecList(in->child_list[2]);
    }
}
void translate_Dec(struct Node* in){
//call vardec, assign value to address
}
void translate_Exp(struct Node* in){
//copy && add
}
void translate_Args(struct Node* in){
    //should add ARG x
    translate_Exp(in->child_list[0]);
    if(in->child_num==0){
        translate_Args(in->child_list[2]);
    }
}