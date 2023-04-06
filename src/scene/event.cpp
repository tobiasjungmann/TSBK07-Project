#include "event.hpp"

namespace sm {

void FSM::pushState(std::shared_ptr<State> toPush) {
    states.push_back(toPush);
}

void FSM::interpret(Input const& input) {
    auto it = states.rbegin();
    while(it != states.rend() and (*it++)->update(input, *this));
}

}