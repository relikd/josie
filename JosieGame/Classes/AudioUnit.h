#ifndef AUDIOUNIT_H_
#define AUDIOUNIT_H_

class Level;

class AudioUnit
{
public:
	AudioUnit();
	~AudioUnit();
	static AudioUnit* initWithLevel(Level *lvl);
	void playBackground();
	void stopBackground();
	void playJosieJumpSound();
	void playJosieSlideSound();
	void playJosieStopRunSound();

private:
	Level* _level;
	void preloadJosieActions_Level();
	void preloadJosieActions_Boss();
	void unloadJosieActions_Level();
	void unloadJosieActions_Boss();
};

#endif /* AUDIOUNIT_H_ */
