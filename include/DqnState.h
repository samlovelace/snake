#ifndef DQN_STATE_H
#define DQN_STATE_H

#include "Observation.h"

/**
 * @brief Shared encode/decode boundary between the game and the DQN: how an
 *        Observation becomes the network's input State, and how the
 *        network's chosen action maps back to a DIRECTION. Used by both the
 *        training agent (zmq) and the inference agent (ONNX Runtime) so they
 *        see identical state representations.
 */
namespace DqnState
{
    State build(const Observation& observation);

    DIRECTION actionToDirection(int action, DIRECTION currentDir);
}

#endif // DQN_STATE_H
