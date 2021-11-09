#include <map>
#include <string>
#include <vector>
#include <iostream>

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
    void invokeFun(Node* ID, Node* args, int line){
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

private:
    static std::map<std::string, Type*> variable_table;
    static std::map<std::string, std::vector<Type*>> function_table;
    static std::map<std::string, Type*>struct_table;

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
