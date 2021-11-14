#include<stdlib.h>
#include<string.h>
#include "../symbolTable.cpp"

int main() {
    Type* tp1, *tp2;
    strcpy(tp1->name, "st1");
    strcpy(tp1->name, "st2");

    FieldList* list;
    list->type = new_prim_type("int");
    Type* arrtp;
    arrtp->category = Type::ARRAY;
    Array* arr;
    arr->base = new_prim_type("char");
    arr->size = 3;
    arrtp->array = arr;


    FieldList* list1, *list2, *list3;
    FieldList* _list1, *_list2, *_list3;

    list1->next = list2;
    list2->next = list3;
    list3->next = nullptr;

    _list1->next = _list2;
    _list2->next = _list3;
    _list3->next = nullptr;

    list1->type = new_prim_type("int");
    list2->type = new_prim_type("float");
    list3->type = arrtp;

    _list1->type = new_prim_type("float");
    _list2->type = arrtp;
    _list3->type = new_prim_type("int");

    tp1->structure = list1;
    tp2->structure = _list1;

    std::cout << calStructHash(tp1) << std::endl;
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
            return -1;
        }
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

