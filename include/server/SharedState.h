#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <memory>
#include <string>
#include <unordered_set>

class HttpSession;

// Represents the shared server state
class SharedState
{
    std::string doc_root_;
    std::unordered_set<HttpSession*> sessions_;

public:
    explicit
    SharedState(std::string doc_root);

    std::string const&
    doc_root() const noexcept
    {
        return doc_root_;
    }

    void join  (HttpSession& session);
    void leave (HttpSession& session);
};

#endif
