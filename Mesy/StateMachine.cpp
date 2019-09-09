#include "StateMachine.hpp"

namespace MESY {
    void StateMachine::AddState(StateRef newState, bool isReplacing) {
        this->_isAdding = true;
        this->_isReplacing = isReplacing;
        this->_newState = std::move(newState);
    }
    
    void StateMachine::ProcessStateChanges() {
        
        if(this->_isAdding){
            if(!this->_states.empty()){
                if(this->_isReplacing){
                    this->_states.pop();
                }
            }
            this->_states.push(std::move(this->_newState));
            this->_isAdding = false;
        }
    }
    
    StateRef &StateMachine::GetActiveState() {
        return this->_states.top();
    }
}
