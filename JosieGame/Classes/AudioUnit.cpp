#include "AudioUnit.h"
#include "SimpleAudioEngine.h"
#include <cstdlib> // rand()
#include <sstream>
#include "cocos2d.h"

using namespace CocosDenshion;

AudioUnit::AudioUnit() {}
AudioUnit::~AudioUnit() {}

//
// Preload & Unload
//

void AudioUnit::preloadLevelSounds()
{
	CCLOG("music volume at audioUnitInit: %f", cocos2d::UserDefault::getInstance()->getIntegerForKey("music_volume")/100.0);
	CCLOG("sfx volume at audioUnitInit: %f", cocos2d::UserDefault::getInstance()->getIntegerForKey("sfx_volume")/100.0);
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->preloadEffect("audio/josie_sounds/jump_1.mp3");
	engine->preloadEffect("audio/josie_sounds/jump_2.mp3");
	engine->preloadEffect("audio/josie_sounds/jump_3.mp3");
	engine->preloadEffect("audio/josie_sounds/slide_1.mp3");
	engine->preloadEffect("audio/josie_sounds/slide_2.mp3");
	engine->preloadEffect("audio/josie_sounds/slide_3.mp3");
	engine->preloadEffect("audio/josie_sounds/stop_1.mp3");
	engine->preloadEffect("audio/josie_sounds/stop_2.mp3");
	engine->preloadEffect("audio/josie_sounds/stop_3.mp3");
}

void AudioUnit::preloadBossSounds()
{

}

void AudioUnit::unloadLevelSounds()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->unloadEffect("audio/josie_sounds/jump_1.mp3");
	engine->unloadEffect("audio/josie_sounds/jump_2.mp3");
	engine->unloadEffect("audio/josie_sounds/jump_3.mp3");
	engine->unloadEffect("audio/josie_sounds/slide_1.mp3");
	engine->unloadEffect("audio/josie_sounds/slide_2.mp3");
	engine->unloadEffect("audio/josie_sounds/slide_3.mp3");
	engine->unloadEffect("audio/josie_sounds/stop_1.mp3");
	engine->unloadEffect("audio/josie_sounds/stop_2.mp3");
	engine->unloadEffect("audio/josie_sounds/stop_3.mp3");
}

void AudioUnit::unloadBossSounds()
{

}

//
// Play Josie Sounds
//

void AudioUnit::playJosieJumpSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/jump_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setEffectsVolume(cocos2d::UserDefault::getInstance()->getIntegerForKey("sfx_volume")/100.0);
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}
void AudioUnit::playJosieSlideSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/slide_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setEffectsVolume(cocos2d::UserDefault::getInstance()->getIntegerForKey("sfx_volume")/100.0);
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}
void AudioUnit::playJosieStopRunSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/stop_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setEffectsVolume(cocos2d::UserDefault::getInstance()->getIntegerForKey("sfx_volume")/100.0);
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}

//
// Other Sounds
//

void AudioUnit::stopBackground() {
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void AudioUnit::pauseBackground() {
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

void AudioUnit::resumeBackground() {
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void AudioUnit::playLevelBackground()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setBackgroundMusicVolume(cocos2d::UserDefault::getInstance()->getIntegerForKey("music_volume")/100.0);
	engine->playBackgroundMusic("audio/MainMenuAmbienceTrack96bit.mp3", true);
}

void AudioUnit::playBossBackground()
{

}
