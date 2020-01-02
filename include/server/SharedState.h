/**
 * @file SharedState.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief SharedState class' declaration
 * @version 0.1
 * @date 2020-01-02
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#ifndef SHARED_STATE_H
#define SHARED_STATE_H

#include <memory>
#include <string>
#include <unordered_set>

class HttpSession;

// 
/**
 * @brief Represents the server state that should be accessible by all
 *        server's modules
 */
class SharedState
{
    /// Absolute path to the folder containing static files
    std::string doc_root_;
    /// List of the active client's connections
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
