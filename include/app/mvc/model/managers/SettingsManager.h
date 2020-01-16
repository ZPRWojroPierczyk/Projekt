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


/**
 * @brief Class responsible for keeping track of application instance's global settings.
 *  
 * Global settings involve graphic and auto settings. Class facilitates
 * interface to load and save client's settings to the database when the
 * client is logginig in and logging out.
 */
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
    };

// Constructors & Destructors
public:

    /**
     * @brief Creates a new settings menager for the app's instance and
     *        loads client's settings from the data base
     * @param clientID 
     */
    SettingsManager(const std::string& clientID);

    /**
     * @brief Destroy the Settings Manager and saves settings to the database
     */
    ~SettingsManager();

// Interface
public:
    
    /**
     * @returns Actuall graphical theme 
     */
    Theme getTheme();
    
    /**
     * @brief Sets a new graphical theme from the set of available
     * @param theme A new theme
     */
    void setTheme(Theme theme);

    /**
     * @return Actuall soundtrack
     */
    Soundtrack getSoundtrack();

    /**
     * @brief Sets a new soundtrack from the set of available
     * @param soundtrack A new soundtrack 
     */
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

    /**
     * @todo Implement settings' loading
     * @brief Loads client's settings from the data base
     */
    void __loadSettings();

    /**
     * @todo Implement settings' saving
     * @brief Saves client's settings to the data base
     */
    void __saveSettings();
    
};

#endif