#include <stack>
#include <memory>

namespace sm {
class Input;
class State;
class FSM;

class Input {
    
};

class State {
    protected:
        virtual void enter() = 0;
        virtual void exit() = 0;
    public:
        virtual bool update(Input const& input, FSM& machine) = 0;
};

using state_t = std::unique_ptr<State>;

class FSM : public std::stack<state_t> {
    private:
    std::stack<std::unique_ptr<State>> states; // TODO maybe consider linked list or deque for performance on access

    void popState();
    void pushState(std::unique_ptr<State> toPush);

    public:
    void interpret(Input const& input);
};
}