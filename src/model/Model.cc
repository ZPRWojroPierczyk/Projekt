#include "Model.h"


bool Model::setState(Model::State newState_t){
    state = newState_t;
    return true;
}


bool Model::openMenu(){
    setState(State::Menu);
    return true;
}