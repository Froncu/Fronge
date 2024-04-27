#if not defined fro_STEAM_H
#define fro_STEAM_H

#include "Defines.hpp"
#include "Singleton.hpp"

#include <isteamuserstats.h>

#include <cstdint>
#include <string>
#include <vector>

#define fro_STEAM_ACHIEVEMENT_ID(ID, name){ ID, #ID, name, "", 0, 0 }

namespace fro
{
	class Steam final : public Singleton<Steam>
	{
	public:
		enum AchievementID
		{
			ACH_WIN_ONE_GAME = 0
		};

		Steam();

		virtual ~Steam() override;

		void update() const;
		bool requestStats() const;
		bool unlockAchievement(AchievementID const achievementID) const;

	private:
		Steam(Steam const&) = delete;
		Steam(Steam&&) noexcept = delete;

		Steam& operator=(Steam const&) = delete;
		Steam& operator=(Steam&&) noexcept = delete;

		struct Achievement
		{
			AchievementID achievementID;
			char const* pAchievementID;
			char aName[128];
			char aDescription[256];
			bool achieved;
			int iconImage;
		};

		STEAM_CALLBACK(Steam, onUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived);
		STEAM_CALLBACK(Steam, onUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored);
		STEAM_CALLBACK(Steam, onAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored);

		fro_NODISCARD std::string getAchievementName(AchievementID const achievementID) const;

		std::vector<Achievement> m_vAchievements
		{
			fro_STEAM_ACHIEVEMENT_ID(ACH_WIN_ONE_GAME, "Winner")
		};

		bool m_IsInitialized{ false };

		std::uint32_t m_AppID;
	};
}

#endif