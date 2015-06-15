#include "AudioUnit.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace CocosDenshion;

AudioUnit::AudioUnit()
{
	_isBoss = false;
}
AudioUnit::~AudioUnit()
{
	CCLOG("~AudioUnit");
	if (_isBoss)
		unloadJosieActions_Boss();
	else
		unloadJosieActions_Level();
}

AudioUnit* AudioUnit::initWithBoss(bool isBoss)
{
	AudioUnit *au = new AudioUnit();
	au->_isBoss = isBoss;

	if (isBoss)
		au->preloadJosieActions_Boss();
	else
		au->preloadJosieActions_Level();

	return au;
}


//
// Preload & Unload
//

void AudioUnit::preloadJosieActions_Level()
{
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

void AudioUnit::preloadJosieActions_Boss()
{

}

void AudioUnit::unloadJosieActions_Level()
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

void AudioUnit::unloadJosieActions_Boss()
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
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}
void AudioUnit::playJosieSlideSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/slide_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}
void AudioUnit::playJosieStopRunSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/stop_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 1.0);
}



//
// Other Sounds
//

void AudioUnit::playBackground()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setBackgroundMusicVolume(1.0);

	if (_isBoss)
		nullptr;
	else
		engine->playBackgroundMusic("audio/MainMenuAmbienceTrack96bit.mp3", true);
}

void AudioUnit::stopBackground()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->stopBackgroundMusic(true);
}
