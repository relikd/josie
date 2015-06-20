#ifndef AUDIOUNIT_H_
#define AUDIOUNIT_H_

class AudioUnit
{
public:
	AudioUnit();
	virtual ~AudioUnit();

	static void preloadLevelSounds();
	static void preloadBossSounds();
	static void unloadLevelSounds();
	static void unloadBossSounds();

	static void playJosieJumpSound();
	static void playJosieSlideSound();
	static void playJosieStopRunSound();
	static void playJosieShootSound();
	static void playJosieHitSound();

	static void playShopPurchaseSound();

	static void playBossHitSound();
	static void playProjectileHitSound();


	static void stopBackground();
	static void pauseBackground();
	static void resumeBackground();
	static void startBackgroundLevel();
	static void startBackgroundBoss();
};

#endif /* AUDIOUNIT_H_ */
