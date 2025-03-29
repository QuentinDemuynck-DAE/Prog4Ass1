#include "SteamAchievement.h"
#include "Globals.h"
#include <iostream>

SteamAchievement::SteamAchievement()
	:m_AppId(0), 
	m_Initialized(false),
	m_CallbackUserStatsReceived(this, &SteamAchievement::OnUserStatsReceived),
	m_CallbackUserStatsStored(this, &SteamAchievement::OnUserStatsStored),
	m_CallbackAchievementStored(this, &SteamAchievement::OnAchievementStored)
{
	
	m_AppId = SteamUtils()->GetAppID();
	RequestStats();
}

bool SteamAchievement::RequestStats()
{
	if (NULL == SteamUserStats() || NULL == SteamUser()) {
		return false;
	}

	if (!SteamUser()->BLoggedOn()) {
		return false;
	}

	return SteamUserStats()->RequestCurrentStats();
}

bool SteamAchievement::SetAchievement(const char* ID)
{
	if (m_Initialized) 
	{
		SteamUserStats()->SetAchievement(ID);
		return SteamUserStats()->StoreStats();
	}

	return false;
}

void SteamAchievement::OnUserStatsReceived(UserStatsReceived_t* pCallback) {

	if (static_cast<uint64>(m_AppId) == pCallback->m_nGameID) {
		if (k_EResultOK == pCallback->m_eResult) {
			std::cout << "Received stats and achievements from Steam" << std::endl;
			m_Initialized = true;
		}
	}
}

void SteamAchievement::OnUserStatsStored(UserStatsStored_t* pCallback) {

	if (static_cast<uint64>(m_AppId) == pCallback->m_nGameID) {
		if (k_EResultOK == pCallback->m_eResult) {
			std::cout << "Stored stats for Steam" << std::endl;
		}
		else {
			std::cout << "Failed to store stats for steam" << std::endl;
		}
	}
}

void SteamAchievement::OnAchievementStored(UserAchievementStored_t* pCallback) {

	if (static_cast<uint64>(m_AppId) == pCallback->m_nGameID) {
		std::cout << "Stored achievement for Steam" << std::endl;
	}
}
