#ifndef QLEARNING_HPP
#define QLEARNING_HPP

#include "table.hpp"
#include <random>
#include <map>
#include <string>
#include <ctime>
#include <vector>

class QL{
    private:
        double learning_rate, gamma, e_greedy;
        Table<double> Q_table;
        int actions;
    public:
        QL();
        QL(int act, double lr = 0.1, double reward_decay = 0.9, double e = 0.99);
        int choose_action(std::string observation);
        void check_state_exist(std::string state);
        void learn(std::string state, int _action, int reward, std::string new_state);
        void showQ_table();
};

QL::QL(){
    learning_rate = 0.1;
    gamma = 0.9;
    e_greedy = 0.99;
    actions = 0;
}

QL::QL(int act, double lr, double reward_decay, double e){
    learning_rate = lr;
    gamma = reward_decay;
    e_greedy = e;
    actions = act;
    Q_table.update_state(actions);
}

int QL::choose_action(std::string observation){
    check_state_exist(observation);
    std::random_device rd;
    std::default_random_engine e{ rd() };
    std::uniform_real_distribution<> g1(0.0, 1.0);
    // static std::uniform_int_distribution<> g2(0, actions);
    int act = -1;
    if(g1(e) < e_greedy){
        std::map<std::string, double> temp;
        Q_table.get_value(observation, temp);
        std::vector<int> _;
        double max = temp["0"];
        for(auto &i : temp){
            if(i.second > max)max = i.second;
        }
        for(auto &i : temp){
            if(i.second == max)_.push_back(std::stoi(i.first));
        }
        std::uniform_int_distribution<> g2{ 0, static_cast<int>(_.size() - 1) };
        act = _[g2(e)];
    }
    else{
        std::uniform_int_distribution<> g2{ 0, static_cast<int>(actions - 1) };
        act = g2(e);
    }
    return act;
}

void QL::check_state_exist(std::string state){
    if(!Q_table.isExist(state)){
        std::map<std::string, double> temp;
        for(int i = 0; i < actions; ++i){
            temp.insert(std::pair<std::string, double>(std::to_string(i), 0.0));
        }
        Q_table.insert(temp, state);
    }
}

void QL::learn(std::string state, int _action, int reward, std::string new_state){
    check_state_exist(new_state);
    double Q_predict, Q_target;
    Q_table.get_value(state, std::to_string(_action), Q_predict);
    if(new_state != "terminal"){
        std::map<std::string, double> _temp;
        Q_table.get_value(new_state, _temp);
        double max = _temp["0"];
        for(auto &i : _temp){
            if(i.second > max)max = i.second;
        }
        Q_target = reward + gamma * max;
    }
    else{
        Q_target = reward;
    }
    Q_table.update_value(state, std::to_string(_action), learning_rate * (Q_target - Q_predict));
}

void QL::showQ_table() {
    Q_table.showTable();
}
#endif