
#include "TrainingAgent.h"
#include "DqnState.h"


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
    State state = DqnState::build(observation);

    nlohmann::json jsonState = state.toJson();
    std::string serializedState = jsonState.dump();

    // Send the serialized observation to the Python process
    zmq::message_t request(serializedState.size());
    memcpy(request.data(), serializedState.c_str(), serializedState.size());
    mSocket.send(request, zmq::send_flags::none);

    // Receive the response from the Python process
    zmq::message_t reply;
    zmq::recv_result_t result = mSocket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());

    // Parse the JSON response and extract the integer direction
    nlohmann::json jsonResponse = nlohmann::json::parse(response);
    int directionInt = jsonResponse["direction"].get<int>();

    // convert fwd(0)/left(1)/right(2) action from DQN to DIRECTION based on current direction
    return DqnState::actionToDirection(directionInt, observation.direction);
}

void TrainingAgent::reward(const Reward& reward)
{
    State state = DqnState::build(reward.state);

    nlohmann::json jsonReward;
    jsonReward["reward"] = reward.reward;
    jsonReward["state"] = state.toJson();
    jsonReward["done"] = reward.done;
    std::string serializedReward = jsonReward.dump();

    zmq::message_t rewardReq(serializedReward.size());
    memcpy(rewardReq.data(), serializedReward.c_str(), serializedReward.size());
    mSocket.send(rewardReq, zmq::send_flags::none);

    // Receive the response from the Python process notifying receipt
    zmq::message_t reply;
    zmq::recv_result_t result = mSocket.recv(reply, zmq::recv_flags::none);
    std::string response(static_cast<char*>(reply.data()), reply.size());
}
