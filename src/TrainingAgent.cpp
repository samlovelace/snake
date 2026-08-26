
#include "TrainingAgent.h"
#include "constants.h" // grid size constants


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
    State state = buildState(observation);

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
    return actionToDirection(directionInt, observation.direction);
}

DIRECTION TrainingAgent::actionToDirection(int action, DIRECTION currentDir)
{
    switch (static_cast<AI::TRY>(action))
    {
    case AI::TRY::LEFT:
        return AI::turnSnakeLeft(currentDir);
    case AI::TRY::RIGHT:
        return AI::turnSnakeRight(currentDir);
    case AI::TRY::FORWARD:
    default:
        return currentDir;
    }
}

void TrainingAgent::reward(const Reward& reward)
{
    State state = buildState(reward.state);

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

bool TrainingAgent::tryDir(const Observation& observation, AI::TRY tryDir)
{
    bool danger = false;
    int try_x = observation.head.x;
    int try_y = observation.head.y;

    switch (observation.direction)
    {
    case DIRECTION::UP:
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_y += -CELL_SIZE;  
            break;
        case AI::TRY::LEFT:
            try_x += -CELL_SIZE; 
            break; 
        case AI::TRY::RIGHT: 
            try_x += CELL_SIZE; 
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::DOWN: 
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_y += CELL_SIZE; 
            break;
        case AI::TRY::LEFT:
            try_x += CELL_SIZE; 
            break;
        case AI::TRY::RIGHT: 
            try_x += -CELL_SIZE; 
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::LEFT:
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_x += -CELL_SIZE; 
            break;
        case AI::TRY::LEFT: 
            try_y += CELL_SIZE;  
            break;
        case AI::TRY::RIGHT: 
            try_y += -CELL_SIZE;  
            break; 
        default:
            break;
        }
        break;
    case DIRECTION::RIGHT: 
        switch (tryDir)
        {
        case AI::TRY::FORWARD:
            try_x += CELL_SIZE; 
            break;
        case AI::TRY::LEFT: 
            try_y += -CELL_SIZE; 
            break; 
        case AI::TRY::RIGHT:
            try_y += CELL_SIZE; 
            break;
        default:
            break;
        }
    
    default:
        break;
    }

    // dont run into yourself dummy
    for(const auto& seg : observation.segments)
    {
        if(try_x == seg.x && try_y == seg.y)
        {
            danger = true; 
        }
    }

    // dont hit the border
    if(try_x < MIN_X || try_x >= MAX_X)
    {
        danger = true; 
    }

    // dont hit the other border dummy 
    if(try_y < MIN_Y || try_y >= MAX_Y)
    {
        danger = true; 
    }

    return danger;
}


std::array<int, 4> TrainingAgent::foodDirFromObservation(const Observation& observation)
{
    int dx = observation.apple.x - observation.head.x;
    int dy = observation.apple.y - observation.head.y;

    std::array<int, 4> food_direction = {
        dy < 0 ? 1 : 0,  // up
        dy > 0 ? 1 : 0,  // down
        dx < 0 ? 1 : 0,  // left
        dx > 0 ? 1 : 0   // right
    };

    return food_direction;
};

State TrainingAgent::buildState(const Observation& observation)
{
    // one hot encoding of current direction
    std::array<int, 4> dir = {0, 0, 0, 0};
    dir[(int)observation.direction] = 1;

    // danger of doing each action
    std::array<AI::TRY, 3> tryDirs = {AI::TRY::FORWARD, AI::TRY::LEFT, AI::TRY::RIGHT};
    std::array<int, 3> danger = {0, 0, 0};
    for(int i = 0; i < 3; i++)
    {
        danger[i] = tryDir(observation, tryDirs[i]);
    }

    // relative direction to food
    std::array<int, 4> foodDir = foodDirFromObservation(observation);

    // relative distance to food
    float dx = (observation.head.x - observation.apple.x); 
    float dy = (observation.head.y - observation.apple.y); 
    float dist = sqrt(dx * dx + dy * dy); 

    // normalize by the grid's max possible distance (the diagonal)
    const float maxDist = std::sqrt(2.0f) * GRID_DIM;
    float normalizedDist = dist / maxDist;

    State state;
    state.direction = dir;
    state.danger = danger;
    state.foodDir = foodDir;
    state.distance = normalizedDist;

    return state;
}