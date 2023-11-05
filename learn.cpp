#include "QLearning_Brain.hpp"
#include "maze.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <thread>
#include <chrono>
using namespace std;

string s = 
    "############\n"
    "#@  #   # ##\n"
    "# #       ##\n"
    "#  ## # #  #\n"
    "##    # ## #\n"
    "#   #    # #\n"
    "# ## #     #\n"
    "#    #   # #\n"
    "##  ### #  #\n"
    "###     ## #\n"
    "##  # #   O#\n"
    "############\n";

int main(){
    QL ql_test(4);
    Maze maze(s);
    int x_start, y_start;
    maze.get_start(x_start, y_start);
    string ob = to_string(x_start) + ", " + to_string(y_start);
    maze.showMap();
    int x, y;
    maze.get_size(x, y);
    cout<<x<<" "<<y<<endl;
    int cnt = 0;
    for(int i = 0; i < 20000; ++i){
        ob = to_string(x_start) + ", " + to_string(y_start);
        maze.reset();
        while(true){
            int action = ql_test.choose_action(ob);
            string next;
            int reward = maze.step(action, next);
            ql_test.learn(ob, action, reward, next);
            ob = next;
            if(i % 20 == 0){ 
                system("cls");
                maze.showMap();
                // cout<<cnt<<' '<<i<<endl;
                // cout<<i<<endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
            // ql_test.showQ_table();
            if(next == "terminal"){
                if(reward == 1)cnt++;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}