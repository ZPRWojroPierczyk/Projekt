/**
 * @file SettingsManager.h
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief Class representing applications settings
 * @version 0.1
 * @date 2019-12-30
 * 
 * @copyright Copyright (c) 2019
 * 
 */
#ifndef SETTINGS_MANAGER_H 
#define SETTINGS_MANAGER_H

#include <string>

class SettingsManager
{
// Public types
public:
    /// Graphical themes
    enum class Theme{
        Default
    };
    /// Soundtracks
    enum class Soundtrack{
        Default
    }

// Constructors & Destructors
public:
    SettingsManager(const std::string& clientID);
    ~SettingsManager();

// Interface
public:
    
    Theme getTheme();
    void setTheme(Theme theme);

    Soundtrack getSoundtrack();
    void setSoundtrack(Soundtrack soundtrack);

// Private members
private:
    /// ID of the client owning app instance
    const std::string __clientID;
    
    /// Settings values
    Theme __theme;
    Soundtrack __soundtrack;

// Private member methods
private:
    void __loadSettings();
    void __saveSettings();
};

#endif