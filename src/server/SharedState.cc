#include "SharedState.h"

SharedState::SharedState(std::string doc_root)
    : doc_root_(std::move(doc_root))
{
}

void SharedState::join(HttpSession& session){
    sessions_.insert(&session);
}

void SharedState::leave(HttpSession& session){
    sessions_.erase(&session);
}