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


SettingsManager::SettingsManager(const std::string& clientID) :
    __clientID(clientID)
{
    __loadSettings();
}

SettingsManager::~SettingsManager(){
    __saveSettings();
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Public member methods ----------------------------*/
/*--------------------------------------------------------------------------------*/

SettingsManager::Theme SettingsManager::getTheme(){
    return __theme;
}

void SettingsManager::setTheme(SettingsManager::Theme theme){
    __theme = theme;
}

SettingsManager::Soundtrack SettingsManager::getSoundtrack(){
    return __soundtrack;
}

void SettingsManager::setSoundtrack(SettingsManager::Soundtrack soundtrack){
    __soundtrack = soundtrack;
}



/*--------------------------------------------------------------------------------*/
/*----------------------------- Private member methods ---------------------------*/
/*--------------------------------------------------------------------------------*/

void SettingsManager::__loadSettings(){
    __theme = Theme::Default;
    __soundtrack = Soundtrack::Default;
}

void SettingsManager::__saveSettings(){

}