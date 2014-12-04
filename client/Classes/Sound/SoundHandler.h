#ifndef SoundHandler_H_
#define SoundHandler_H_

#include <string.h>

#include "cocos2d.h"
USING_NS_CC;

class SoundHandler {
public:
	void pauseSound();
	void resumeSound();
	static SoundHandler* getInstance();

	void playEffect(std::string name);
	void playBackground(std::string name, bool skipCurrentCheck = false);
	void stopBackground();

	void enableSound(bool enable);
	bool isSoundEnabled() {
		return _sound;
	}
private:
	bool _sound;
	static SoundHandler* _instance;
	~SoundHandler();
	SoundHandler();
	std::string _fileExtention;
	std::string _currentBackgroundMusic;
};

#endif /* SoundHandler_H_ */
