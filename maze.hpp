#ifndef MAZE_HPP
#define MAZE_HPP

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

class Maze{
    private:
        std::vector<std::string> map;
        std::vector<std::string> origin_map;
        std::string str_map;
        int x_size, y_size, x_now, y_now, x_destination, y_destination, x_start, y_start;
    public:
        Maze();
        Maze(const std::string &m);
        virtual ~Maze() = default;
        void init();

        void importMap(std::string &m);
        void showMap();
        void get_size(int &x, int &y);
        void get_now(int &x, int &y);
        void get_start(int &x, int &y);
        void get_destination(int &x, int &y);
        int step(int action, std::string &next);
        void reset();
};

Maze::Maze(){
    x_size = y_size = x_now = y_now = x_destination = y_destination = x_start = y_start = 0;
}

Maze::Maze(const std::string &m){
    x_size = y_size = x_now = y_now = x_destination = y_destination = x_start = y_start = 0;
    str_map = m;
    init();
}

void Maze::init(){
    std::string str_temp;
    for(auto &i : str_map){
        if(i == '\n'){
            map.push_back(str_temp);
            str_temp.clear();
        }
        else if(i == '@'){
            x_start = x_now = map.size();
            str_temp += i;
            y_start = y_now = str_temp.length() - 1;
        }
        else if(i == 'O'){
            x_destination = map.size();
            str_temp += i;
            y_destination = str_temp.length() - 1;
        }
        else str_temp += i;
    }
    origin_map = map;
    x_size = map.size() - 1;
    y_size = map[0].length() - 1;
}

void Maze::importMap(std::string &m){
    str_map = m;
    init();
}

void Maze::showMap(){
    str_map.clear();
    for(auto &i : map){
        str_map += i + '\n';
    }
    std::cout<<str_map;
}

void Maze::get_size(int &x, int &y){
    x = x_size;
    y = y_size;
}

void Maze::get_now(int &x, int &y){
    x = x_now;
    y = y_now;
}

void Maze::get_start(int &x, int &y){
    x = x_start;
    y = y_start;
}

void Maze::get_destination(int &x, int &y){
    x = x_destination;
    y = y_destination;
}

int Maze::step(int action, std::string &next){
    int direction[4][2] = {
        {-1, 0},
        {0, 1},
        {1, 0},
        {0, -1}
    };

    int dx = x_now + direction[action][0];
    int dy = y_now + direction[action][1];
    if(dx <= 0 || dx >= x_size || dy <= 0 || dy >= y_size){
        next = std::to_string(x_now) + ", " + std::to_string(y_now);
        return 0;
    }
    else{
        next = std::to_string(dx) + ", " + std::to_string(dy);

        map[x_now][y_now] = ' ';
        map[x_now = dx][y_now = dy] = '@'; 

        if(origin_map[dx][dy] == '#'){
            next = "terminal"; 
            return -1;
        }
        else if(origin_map[dx][dy] == 'O'){
            next = "terminal";
            return 1;
        }
        else{
            return 0;
        }
    }
}

void Maze::reset() {
    map[x_now][y_now] = ' ';
    map[x_now = x_start][y_now = y_start] = '@';
    map = origin_map;
}

#endif