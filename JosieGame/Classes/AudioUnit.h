#ifndef AUDIOUNIT_H_
#define AUDIOUNIT_H_

class AudioUnit
{
public:
	AudioUnit();
	~AudioUnit();
	static AudioUnit* initWithBoss(bool isBoss);
	void playBackground();
	void stopBackground();
	void playJosieJumpSound();
	void playJosieSlideSound();
	void playJosieStopRunSound();

private:
	bool _isBoss;
	void preloadJosieActions_Level();
	void preloadJosieActions_Boss();
	void unloadJosieActions_Level();
	void unloadJosieActions_Boss();
};

#endif /* AUDIOUNIT_H_ */
