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
    // There are some error in this function, so I comment it for safety. This can be check soon.
    void defVar(Node* specifier,Node* ExtDecList,int line){
        //Type* vartype=specifierNodeType(specifier);
        std::stack<Node*> namestack = namestack;
        namestack.push(ExtDecList);
        while(!namestack.empty()){
            // Node* iter = namestack.pop();
            Node *iter;
            if(strcmp(iter->token, "ExtDecList") == 0||strcmp(iter->token, "DecList") == 0){
                for(int i=0;i<iter->child_num;i++){
                    Node* n=iter->child_list[i];
                    if(strcmp(n->token, "COMMA") != 0){
                        namestack.push(n);
                    }
                }
            }else if(strcmp(iter->token, "VarDec") == 0){
                // int childNum = varDec->child_num;
                int childNum;
                // none array type
                if (childNum == 1){
                    Type* type = specifierNodeType(specifier);
                    Node* id = iter->child_list[0];
                    variable_table[id->string_value]=type;
                // VarDec -> VarDec LB INT RB | ID
                }else{
                    Type* type = specifierNodeType(specifier);
                    Node* varDec=iter;
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
                    // variable_table[VarDec->string_value]=type;
                }
            }else{
                //the token is 'Dec'
                namestack.push(iter->child_list[0]);
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

/***
 * struct myStruct {
    float val;
    struct _ {char c[2]; } charr;
    int arr2[3][2];
};

 ExtDef (1)
    ->Specifier (1)
        StructSpecifier (1)
*/
    void defStructure(Node* node, int line) {
        char* tname = node->child_list[0]->child_list[1]->string_value;
        if (searchStructType(tname) != NULL) {
            print15TypeError(line);
        } else {
            // define a new structure
            FieldList* field_list = generateFieldList(node->child_list[0]->child_list[3]);
            struct Type* type = (struct Type*) malloc(sizeof(struct Type));
            type->category = Type::STRUCTURE;
            strcpy(type->name, tname);
            type->structure = field_list;
            struct_table[tname] = type;
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

    void print05TypeError(int line) {
        // Something wrong happen, when "unmathing types appear at both sides of the assignment operator (=)".
    }

    void print10TypeError(int line) {
        // Something wrong happen, when "applying index operator ([...]) on non-array type variables".
    }

    void print13TypeError(int line) {
        // Something wrong happen, when "accessing members of a non-structure type expression".
    }

    void print14TypeError(int line) {
        // Something wrong happen, when "accessing an undefined structure member".        
    }

    void print15TypeError(int line) {
        // Something wrong happen, when "redefine the same structure type".        
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
        return false;
    }

/***
 *  LC
      ->DefList (2)
            Def (2)
 * */
    FieldList* generateFieldList(Node* node) {
        if(node->child_num <= 4) {
            return nullptr;
            // This is an empty structure, just return from the function.
        } else {
            std::vector<FieldList*> field_list_v;
            Node *field_node = node->child_list[3];
            field_list_v.push_back(getFieldListFromNode(field_node));
            field_node = field_node->child_list[1];
            while (field_node != nullptr && strcmp(field_node->string_value, "DefList") == 0) {
                field_list_v.push_back(getFieldListFromNode(field_node));
                if (field_node->child_num <= 1) {
                    break;
                }
                field_node = field_node->child_list[1];
            }
            for (int i = 0; i < field_list_v.size() - 1; i ++) {
                field_list_v[i]->next = field_list_v[i+1];
                return field_list_v.front();
            }
        }
    }
/***
 * LC
      ->DefList (2)
            Def (2)
*/
    FieldList* getFieldListFromNode(Node *node) { // From The DefList Node!
        Node *base_node = node->child_list[0];
        FieldList* res;
        res->next = nullptr;
        strcpy(res->name, getNameFromDecList(base_node->child_list[1]));
        // The base node is the node of Def
        if (base_node->child_list[0]->child_num==0) {
            // It means that it is shown like TYPE:float
            // This is the type of Array or prim type.
            res->type = getArrayOrPrimTypeFromDecList(base_node->child_list[1]);
        } else {
            // It is a struct type, just search it in the struct table, since if it exist, if must be in it.
            std::string s_name = base_node->child_list[0]->child_list[0]->child_list[0]->string_value;
            res->type = searchStructType(s_name);
        }
        return res;
    }

    Type* getArrayOrPrimTypeFromDecList(Node* node) { // From DecList node get the type of the var, it may be prim type or array type
        if (node->child_list[0]->child_num == 0 && node->child_list[0]->token=="TYPE") {
            // This is a prim type, just return it.
            return new_prim_type(node->child_list[0]->string_value);
        } else {
            // This is a array type. Create it.
            return createArrayType(node->child_list[1]->child_list[0], new_prim_type(node->child_list[0]->string_value));
        }
    }

    char* getNameFromDecList(Node* node) { // From DecList node get the id of the type.
        // node is a DecList
        while (node->child_list != 0) {
            node = node->child_list[0];
        }
        return node->string_value;
    }

    Type* createArrayType(Node* node, Type* tp) { // From the node Dec get the Array Type.
        Array* arr;
        generateArr(node, arr, tp);
    }

    void generateArr(Node* node, Array* arr, Type* tot_tp) {
        if (node->child_list[0]->child_num == 0) {
            // It is the 'getting out recusive' state.
            arr->size = node->child_list[2]->int_value;
            arr->base = tot_tp;
        } else {
            arr->size = node->child_list[2]->int_value;
            Type* tp;
            tp->category = Type::ARRAY;
            Array* new_arr;
            tp->array = new_arr;
            generateArr(node->child_list[0], new_arr, tot_tp);
        }
    }
};
