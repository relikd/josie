#ifndef AUDIOUNIT_H_
#define AUDIOUNIT_H_

class AudioUnit
{
public:
	AudioUnit();
	~AudioUnit();
	static AudioUnit* initWithLevel(int lvl, int sub_lvl);
	void playBackground();
	void playJosieJumpSound();
	void playJosieSlideSound();
	void playJosieStopRunSound();

private:
	int _level;
	int _sublevel;

	bool isBossLevel();

	void preloadJosieActions_Level();
	void preloadJosieActions_Boss();
	void unloadJosieActions_Level();
	void unloadJosieActions_Boss();
};

#endif /* AUDIOUNIT_H_ */
