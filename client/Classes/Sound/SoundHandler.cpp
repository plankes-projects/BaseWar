#include "SoundHandler.h"
#include "SimpleAudioEngine.h"

SoundHandler* SoundHandler::_instance = NULL;
SoundHandler* SoundHandler::getInstance() {
	if (_instance == NULL)
		_instance = new SoundHandler();

	return _instance;
}

SoundHandler::SoundHandler() {
	//init sound file names
	/*
	 * Raw music in same file.
	 * Than convert
	 * 	For iOS:
	 * 		Effects: CAF
	 * 		Music: AIFC or CAF
	 * 		Convert on mac:
	 * 			afconvert -f caff -d ima4 sfx.wav
	 *			afconvert -f AIFC -d ima4 bgm.wav
	 * 	For Android/Linux:
	 * 		Effects: OOG
	 * 		Music: OOG
	 */

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	_fileExtention = ".caf";
#else
	_fileExtention = ".ogg";
#endif

	CocosDenshion::SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
	_currentBackgroundMusic = "";
	_sound = CCUserDefault::sharedUserDefault()->getBoolForKey("soundIsEnabled", true);
}

SoundHandler::~SoundHandler() {
}

void SoundHandler::pauseSound() {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

void SoundHandler::resumeSound() {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
	CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}

void SoundHandler::playEffect(std::string name) {
//CCLOG("start effect: %s", (name + _fileExtention).c_str());
	if (!_sound)
		return;

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect((name + _fileExtention).c_str());
}

void SoundHandler::playBackground(std::string name, bool skipCurrentCheck) {
//CCLOG("start background: %s", (name + _fileExtention).c_str());
	if (!skipCurrentCheck && _currentBackgroundMusic == name)
		return;

	_currentBackgroundMusic = name;

	if (!_sound)
		return;

	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic((name + _fileExtention).c_str(), true);
}
void SoundHandler::stopBackground() {
	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
}

void SoundHandler::enableSound(bool enable) {
	_sound = enable;
	CCUserDefault::sharedUserDefault()->setBoolForKey("soundIsEnabled", _sound);

	if (_sound)
		playBackground(_currentBackgroundMusic, true);
	else
		stopBackground();
}
