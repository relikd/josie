#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_



class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	static void initDefaults();
	static void initSpriteCache();
	static bool isLevelUnlocked(int level, int sublevel);

	static void updateCoinsForLevel(int level, int sublevel, int coins);
	static void addCoins(int coins);
	static int getAllCollectedCoins();
	static int getCoinsForLevel(int level, int sublevel);
	static void setCoinsForLevel(int level, int sublevel, int coins);
};

#endif /* GAMESTATEMANAGER_H_ */
