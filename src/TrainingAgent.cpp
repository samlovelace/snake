
#include "TrainingAgent.h"


TrainingAgent::TrainingAgent()
{
    mContext = zmq::context_t(1);
    mSocket = zmq::socket_t(mContext, zmq::socket_type::req);
    mSocket.connect("tcp://localhost:5555");

}

TrainingAgent::~TrainingAgent()
{

}

DIRECTION TrainingAgent::move(const Observation& observation)
{
    // Serialize the observation to JSON
    nlohmann::json jsonObservation = observation.toJson();
    std::string serializedObservation = jsonObservation.dump();

    // Send the serialized observation to the Python process
    zmq::message_t request(serializedObservation.size());
    memcpy(request.data(), serializedObservation.c_str(), serializedObservation.size());
    mSocket.send(request, zmq::send_flags::none);

    // Receive the response from the Python process
    zmq::message_t reply;
    mSocket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());

    // Parse the JSON response and extract the integer direction
    nlohmann::json jsonResponse = nlohmann::json::parse(response);
    int directionInt = jsonResponse["direction"].get<int>();

    return static_cast<DIRECTION>(directionInt);
}

void TrainingAgent::reward(const Reward& reward) 
{
    nlohmann::json jsonReward = reward.toJson(); 
    std::string serializedReward = jsonReward.dump(); 

    zmq::message_t rewardReq(serializedReward.size()); 
    memcpy(rewardReq.data(), serializedReward.c_str(), serializedReward.size()); 
    mSocket.send(rewardReq, zmq::send_flags::none); 

    // Receive the response from the Python process notifying receipt
    zmq::message_t reply;
    mSocket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());
}