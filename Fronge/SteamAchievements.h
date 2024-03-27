#pragma once

#include <Steam/isteamuserstats.h>
#include <cstdint>
#include <vector>
#include <string>

#define fro_ACHIEVEMENT_ID( ID, name ) { ID, #ID, name, "", 0, 0 }

namespace fro
{
	enum class AchievementID
	{
		ACH_WIN_ONE_GAME = 0
	};

	class SteamAchievements final
	{
		struct Achievement
		{
			AchievementID achievementID;
			const char* pAchievementID;
			char aName[128];
			char aDescription[256];
			bool achieved;
			int iconImage;
		};

	public:
		SteamAchievements(const std::vector<Achievement>& vAchievements = { fro_ACHIEVEMENT_ID(AchievementID::ACH_WIN_ONE_GAME, "Winner") });

		~SteamAchievements() = default;

		bool requestStats();

		bool unlockAchievement(AchievementID achievementID);

	private:
		SteamAchievements(const SteamAchievements&) = delete;
		SteamAchievements(SteamAchievements&&) = delete;

		SteamAchievements& operator=(const SteamAchievements&) = delete;
		SteamAchievements& operator=(SteamAchievements&&) = delete;

		STEAM_CALLBACK(SteamAchievements, onUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
		STEAM_CALLBACK(SteamAchievements, onUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(SteamAchievements, onAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);

		std::string getAchievementName(AchievementID achievementID);

		std::uint32_t m_AppID; // Our current AppID
		std::vector<Achievement> m_vAchievements; // Achievements data
		bool m_IsInitialized{ false }; // Have we called Request stats and received the callback?
	};
}