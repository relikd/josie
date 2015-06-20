#include "AudioUnit.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

using namespace CocosDenshion;
using namespace cocos2d;

AudioUnit::AudioUnit() {}
AudioUnit::~AudioUnit() {}

//
// Preload & Unload
//

void AudioUnit::preloadLevelSounds()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setEffectsVolume(UserDefault::getInstance()->getIntegerForKey("sfx_volume")/200.0);
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
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setEffectsVolume(UserDefault::getInstance()->getIntegerForKey("sfx_volume")/200.0);
	engine->preloadEffect("audio/boss_sounds/boss_hit1.mp3");
	engine->preloadEffect("audio/boss_sounds/boss_hit2.mp3");
	engine->preloadEffect("audio/boss_sounds/boss_hit3.mp3");
	engine->preloadEffect("audio/boss_sounds/projectile_hit1.mp3");
	engine->preloadEffect("audio/boss_sounds/projectile_hit2.mp3");
	engine->preloadEffect("audio/boss_sounds/projectile_hit3.mp3");
	engine->preloadEffect("audio/boss_sounds/shoot.mp3");
	engine->preloadEffect("audio/josie_sounds/josie_hit1.mp3");
	engine->preloadEffect("audio/josie_sounds/josie_hit2.mp3");
	engine->preloadEffect("audio/josie_sounds/josie_hit3.mp3");
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
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->unloadEffect("audio/boss_sounds/boss_hit1.mp3");
	engine->unloadEffect("audio/boss_sounds/boss_hit2.mp3");
	engine->unloadEffect("audio/boss_sounds/boss_hit3.mp3");
	engine->unloadEffect("audio/boss_sounds/projectile_hit1.mp3");
	engine->unloadEffect("audio/boss_sounds/projectile_hit2.mp3");
	engine->unloadEffect("audio/boss_sounds/projectile_hit3.mp3");
	engine->unloadEffect("audio/boss_sounds/shoot.mp3");
	engine->unloadEffect("audio/josie_sounds/josie_hit1.mp3");
	engine->unloadEffect("audio/josie_sounds/josie_hit2.mp3");
	engine->unloadEffect("audio/josie_sounds/josie_hit3.mp3");
}

//
// Play Josie Sounds
//

void AudioUnit::playJosieJumpSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/jump_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 0.5);
}
void AudioUnit::playJosieSlideSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/slide_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 0.5);
}
void AudioUnit::playJosieStopRunSound()
{
	std::ostringstream s;
	s << "audio/josie_sounds/stop_"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 0.5);
}


void AudioUnit::playJosieShootSound(){
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect("audio/boss_sounds/shoot.mp3",false,1.0,1.0,3.0);
}

void AudioUnit::playJosieHitSound(){
	std::ostringstream s;
	s << "audio/josie_sounds/josie_hit"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 0.7);
}

void AudioUnit::playShopPurchaseSound(){
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect("audio/shoppurchase.mp3",false,1.0,1.0,1.0);
}

void AudioUnit::playBossHitSound(){
	std::ostringstream s;
	s << "audio/boss_sounds/boss_hit"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, -1.0, 0.3);
}

void AudioUnit::playProjectileHitSound(){
	std::ostringstream s;
	s << "audio/boss_sounds/projectile_hit"<< (rand()%3)+1 <<".mp3";

	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->playEffect(s.str().c_str(), false, 1.0, 1.0, 0.3);
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

void AudioUnit::startBackgroundLevel()
{
	SimpleAudioEngine* engine = SimpleAudioEngine::getInstance();
	engine->setBackgroundMusicVolume(UserDefault::getInstance()->getIntegerForKey("music_volume")/100.0);
	engine->playBackgroundMusic("audio/level_audio.mp3", true);
}

void AudioUnit::startBackgroundBoss()
{

}
