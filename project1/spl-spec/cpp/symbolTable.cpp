#include <map>
#include <string>
#include <vector>
#include "type.h"

class symbolTable{
public:
    symbolTable() {};
    std::string searchVariable(std::string varName){
        std::map<std::string, std::string>::iterator iter;
        iter = variable_table.find(varName);
        if (iter != variable_table.end())
            return iter->second;
        else
            return nullptr;
    }
    std::vector<std::string>* searchFunction(std::string funName){
        std::map<std::string, std::vector<std::string>>::iterator iter;
        iter = function_table.find(funName);
        if (iter != function_table.end())
            return &iter->second;
        else
            return NULL;
    }
    Type* serchTypeInfo(std::string typeName){
        std::map<std::string, Type>::iterator iter;
        iter = type_table.find(typeName);
        if (iter != type_table.end())
            return &iter->second;
        else
            return NULL;
    }

private:
    static std::map<std::string, std::string> variable_table;
    static std::map<std::string, std::vector<std::string>> function_table;
    static std::map<std::string, Type>type_table;
};
