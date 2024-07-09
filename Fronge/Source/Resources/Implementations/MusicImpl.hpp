#if not defined MUSIC_IMPL_HPP
#define MUSIC_IMPL_HPP

#include "froch.hpp"

#include "Resources/Music.hpp"
#include "Utility.hpp"

typedef struct _Mix_Music Mix_Music;

namespace fro
{
	class Music::Implementation final
	{
	public:
		FRO_API Implementation(std::string_view const filePath);

		~Implementation() = default;

		FRO_API FRO_NODISCARD Mix_Music* getSDLMusic() const;

	private:
		Implementation(Implementation const&) = delete;
		Implementation(Implementation&&) noexcept = delete;

		Implementation& operator=(Implementation const&) = delete;
		Implementation& operator=(Implementation&&) noexcept = delete;

		CustomUniquePointer<Mix_Music> mSDLMusic;
	};
}

#endif