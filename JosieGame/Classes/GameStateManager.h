#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	static void initManagerWithDefaults();
	static bool isLevelUnlocked(int level, int sublevel);
	static int getCoinsForLevel(int level, int sublevel);
	static int getAllCollectedCoins();
	static void setCoinsForLevel(int level, int sublevel, int coins);
};

#endif /* GAMESTATEMANAGER_H_ */
