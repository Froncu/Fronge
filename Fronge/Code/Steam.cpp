#include "Steam.h"

#include "Console.hpp"

#include <isteamuser.h>
#include <isteamutils.h>
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)

#include <format>
#include <stdexcept>

#pragma region Constructors/Destructor
fro::Steam::Steam()
	: m_CallbackUserStatsReceived{ this, &Steam::onUserStatsReceived }
	, m_CallbackUserStatsStored{ this, &Steam::onUserStatsStored }
	, m_CallbackAchievementStored{ this, &Steam::onAchievementStored }
{
	if (SteamAPI_Init() == 0)
		throw std::runtime_error("SteamAPI_Init() failed");

	if (!requestStats())
		throw std::runtime_error("couldn't retrieve Steam stats");

	m_AppID = SteamUtils()->GetAppID();
}

fro::Steam::~Steam()
{
	SteamAPI_Shutdown();
}
#pragma endregion Constructors/Destructor



#pragma region PublicMethods
bool fro::Steam::requestStats() const
{
	if (SteamUserStats() == NULL || SteamUser() == NULL)
		return false;

	if (!SteamUser()->BLoggedOn())
		return false;

	return SteamUserStats()->RequestCurrentStats();
}

bool fro::Steam::unlockAchievement(AchievementID const achievementID) const
{
	if (m_IsInitialized)
	{
		SteamUserStats()->SetAchievement(getAchievementName(achievementID).c_str());

		return SteamUserStats()->StoreStats();
	}

	return false;
}

void fro::Steam::update() const
{
	SteamAPI_RunCallbacks();
}
#pragma endregion PublicMethods



#pragma region PrivateMethods
void fro::Steam::onUserStatsReceived(UserStatsReceived_t* const pCallback)
{
	if (pCallback->m_nGameID == m_AppID)
	{
		if (pCallback->m_eResult == k_EResultOK)
		{
			Console::getInstance().log("Received stats and achievements from Steam");
			m_IsInitialized = true;

			for (auto& achievement : m_vAchievements)
			{
				SteamUserStats()->GetAchievement(achievement.pAchievementID, &achievement.achieved);

				_snprintf_s
				(
					achievement.aName,
					sizeof(achievement.aName),
					"%s",
					SteamUserStats()->GetAchievementDisplayAttribute(achievement.pAchievementID, "name")
				);

				_snprintf_s
				(
					achievement.aDescription,
					sizeof(achievement.aDescription),
					"%s",
					SteamUserStats()->GetAchievementDisplayAttribute(achievement.pAchievementID, "desc")
				);
			}
		}
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "Failed to receive stats and achievements from Steam, %d", pCallback->m_eResult);
			Console::getInstance().log(buffer);
		}
	}
}

void fro::Steam::onUserStatsStored(UserStatsStored_t* const pCallback)
{
	if (m_AppID == pCallback->m_nGameID)
	{
		if (pCallback->m_eResult == k_EResultOK)
			Console::getInstance().log("Stored stats for Steam");
		else
		{
			char buffer[128];
			_snprintf_s(buffer, 128, "Failed to store stats for Steam, %d", pCallback->m_eResult);
			Console::getInstance().log(buffer);
		}
	}
}

void fro::Steam::onAchievementStored(UserAchievementStored_t* const pCallback)
{
	if (pCallback->m_nGameID == m_AppID)
		Console::getInstance().log("Stored Achievement for Steam");
}

fro_NODISCARD std::string fro::Steam::getAchievementName(AchievementID const achievementID) const
{
	switch (achievementID)
	{
	case ACH_WIN_ONE_GAME:
		return "ACH_WIN_ONE_GAME";

	default:
		throw std::runtime_error(std::format("Achievement ID {} not mapped to a string name", achievementID));
	}
}
#pragma endregion PrivateMethods