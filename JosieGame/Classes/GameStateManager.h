#ifndef GAMESTATEMANAGER_H_
#define GAMESTATEMANAGER_H_

#define SAVE_USER_STATE false // TODO: active before App release

class GameStateManager {
public:
	GameStateManager();
	virtual ~GameStateManager();

	static void initManagerWithDefaults();
	static bool isLevelUnlocked(int level, int sublevel);

	static void updateCoinsForLevel(int level, int sublevel, int coins);
	static void addCoins(int coins);
	static int getAllCollectedCoins();
	static int getCoinsForLevel(int level, int sublevel);
	static void setCoinsForLevel(int level, int sublevel, int coins);
};

#endif /* GAMESTATEMANAGER_H_ */
