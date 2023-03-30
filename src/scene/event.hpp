#include <vector>
#include <memory>

class State {
    protected:
        virtual void enter() = 0;
        virtual void exit() = 0;
    public:
        virtual bool update(Input const& input, FSM& machine) = 0;
};

class Input {

};

class FSM {
    private:
    std::vector<std::shared_ptr<State>> states; // TODO maybe consider linked list or deque for performance on access

    void popState();
    void pushState(std::shared_ptr<State> toPush);

    public:
    void interpret(Input const& input);
};