#include <map>
#include <string>
#include <vector>
#include "type.h"

class symbolTable{
public:
    symbolTable() {};
    std::string searchVariable(std::string varName);
    std::vector<std::string> searchFunction(std::string funName);
    Type serchTypeInfo(std::string typeName);

private:
    static std::map<std::string, std::string> variable_table;
    static std::map<std::string, std::vector<std::string>> function_table;
    static std::map<std::string, Type>type_table;
};