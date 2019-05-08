#include "Decision.h"
string get_action(World * wm){
    Vector2D goal = wm->goal_position;
    Vector2D head = wm->get_self()->get_head();
    bool safe_pos[4] = {true, true, true, true};
    Vector2D next_pos[4] = {head + Vector2D(1,0), head + Vector2D(-1,0),head + Vector2D(0,1),head + Vector2D(0,-1)};
    for(int n = 0; n < 4; n++){
        cout<<next_pos[n]<<endl;
        auto in_wall = find(wm->walls->begin(), wm->walls->end(), next_pos[n]);
        if(in_wall != wm->walls->end()){
            safe_pos[n] = false;
            continue;
        }
        for(int s = 1; s <= 4; s++){
            cout<<wm->snakes[s]->get_body().size()<<endl;
            auto in_snake = find(wm->snakes[s]->get_body().begin(), wm->snakes[s]->get_body().end(), next_pos[n]);
            if(in_snake != wm->snakes[s]->get_body().end()){
                safe_pos[n] = false;
                break;
            }
        }
    }

    int best_dist = 1000;
    int best_action = -1;
    for(int n = 0; n < 4; n++){
        if (!safe_pos[n]){
            continue;
        }
        int dist = next_pos[n].dist(goal);
        cout<<next_pos[n]<<":"<<dist<<std::endl;
        if (dist < best_dist){
            best_dist = dist;
            best_action = n;
        }
    }
    switch (best_action) {
    case 0:
        return "d";
    case 1:
        return "u";
    case 2:
        return "r";
    case 3:
        return "l";
    default:
        return "u";
    }
}

string simple_decision(World * wm){
    Vector2D goal = wm->goal_position;
    Vector2D head = wm->get_self()->get_head();
    string ret = "u";
    if(goal.i > head.i){
        ret = "d";
    }else if(goal.i < head.i){
        ret = "u";
    }
    else if(goal.j > head.j){
        ret = "r";
    }else{
        ret = "l";
    }
    return ret;
}
