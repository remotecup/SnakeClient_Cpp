#ifndef WORLD
#define WORLD

#include "Math.h"
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "rapidjson/document.h"

using namespace rapidjson;
using namespace std;
class Snake{
public:
    int id;
    Vector2D head;
    vector<Vector2D> body;
    string name;
    Snake(int _id){
        id = _id;
    }
    vector<Vector2D>& get_body(){
        return body;
    }
    void add_body(Vector2D pos){
        body.push_back(pos);
    }
    Vector2D get_head(){
        return head;
    }
    void set_head(Vector2D pos){
        head = pos;
    }
    int get_id(){
        return id;
    }
    void reset(string _name){
        head = Vector2D::invalid();
        body.clear();
        name = _name;
    }
};
class World{
public:
    int ** board;
    int cycle;
    int self_id;
    int goal_id;
    Vector2D goal_position;
    map<int, Snake*> snakes;
    vector<Vector2D> * walls;
    int max_i;
    int max_j;
    World(){
        for(int i = 1; i <= 4; i++){
            snakes[i] = new Snake(i);
        }
        walls = new vector<Vector2D>;
    }
    void init_board(int _max_i, int _max_j){
        max_i = _max_i;
        max_j = _max_j;
        board = new int * [max_i];
        for(int i = 0; i < max_i; i++){
            board[i] = new int[max_j];
            for(int j = 0; j < max_j; j++){
                board[i][j] = 0;
            }
        }

    }

    void set_id(int _self_id, int _goal_id){
        self_id = _self_id;
        goal_id = _goal_id;
    }
    Snake * get_self(){
        return snakes[self_id];
    }
    Snake * get_snake(int id){
        return snakes[id];
    }
    vector<Vector2D> * get_walls(){
        return walls;
    }

    void update(Value * world_value, int _cycle){
        walls->clear();
        cycle = _cycle;
        Value & board_value = (*world_value)["board"];
        vector<vector<int> > _board;
        for(auto& p : board_value.GetArray()){
            vector<int> iboard;
            for(auto& q : p.GetArray()){
                iboard.push_back(q.GetInt());
            }
            _board.push_back(iboard);
        }


        Value & heads_value = (*world_value)["heads"];
        Value::MemberIterator M;
        int s = 1;
        for (M=heads_value.MemberBegin(); M!=heads_value.MemberEnd(); M++)
        {
            string team = M->name.GetString();
            snakes[s]->reset(team);
            Vector2D head;
            int n = 0;
            for(auto& p : M->value.GetArray()){
                if(n==0)
                    head.i = p.GetInt();
                else
                    head.j = p.GetInt();
                n += 1;
            }
            snakes[s]->set_head(head);
            s++;
        }

        for(int i = 0; i < max_i; i++){
            for(int j = 0; j < max_j; j++){
                board[i][j] = _board[i][j];
                if(board[i][j] == goal_id){
                    goal_position = Vector2D(i, j);
                }
                else if(board[i][j] > 0){
                    snakes[board[i][j]]->add_body(Vector2D(i, j));
                }
                else if(board[i][j] == -1){
                    walls->push_back(Vector2D(i, j));
                }
            }
        }
    }
    void print(){
        cout<<"cycle:"<<cycle<<endl;
        for (int i = 0; i < max_i; i++){
            for (int j = 0; j < max_i; j++){
                std::cout<<board[i][j]<<",";
            }
            cout<<std::endl;
        }
        cout<<endl<<"snake 1:";
        for (auto& p : snakes[1]->get_body()){
            cout<<p<<",";
        }
        cout<<endl<<"snake 2:";
        for (auto& p : snakes[2]->get_body()){
            cout<<p<<",";
        }
        cout<<endl<<"snake 3:";
        for (auto& p : snakes[3]->get_body()){
            cout<<p<<",";
        }
        cout<<endl<<"snake 4:";
        for (auto& p : snakes[4]->get_body()){
            cout<<p<<",";
        }
        cout<<endl;
    }
};
#endif
