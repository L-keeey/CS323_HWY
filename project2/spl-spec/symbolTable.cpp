#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <stack>

#include "parseTree.h"

#define MOD 1000000007;
#define INT_BASE 3;
#define ARRAY_PROM 409
#define FLOAT_BASE 4001;
#define STRU_PROM 80207;
#define CHAR_BASE 1560217;
typedef long long ll;

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
            printType15Error(line);
        } else {
            // define a new structure
            FieldList* field_list = generateFieldList(node->child_list[0]->child_list[3]);
            struct Type* type = (struct Type*) malloc(sizeof(struct Type));
            type->category = Type::STRUCTURE;
            strcpy(type->name, tname);
            type->structure = field_list;
            struct_table[tname] = type;
            calStructHash(tname, type);
        }
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
        Type* out=searchVariable(name);
        if(out==NULL){
            printType1Error(line);
        }
        return out;
    }

    void checkAssignOperand(Node* exp1, Node* exp2, int line){
        if (!isSameTypes(exp1->type_value, exp2->type_value))
            printType5Error(line);        
    }

    void determineExpType(Node* expLeft, Node* exp1, int line){
        Type* expType = exp1->type_value;
        if(expType->category == expType->ARRAY){
            expLeft->type_value = expType->array->base;
        }else
            printType10Error(line);
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

    //  Exp <- Exp DOT ID
    Type* checkStructMember(Node* exp, Node* id, int line){
        //TODO: 1. return the type of the structure member
        //2. throw type 13 error when the exp is not a structure with line number
        //3. throw type 14 error when id is not a valid structure member line number
        std::string id_str = id->string_value;
        Type* s_type = searchStructType(exp->string_value);
        if (s_type == NULL) {
            printType13Error(line);
        } else {
            FieldList* fl = s_type->structure;
            bool flag = false;
            char* mem_name = id->string_value;
            Type* tp;
            while (fl->next != nullptr) {
                if (strcmp(mem_name, fl->name) == 0) {
                    flag = true;
                    tp = fl->type;
                    break;
                }
                fl = fl->next;
            }
            if (!flag) 
                flag |= strcmp(mem_name, fl->name) == 0;
            if (flag) {
                return tp;
            } else {
                printType14Error(line);
            }
        }
        return NULL;
    }

    Type* typeAfterCalc(Node* exp1,Node* exp2,int line){
        //deal with null
        if(exp1->type_value==NULL||exp2->type_value==NULL){
            printType7Error(line);
            return NULL;
        }
        if(exp1->type_value->primitive==exp1->type_value->FLOAT&&(checkINTexp(exp2,0)||exp2->type_value->primitive==exp2->type_value->FLOAT)){
            return new_prim_type("float");
        }else if(checkINTexp(exp1,0)&&exp2->type_value->primitive==exp2->type_value->FLOAT){
            return new_prim_type("float");
        }else if(checkINTexp(exp1,0)&&checkINTexp(exp2,0)){
            return new_prim_type("int");
        }else{
            printType7Error(line);
            return NULL;
        }

    }

    //do "and" and "or" allow float value?
    //return 1 if the expression have int value
    int checkINTexp(Node* exp, int allowbool){
        if(exp->type_value->primitive==exp->type_value->INT){
            if(allowbool){
                return 1;
            }else{
                if(strcmp(exp->type_value->name,"bool")==0){
                    return 0;
                }else{
                    return 1;
                }
            }
            
        }else{
            return 0;
        }
    } 

private:
    static std::map<std::string, Type*> variable_table;
    static std::map<std::string, std::vector<Type*>> function_table;
    static std::map<std::string, Type*>struct_table;

    static std::map<std::string, ll> struct_hash_table;

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

    void printType10Error(int line){
        std::cout << "Error type 10 at Line " << line << ": non-array type variable could not apply indexing operator." << std::endl;
    } 

    void printType5Error(int line){
        std::cout << "Error type 5 at Line " << line << ": the type of the operands at both sides of assignment operator should match." << std::endl;
    }    

    void printType15Error(int line){
        std::cout << "Error type 15 at Line " << line << ": the structure should not be redfined." << std::endl;
    } 

    void printType14Error(int line){
        std::cout << "Error type 14 at Line " << line << ": the structure should be defined before access." << std::endl;
    } 

    void printType13Error(int line){
        std::cout << "Error type 13 at Line " << line << ": non-structure type variable could not apply dot operator." << std::endl;
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

    //please throw error type here
    // NO, the function is recusively called here, the error must be thrown outer
     bool isSameTypes(Type* type1, Type* type2){
        if (type1->category != type2->category) {
            return false; // The main struct are not the same.
        } else if (type1->category == Type::PRIMITIVE) {
            return type1->primitive == type2->primitive;
        } else if (type1->category == Type::ARRAY) {
            return checkTwoArrayEquv(type1->array, type2->array);
        } else if (type1->category == Type::STRUCTURE) { // There are all Struct type, first try to use simply compare the name of the both struct.
        // try to solve the structure equivence.
            return isSameStruct(type1, type2);
        } else
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

    bool checkTwoArrayEquv(Array* arr1, Array* arr2){
        if (arr1->base->category != arr2->base->category) {
            return false;
        } else if (arr1->base->category == arr2->base->category) {
            if (arr1->base->category == Type::PRIMITIVE) {
                return arr1->base->primitive == arr2->base->primitive;
            } else if (arr1->base->category == Type::STRUCTURE) {
                return isSameStruct(arr1->base, arr2->base);
            } else if (arr1->base->category == Type::ARRAY) {
                return checkTwoArrayEquv(arr1->base->array, arr2->base->array);
            } else return false;
        } else return false;
    }

    bool isSameStruct(Type* type1, Type* type2) {
        return struct_hash_table[type1->name] == struct_hash_table[type2->name];
    }

    ll calStructHash(Type* type) {
        FieldList* list = type->structure;
        ll res = 0;
        while (list->next != nullptr) {
            res = res + calFieldListHash(list);
            res = res % MOD;
            list = list->next;
        }
        res = res + calFieldListHash(list);
        res = res % MOD;
        return res;
    }

    void calStructHash(std::string tname, Type* type) {
        struct_hash_table[tname] = calStructHash(type);
    }

    ll calFieldListHash(FieldList* fieldList) {
        if (fieldList->type->category==Type::PRIMITIVE) {
            if (fieldList->type->primitive == 0) {
                return INT_BASE;
            } else if (fieldList->type->primitive == 1) {
                return FLOAT_BASE;
            } else {
                return CHAR_BASE;
            }
        } else if (fieldList->type->category==Type::ARRAY) {
            ll res = 1;
            Type* tp = fieldList->type;
            while (tp->array->base->category != Type::PRIMITIVE) {
                res = res * ARRAY_PROM;
                res = res % MOD;
                tp = tp->array->base;
            }
            tp = tp->array->base;
            if (tp->primitive == 0) {
                res = res * INT_BASE;
                res = res % MOD;
            } else if (tp->primitive == 1) {
                res = res * FLOAT_BASE;
                res = res % MOD;
            } else {
                res = res * CHAR_BASE;
                res = res % MOD;
            }
            return res;
        } else {
            ll s_hash = struct_hash_table[fieldList->name] * STRU_PROM;
            s_hash = s_hash % MOD;
            return s_hash % MOD;
        }
    }
};
