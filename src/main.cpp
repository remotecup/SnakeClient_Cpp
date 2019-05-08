#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <fstream>
#include <string>
#include "Message.h"
#include "Decision.h"
#include "World.h"

using namespace std;

int main() {
    //read config
    ifstream config;
    config.open("config");
    string line;
    getline(config, line);getline(config, line);
    string ip = line;
    getline(config, line);getline(config, line);
    string port = line;
    getline(config, line);getline(config, line);
    string team_name = line;

    int thisSocket;
    struct sockaddr_in destination;
    destination.sin_family = AF_INET;
    thisSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (thisSocket < 0)
    {
        printf("\nSocket Creation FAILED!");
        return 0;
    }
    destination.sin_port = htons(stoi(port));
    destination.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(thisSocket,(struct sockaddr *)&destination,sizeof(destination))!=0){
        printf("\nSocket Connection FAILED!\n");
        return 0;
    }

    //send connect request
    string msg = MessageClientConnectRequest(team_name).build();
    char buffer[4096];
    memcpy(buffer, msg.c_str(), msg.length());
    send(thisSocket, buffer, msg.length(), 0);

    //receive connect resp
    socklen_t len;
    ssize_t newData;
    size_t maxr = 4096;
    char rcvb[4096];
    newData = recvfrom(thisSocket, rcvb, 4096,
                       MSG_WAITALL, (struct sockaddr *) &destination,
                       &len);
    Message * Msg = pars(rcvb);
    if(Msg != nullptr){
        std::cout<<"self_id:"<<static_cast<MessageClientConnectResponse *>(Msg)->id<<std::endl;
        std::cout<<"max_i:"<<static_cast<MessageClientConnectResponse *>(Msg)->max_i<<std::endl;
        std::cout<<"max_j:"<<static_cast<MessageClientConnectResponse *>(Msg)->max_j<<std::endl;
        std::cout<<"team_number:"<<static_cast<MessageClientConnectResponse *>(Msg)->team_number<<std::endl;
        std::cout<<"goal_id:"<<static_cast<MessageClientConnectResponse *>(Msg)->goal_id<<std::endl;
    }else{
        std::cout<<"received msg is not client connection"<<std::endl;
        return 1;
    }

    //make world
    World * wm = new World();
    wm->init_board(static_cast<MessageClientConnectResponse *>(Msg)->max_i, static_cast<MessageClientConnectResponse *>(Msg)->max_j);
    wm->set_id(static_cast<MessageClientConnectResponse *>(Msg)->id, static_cast<MessageClientConnectResponse *>(Msg)->goal_id);

    //run game
    while (true){
        char rcvb[8000];
        socklen_t len;
        newData = recvfrom(thisSocket, rcvb, 8000,
                           MSG_WAITALL, (struct sockaddr *) &destination,
                           &len);
        rcvb[newData] = '\0';
        Message * Msg = pars(rcvb);
        if(Msg != nullptr){
            if(Msg->e_type == MessageType::ClientDisconnect){
                break;
            }else if(Msg->e_type == MessageType::ClientWorld){
                wm->update(static_cast<MessageClientWorld *>(Msg)->world_value, static_cast<MessageClientWorld *>(Msg)->cycle);
                wm->print();
                //get action
                string action = get_action(wm);
                //send action
                string msg("{\"message_type\":\"MessageClientAction\" , \"value\": {\"action\": \""+action+"\"}}");
                cout<<msg<<endl;
                char action_msg[4096];
                memcpy(action_msg, msg.c_str(), msg.length());
                send(thisSocket, action_msg, msg.length(), 0);
            }else{
                continue;
            }
        }
    }
    return 0;
}
