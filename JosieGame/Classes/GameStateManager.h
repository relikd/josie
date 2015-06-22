#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

#define JOSIE_DEVELOPER_MODE 1

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	static void initDefaults(bool force=false);
	static void initSpriteCache();
	
	static bool isLevelUnlocked(int level, int sublevel);
	static bool isRandomLevelUnlocked(int level);
	static void setBossDefeated(int level);
	static bool showCutscenes();

	static void addCoins(int coins);
	static int getAllCollectedCoins();

	static void updateCoinsForLevel(int level, int sublevel, int coins);
	static void updateTimeForLevel(int level, int sublevel, int time);
	static int getCoinsForLevel(int level, int sublevel);
	static int getTimeForLevel(int level, int sublevel);
private:
	// please call update coins/time
	static void setCoinsForLevel(int level, int sublevel, int coins);
	static void setTimeForLevel(int level, int sublevel, int time);
	static int getIntForLevel(const char* attribute, int level, int sublevel);
	static void setIntForLevel(const char* attribute, int level, int sublevel, int new_val);
};

#endif /* GAMESTATEMANAGER_H_ */
