#ifndef TABLE_HPP
#define TABLE_HPP
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <cassert>
#include <algorithm>

#define SUCCESS 1
#define FAILURE 0
#define OVERFLOW -1
#define NOEXIST -2

template<class data>
class Table {
private:
    int state;
    int index;
    std::vector<std::map<std::string, data>> table;
    std::vector<std::string> rows;
    std::vector<std::string> columns;
    std::string type;
public:
    Table();
    Table(const std::vector<std::string>& _columns);
    Table(const int _columns);
    virtual ~Table();
    Table(const Table& t);
    int update_state(const std::vector<std::string>& _columns);
    int update_state(const int _columns);

    int get_index();
    int get_state();
    std::string get_type();
    bool isExist(const std::string &p);

    int insert(std::map<std::string, data> p, std::string row = std::string(""));
    int delRow(const int &row);
    int delRow(const std::string &p);
    void showTable();

    int get_value(const std::string &p, std::map<std::string, data> &value);
    int get_value(const std::string &r, const std::string &c, data &value);
    int update_value(const std::string &r, const std::string &c, const data &value);
    int update_value(const std::string &p, std::map<std::string, data> &value);
};

template<class data>
Table<data>::Table() {
    type = "";
    index = 0;
    state = 0;
}

template<class data>
Table<data>::Table
(const std::vector<std::string>& _columns) {
    type = typeid(data).name();
    index = 0;
    columns = _columns;
    state = 1;
}

template<class data>
Table<data>::Table(const int _columns){
    type = typeid(data).name();
    index = 0;
    for(int i = 0; i < _columns; ++i){
        columns.push_back(std::to_string(i));
    }
    state = 1;
}

template<class data>
Table<data>::~Table() {
    index = 0;
    table.clear();
    columns.clear();
    rows.clear();
    type.clear();
}

template<class data>
Table<data>::Table(const Table& t) {
    state = t.state;
    index = t.index;
    columns = t.columns;
    table = t.table;
    type = t.type;
}

template<class data>
int Table<data>::update_state(const std::vector<std::string>& _columns) {
    if(_columns.size() <= 0)return FAILURE;
    index = 0;
    type = typeid(data).name();
    columns = _columns;
    state = 1;
    return SUCCESS;
}

template<class data>
int Table<data>::update_state(const int _columns){
    if(_columns <= 0)return FAILURE;
    type = typeid(data).name();
    index = 0;
    for(int i = 0; i < _columns; ++i){
        columns.push_back(std::to_string(i));
    }
    state = 1;
    return SUCCESS;
}

template<class data>
int Table<data>::get_index() {
    return index;
}

template<class data>
int Table<data>::get_state() {
    return state;
}

template<class data>
std::string Table<data>::get_type() {
    return type;
}

template<class data>
bool Table<data>::isExist(const std::string &p){
    return std::find(rows.begin(), rows.end(), p) == rows.end() ? false : true;
}

template<class data>
int Table<data>::insert(std::map<std::string, data> p, std::string row) {
    try {
        if (state == 0) {
            throw "the table is not initialized!";
        }
    }
    catch (const char* msg) {
        std::cerr << msg << std::endl;
        assert(state == 1);
        return FAILURE;
    }

    std::map<std::string, data> temp;
    for (auto& i : columns) {
        temp.insert(std::pair<std::string, data>(i, p[i]));
    }
    if (row == "") {
        rows.push_back(std::to_string(index));
    }
    else {
        rows.push_back(row);
    }
    table.push_back(p);
    ++index;

    return SUCCESS;
}

template<class data>
int Table<data>::delRow(const int &row) {
    if (row < 0 || row >= table.size())return OVERFLOW;
    std::cout << row << std::endl;
    table.erase(table.begin() + row);
    std::cout << rows.size()<< ' ' << rows[row] << std::endl;
    rows.erase(rows.begin() + row);
    --index;
    return SUCCESS;
}

template<class data>
int Table<data>::delRow(const std::string &p) {
    std::vector<std::string>::iterator it = std::find(rows.begin(), rows.end(), p);
    if (it != rows.end()) {
        table.erase(table.begin() + (it - rows.begin()));
        rows.erase(it);
        --index;
        return SUCCESS;
    }
    return FAILURE;
}

template<class data>
void Table<data>::showTable() {
    std::cout << "------------" << std::endl;
    std::cout << "         ";
    for (auto& i : columns) {
        std::cout << std::right << std::setfill(' ') << std::setw(8) << i << ' ';
    }
    std::cout << std::endl;
    if (table.size() == 0) {
        std::cout << "...empty..." << std::endl;
    }
    else {
        for (auto& i : table) {
            std::cout << std::right << std::setfill(' ') << std::setw(8) << rows[static_cast<int>(&i - &*table.begin())] << ' ';
            for (auto& col : columns) {
                std::cout << std::right << std::setfill(' ') << std::setw(8) << i[col] << ' ';
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    std::cout << "------------" << std::endl;
}

template<class data>
int Table<data>::get_value(const std::string &p, std::map<std::string, data> &value){
    std::vector<std::string>::iterator it = std::find(rows.begin(), rows.end(), p);
    if(it == rows.end()){
        return NOEXIST;
    }
    
    value = table[static_cast<int>(it - rows.begin())];
    return SUCCESS;
}

template<class data>
int Table<data>::get_value(const std::string &r, const std::string &c, data &value){
    std::vector<std::string>::iterator it_r = std::find(rows.begin(), rows.end(), r);
    std::vector<std::string>::iterator it_c = std::find(columns.begin(), columns.end(), c);
    if(it_r == rows.end() || it_c == columns.end()){
        return NOEXIST;
    }
    
    value = table[static_cast<int>(it_r - rows.begin())][c];
    return SUCCESS;
}

template<class data>
int Table<data>::update_value(const std::string &r, const std::string &c, const data &value){
    std::vector<std::string>::iterator it_r = std::find(rows.begin(), rows.end(), r);
    std::vector<std::string>::iterator it_c = std::find(columns.begin(), columns.end(), c);
    if(it_r == rows.end() || it_c == columns.end()){
        return NOEXIST;
    }

    table[static_cast<int>(it_r - rows.begin())][c] = value;
    return SUCCESS;
}

template<class data>
int Table<data>::update_value(const std::string &p, std::map<std::string, data> &value){
    std::vector<std::string>::iterator it = std::find(rows.begin(), rows.end(), p);
    if(it == rows.end()){
        return NOEXIST;
    }
    
    table[static_cast<int>(it - rows.begin())] = value;
    return SUCCESS;
}

#endif