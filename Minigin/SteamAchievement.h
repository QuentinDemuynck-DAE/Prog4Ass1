#pragma once
#include "Singleton.h"
#include <steam_api.h>

// yoinked from the guide
class SteamAchievement : public dae::Singleton<SteamAchievement>
{
public:
	SteamAchievement();

	bool RequestStats();
	bool SetAchievement(const char* ID);

	STEAM_CALLBACK(SteamAchievement, OnUserStatsReceived, UserStatsReceived_t,
		m_CallbackUserStatsReceived);
	STEAM_CALLBACK(SteamAchievement, OnUserStatsStored, UserStatsStored_t,
		m_CallbackUserStatsStored);
	STEAM_CALLBACK(SteamAchievement, OnAchievementStored,
		UserAchievementStored_t, m_CallbackAchievementStored);
private:
	int m_AppId;
	bool m_Initialized;
};

