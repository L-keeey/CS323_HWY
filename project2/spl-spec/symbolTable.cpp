#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stack>

#include "parseTree.h"

class symbolTable{
public:
    symbolTable() {};

    Type* searchVariable(std::string varName){
        std::map<std::string, Type*>::iterator iter;
        iter = variable_table.find(varName);
        if (iter != variable_table.end())
            return iter->second;
        else
            return NULL;
    }

    std::vector<Type*>* searchFunction(std::string funName){
        std::map<std::string, std::vector<Type*>>::iterator iter;
        iter = function_table.find(funName);
        if (iter != function_table.end())
            return &iter->second;
        else
            return NULL;
    }

    Type* searchStructType(std::string typeName){
        std::map<std::string, Type*>::iterator iter;
        iter = struct_table.find(typeName);
        if (iter != struct_table.end())
            return iter->second;
        else
            return NULL;
    }

    //ExtDef->Specifier ExtDecList SEMI
    //Def-> Specifier DecList SEMI
    //check if the defined var exist
    void defVar(Node* specifier,Node* ExtDecList,int line){
        //Type* vartype=specifierNodeType(specifier);
        std::stack<Node*> namestack;
        namestack.push(ExtDecList);
        while(!namestack.empty()){
            Node* iter=namestack.top();
            namestack.pop();
            if(strcmp(iter->token, "ExtDecList") == 0||strcmp(iter->token, "DecList") == 0){
                for(int i=0;i<iter->child_num;i++){
                    Node* n=iter->child_list[i];
                    if(strcmp(n->token, "COMMA") != 0){
                        namestack.push(n);
                    }
                }
            }else if(strcmp(iter->token, "VarDec") == 0){
                int childNum = iter->child_num;
                // none array type
                if (childNum == 1){
                    Type* type = specifierNodeType(specifier);
                    Node* id = iter->child_list[0];
                    if(variable_table.count(id->string_value)>0){
                        printType3Error(line);
                    }else{
                        variable_table[id->string_value]=type;
                    }
                // VarDec -> VarDec LB INT RB | ID
                }else{
                    Type* type = specifierNodeType(specifier);
                    Node* VarDec=iter;
                    while(childNum == 3){
                        int size = VarDec->child_list[2]->int_value;

                        Array* a = (struct Array*) malloc(sizeof(struct Array));
                        a->base = type;
                        a->size = size;

                        Type* array = (struct Type*) malloc(sizeof(struct Type));
                        array->category = array->ARRAY;
                        array->array = a;
                        
                        type = array;
                        VarDec = VarDec->child_list[0];
                        childNum = VarDec->child_num;
                    }
                    if(variable_table.count(VarDec->string_value)>0){
                        printType3Error(line);
                    }else{
                        variable_table[VarDec->string_value]=type;
                    }
                }
            }else{
                //the token is 'Dec'
                if(iter->child_num==1){
                    namestack.push(iter->child_list[0]);
                }else{
                    //Vardec ASSIGN Exp
                    Node* exp=iter->child_list[3];
                    iter=iter->child_list[0];
                    int childNum = iter->child_num;
                    //this part is copy from above
                    // none array type
                    if (childNum == 1){
                        Type* type = specifierNodeType(specifier);
                        Node* id = iter->child_list[0];
                        if(variable_table.count(id->string_value)>0){
                            printType3Error(line);
                        }else if(isSameTypes(exp->type_value,type)){
                            //ASSIGN with not equal type
                            printType5Error(line);
                        }else{
                            variable_table[id->string_value]=type;
                        }
                    // VarDec -> VarDec LB INT RB | ID
                    }else{
                        Type* type = specifierNodeType(specifier);
                        Node* VarDec=iter;
                        while(childNum == 3){
                            int size = VarDec->child_list[2]->int_value;

                            Array* a = (struct Array*) malloc(sizeof(struct Array));
                            a->base = type;
                            a->size = size;

                            Type* array = (struct Type*) malloc(sizeof(struct Type));
                            array->category = array->ARRAY;
                            array->array = a;
                            
                            type = array;
                            VarDec = VarDec->child_list[0];
                            childNum = VarDec->child_num;
                        }
                        if(variable_table.count(VarDec->string_value)>0){
                            printType3Error(line);
                        }else if(isSameTypes(exp->type_value,type)){
                            //ASSIGN with not equal type
                            printType5Error(line);
                        }else{
                            variable_table[VarDec->string_value]=type;
                        }
                }

                }
                
            }
        }
    }

    // ExtDef -> Specifier FunDec CompSt
    void defFun(Node* specifier, Node* funDec, Node* compSt,int line){
        // fetch return type
        std::vector<Type*> types;
        types.push_back(specifierNodeType(specifier));

        // check return type
        checkReturnType(types[0], compSt, line);

        // fetch function name as key
        // FunDec -> ID LP VarList RP | ID LP RP  
        std::string funName = funDec->child_list[0]->string_value;
        if (searchFunction(funName) !=  NULL){
            printType4Error(line);
            return;
        }

        // fetch parameters type
        int childNum = funDec->child_num;
        // FunDec -> ID LP VarList RP
        if (childNum == 4){
            Node* varList = funDec->child_list[2];
            childNum = varList->child_num;
            Node* paramDec;

            // VarList -> ParamDec COMMA VarList
            while (childNum == 3){
                paramDec = varList->child_list[0];
                types.push_back(paramDecNodeType(paramDec));

                varList = varList->child_list[2];
                childNum = varList->child_num;
            }
            // VarList -> ParamDec
            paramDec = varList->child_list[0];
            types.push_back(paramDecNodeType(paramDec));
        }

        function_table[funName] = types;
    }

    // Exp -> ID LP Args RP
    // Exp -> ID LP RP 
    void invokeFun(Node* exp, Node* ID, Node* args, int line){
        std::string funName = ID->string_value;
        std::vector<Type*>* typesPtr = searchFunction(funName);
        bool noFun = typesPtr == NULL;
        bool noVar = searchVariable(funName) == NULL;
        if (noFun && noVar)
            printType2Error(line);
        else if(noFun && !noVar)
            printType11Error(line);
        else{
            std::vector<Type*> funTypes = *typesPtr;
            exp->type_value = funTypes[0];
            
            int paramSize = funTypes.size()-1;
            if (args == NULL && paramSize > 0)
                printType9Error(line);                
            else{
                int index = 1;

                //Args -> Exp COMMA Args | Exp
                int childNum = args->child_num;
                while (childNum == 3){
                    Node* exp = args->child_list[0];
                    Type* type = exp->type_value;
                    
                    // args more that defined params
                    if (index > paramSize){
                        printType8Error(line);
                        return;
                    }else{
                        Type* define = funTypes[index++];
                        if (isSameTypes(type, define)){
                            printType8Error(line);
                            return;
                        }
                    }
            
                }

                Node* exp = args->child_list[0];
                Type* type = exp->type_value;
                Type* define = funTypes[index++];
                if (index > paramSize){
                    printType8Error(line);
                    return;
                }else{
                    Type* define = funTypes[index++];
                    if (isSameTypes(type, define)){
                        printType8Error(line);
                        return;
                    }
                }
   
                if (index <= paramSize)
                    printType9Error(line); 
            }
        }
    }

    Type* findID(std::string name,int line){
        if(variable_table.count(name)>0){
            return variable_table[name];
        }
        printType1Error(line);
        return NULL;
    }

    void check_rvalue(Node* exp,int line){
        if(exp->child_num==1){
            if(strcmp(exp->child_list[0]->token,"ID")==0){
                if(findID(exp->child_list[0]->string_value,line)!=NULL){
                    return;
                }
            }
        }else if(exp->child_num==3){
            if(strcmp(exp->child_list[0]->token,"Exp")==0&&strcmp(exp->child_list[1]->token,"DOT")==0&&strcmp(exp->child_list[2]->token,"ID")==0){
                return;
            }else if(strcmp(exp->child_list[0]->token,"LP")==0&&strcmp(exp->child_list[1]->token,"Exp")==0&&strcmp(exp->child_list[2]->token,"RP")==0){
                check_rvalue(exp->child_list[1],line);
            }
        }else if(exp->child_num==4){
            if(strcmp(exp->child_list[0]->token,"Exp")==0&&strcmp(exp->child_list[1]->token,"LB")==0&&strcmp(exp->child_list[2]->token,"Exp")==0&&strcmp(exp->child_list[3]->token,"RB")==0){
                return;
            }
        }
        printType6Error(line);
    }

    Type* typeAfterCalc(Node* exp1,Node* exp2,int line){
        //deal with null
        if(exp1->type_value==NULL||exp2->type_value==NULL){
            printType7Error(line);
            return NULL;
        }
        if(exp1->type_value->primitive==exp1->type_value->FLOAT&&(exp2->type_value->primitive==exp2->type_value->INT||exp2->type_value->primitive==exp2->type_value->FLOAT)){
            return new_prim_type("float");
        }else if(exp1->type_value->primitive==exp1->type_value->INT&&exp2->type_value->primitive==exp2->type_value->FLOAT){
            return new_prim_type("float");
        }else if(exp1->type_value->primitive==exp1->type_value->INT&&exp2->type_value->primitive==exp2->type_value->INT){
            return new_prim_type("int");
        }else{
            printType7Error(line);
            return NULL;
        }

    }

    //do "and" and "or" allow float value?
    //return 1 if the expression have int value
    int checkINTexp(Node* exp){
        if(exp->type_value->primitive==exp->type_value->INT){
            return 1;
        }else{
            return 0;
        }
    } 

private:
    static std::map<std::string, Type*> variable_table;
    static std::map<std::string, std::vector<Type*>> function_table;
    static std::map<std::string, Type*>struct_table;

    void printType1Error(int line){
        std::cout << "Error type 1 at Line " << line << ": variable is used without a definition." << std::endl;
    }

    void printType3Error(int line){
        std::cout << "Error type 3 at Line " << line << ": variable is redefined." << std::endl;
    }

    void printType6Error(int line){
        std::cout << "Error type 6 at Line " << line << ": rvalue appears on the left-hand side of the assignment operator." << std::endl;
    }

    void printType7Error(int line){
        std::cout << "Error type 7 at Line " << line << ": unmantching operands,such as adding an integer to a structure variable" << std::endl;
    }

    void printType12Error(int line){
        std::cout << "Error type 12 at Line " << line << ": array indexing with a non-integer type expression." << std::endl;
    }    

    void printType2Error(int line){
        std::cout << "Error type 2 at Line " << line << ": function should be invoked with a definition." << std::endl;
    }

    void printType4Error(int line){
        std::cout << "Error type 4 at Line " << line << ": function should only be defined once." << std::endl;
    }

    void printType8Error(int line){
        std::cout << "Error type 8 at Line " << line << ": function return type and the declared type mismatch." << std::endl;
    }

    void printType9Error(int line){
        std::cout << "Error type 9 at Line " << line << ": function argument type and the declared type mismatch." << std::endl;
    }

    void printType11Error(int line){
        std::cout << "Error type 11 at Line " << line << ": non-function variable could not apply function invocation." << std::endl;
    }    

     void printType5Error(int line){
        std::cout << "Error type 5 at Line " << line << ": the type of the operands at both sides of assignment operator should match." << std::endl;
    }    

    Type* specifierNodeType(Node* specifier){
        Node* child = specifier->child_list[0];
        // Specifier -> Type
        if (strcmp(child->token, "TYPE") == 0){
            char* type = child->string_value;
            Type* prim = new_prim_type(type);
            return prim;
        // Specifier -> StructSpecifier
        }else{
            // StructSpecifier -> STRUCT ID
            Node* ID = child->child_list[1];
            std::string structName = ID->string_value;
            return searchStructType(structName);
        }   
    }

    Type* paramDecNodeType(Node* paramDec){
        // ParamDec -> Specifier VarDec 
        Node* specifier = paramDec->child_list[0];
        Node* varDec = paramDec->child_list[1];

        int childNum = varDec->child_num;
        // none array type
        if (childNum == 1)
            return specifierNodeType(specifier);
        // VarDec -> VarDec LB INT RB | ID
        else{
            Type* type = specifierNodeType(specifier);

            while(childNum == 3){
                int size = varDec->child_list[2]->int_value;

                Array* a = (struct Array*) malloc(sizeof(struct Array));
                a->base = type;
                a->size = size;

                Type* array = (struct Type*) malloc(sizeof(struct Type));
                array->category = array->ARRAY;
                array->array = a;
                
                type = array;
                varDec = varDec->child_list[0];
                childNum = varDec->child_num;
            }

            return type;
        }
        
    }

    void checkReturnType(Type* define, Node* compSt, int line){
        std::vector<Node*> stack;

        // CompSt -> LC DefList StmtList RC
        Node* stmtList = compSt->child_list[2];
        int childNum = stmtList->child_num;

        // StmtList: empty | Stmt StmtList
        while (childNum == 2){
            Node* stmt = stmtList->child_list[0];
            stmtList = stmtList->child_list[1];
            childNum = stmtList->child_num;

            // Stmt -> CompSt
            if (stmt->child_num == 1){
                stack.push_back(stmtList);
                stmtList = stmt->child_list[0]->child_list[2];
                childNum = stmtList->child_num;
            }

            // Stmt -> RETURN Exp SEMI    
            if (stmt->child_num == 3){
                Node* exp = stmt->child_list[1];
                Type* type = exp->type_value;
                if (!isSameTypes(define, type)){
                    printType8Error(line);
                    return;
                }else if (!stack.empty()){
                    stmtList = stack.back();
                    childNum = stmtList->child_num;
                    stack.pop_back();
                }else 
                    break;                
            }

        }
    }

    bool isSameTypes(Type* type1, Type* type2){
        // TODO:

        return true;
    }
};
