/**
 * @file SettingsManager.cc
 * @author Wojtek Rokicki & Krzysiek Pierczyk
 * @brief 
 * @version 0.1
 * @date 2020-01-14
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "SettingsManager.h"

/*--------------------------------------------------------------------------------*/
/*--------------------------- Constructors & Destructors -------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @brief Creates a new settings menager for the app's instance and
 *        loads client's settings from the data base
 * @param clientID 
 */
SettingsManager::SettingsManager(const std::string& clientID) :
    __clientID(clientID)
{
    __loadSettings();
}

/**
 * @brief Destroy the Settings Manager and saves settings to the database
 */
SettingsManager::~SettingsManager(){
    __saveSettings();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @return Actuall graphical theme 
 */
SettingsManager::Theme SettingsManager::getTheme(){
    return __theme;
}

/**
 * @brief Sets a new graphical theme from the set of available
 * @param theme : a new theme
 */
void SettingsManager::setTheme(SettingsManager::Theme theme){
    __theme = theme;
}

/**
 * @return Actuall soundtrack
 */
SettingsManager::Soundtrack SettingsManager::getSoundtrack(){
    return __soundtrack;
}

/**
 * @brief Sets a new soundtrack from the set of available
 * @param soundtrack 
 */
void SettingsManager::setSoundtrack(SettingsManager::Soundtrack soundtrack){
    __soundtrack = soundtrack;
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

/**
 * @todo Implement settings' loading
 * @brief Loads client's settings from the data base
 */
void SettingsManager::__loadSettings(){
    __theme = Theme::Default;
    __soundtrack = Soundtrack::Default;
}

/**
 * @todo Implement settings' saving
 * @brief Saves client's settings to the data base
 */
void SettingsManager::__saveSettings(){

}