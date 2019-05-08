#include <rapidjson/writer.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"

using namespace std;
using namespace rapidjson;

enum class MessageType{
    None,
    ClientConnectRequest,
    ClientConnectResponse,
    ClientWorld,
    ClientDisconnect
};

class Message{
public:
    string type;
    MessageType e_type = MessageType::None;
};

class MessageClientConnectRequest : public Message{
public:
    string client_name;
    MessageClientConnectRequest(string name){
        type = string("ClientConnectRequest");
        e_type = MessageType::ClientConnectRequest;
        client_name = name;
    }
    string build(){
        Document d;
        Value json_msg(kObjectType);

        Value json_type;
        json_type.SetString(type.c_str(), type.length(), d.GetAllocator());
        json_msg.AddMember("message_type", json_type, d.GetAllocator());

        Value json_name;
        json_name.SetString(client_name.c_str(), client_name.length(), d.GetAllocator());

        Value json_value;
        json_value.SetObject();
        json_value.AddMember("name", json_name, d.GetAllocator());
        json_msg.AddMember("value", json_value, d.GetAllocator());

        StringBuffer buffer;
        Writer<StringBuffer> writer(buffer);
        json_msg.Accept(writer);
        const char* output = buffer.GetString();
        string msg(output);
        return msg;
    }
};

class MessageClientConnectResponse : public Message{
public:
    int id;
    int max_i;
    int max_j;
    int team_number;
    int goal_id;
    MessageClientConnectResponse(int _id,int _max_i, int _max_j, int _team_number, int _goal_id){
        type = string("MessageClientConnectResponse");
        e_type = MessageType::ClientConnectResponse;
        id = _id;
        max_i = _max_i;
        max_j = _max_j;
        team_number = _team_number;
        goal_id = _goal_id;
    }
    static std::pair<bool, Message *> parse(char * msg){
        Document d;
        d.Parse(msg);
        string type = d["message_type"].GetString();
        if (type.compare(string("MessageClientConnectResponse")) == 0){
            Value & value = d["value"];
            int id = value["id"].GetInt();
            Value & ground_config = value["ground_config"];
            int max_i = ground_config["max_i"].GetInt();
            int max_j = ground_config["max_j"].GetInt();
            int team_number = ground_config["team_number"].GetInt();
            int goal_id = ground_config["goal_id"].GetInt();
            return std::make_pair(true, new MessageClientConnectResponse(id, max_i, max_j, team_number, goal_id));
        }
        return std::make_pair(false, nullptr);
    }
};

class MessageClientWorld : public Message{
public:
    int cycle;
    map<string, int> score;
    vector<vector<int> > board;
    Value * world_value;

    MessageClientWorld(int _cycle, map<string, int> _score, Value * _world){
        type = string("MessageClientWorld");
        e_type = MessageType::ClientWorld;
        cycle = _cycle;
        score = _score;
        world_value = _world;
    }
    static std::pair<bool, Message *> parse(char * msg){
        Document d;
        d.Parse(msg);
        string type = d["message_type"].GetString();
        if (type.compare(string("MessageClientWorld")) == 0){
            Value & value = d["value"];
            int cycle = value["cycle"].GetInt();
            Value & score_value = value["score"];
            Value::MemberIterator M;
            map<string, int> score;
            for (M=score_value.MemberBegin(); M!=score_value.MemberEnd(); M++)
            {
                string team = M->name.GetString();
                int sc = M->value.GetInt();
                score.insert(make_pair(team, sc));
            }
            Value * world = new Value(value["world"], d.GetAllocator());
            return std::make_pair(true, new MessageClientWorld(cycle, score, world));
        }
        return std::make_pair(false, nullptr);
    }
};

class MessageClientDisconnect : public Message{
public:
    int cycle;
    map<string, int> score;
    vector<vector<int> > board;
    MessageClientDisconnect(){
        type = string("MessageClientDisconnect");
        e_type = MessageType::ClientDisconnect;
    }
    static std::pair<bool, Message *> parse(char * msg){
        Document d;
        d.Parse(msg);
        string type = d["message_type"].GetString();
        if (type.compare(string("MessageClientDisconnect")) == 0){
            return std::make_pair(true, new MessageClientDisconnect());
        }
        return std::make_pair(false, nullptr);
    }
};

Message * pars(char * _msg){
    string str_msg = string(_msg);
    char a = '\'';
    char b = '\"';
    replace(str_msg.begin(), str_msg.end(), a, b);
    char msg[8000];
    memcpy(msg, str_msg.c_str(), str_msg.length());
    msg[str_msg.length()] = '\0';
    std::pair<bool, Message *> ret;

    ret = MessageClientConnectResponse::parse(msg);
    if (std::get<0>(ret)){
        return std::get<1>(ret);
    }

    ret = MessageClientWorld::parse(msg);
    if (std::get<0>(ret)){
        return std::get<1>(ret);
    }

    ret = MessageClientDisconnect::parse(msg);
    if (std::get<0>(ret)){
        return std::get<1>(ret);
    }
    return nullptr;
}
