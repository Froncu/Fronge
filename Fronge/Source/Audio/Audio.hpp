#ifndef AUDIO_HPP
#define AUDIO_HPP

#include "Core.hpp"
#include "Events/AudioEvent.hpp"
#include "Events/Systems/EventQueue.hpp"
#include "froch.hpp"
#include "Reference/Reference.hpp"
#include "Resources/Music/Music.hpp"
#include "Resources/SoundEffect/SoundEffect.hpp"

namespace fro
{
   class Audio final
   {
      public:
         FRO_API Audio();
         Audio(Audio const&) = delete;
         Audio(Audio&&) noexcept = delete;

         FRO_API ~Audio();

         Audio& operator=(Audio const&) = delete;
         Audio& operator=(Audio&&) noexcept = delete;

         FRO_API void playMusic(std::string filePath);
         FRO_API void playMusic(Reference<Music> const music);
         FRO_API void pauseMusic();
         FRO_API void resumeMusic();
         FRO_API void stopMusic();
         FRO_API FRO_NODISCARD Reference<Music> getActiveMusic();

         FRO_API void playSoundEffect(std::string filePath, int const channel = -1);
         FRO_API void playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel = -1);
         FRO_API void pauseSoundEffect(int const channel = -1);
         FRO_API void resumeSoundEffect(int const channel = -1);
         FRO_API void stopSoundEffect(int const channel = -1);
         FRO_API FRO_NODISCARD Reference<SoundEffect> getActiveSoundEffect(int const channel);
         FRO_API FRO_NODISCARD int getAmountOfChannels();

         FRO_API void setMute(bool const mute);
         FRO_API FRO_NODISCARD bool isMuted();

      private:
         void internalPauseSoundEffect(int const channel);
         void internalResumeSoundEffect(int const channel);
         void internalStopSoundEffect(int const channel);

         template<typename EventType, typename... Arguments> requires
            std::constructible_from<AudioEvent, EventType> and std::constructible_from<EventType, Arguments...>
         void pushEvent(Arguments&&... arguments)
         {
            {
               std::lock_guard const lockGuard{ mMutex };
               mEventQueue.pushEvent(EventType(std::forward<Arguments>(arguments)...));
            }

            mConditionVariable.notify_one();
         }

         EventQueue<AudioEvent> mEventQueue;
         std::mutex mMutex{};
         std::condition_variable mConditionVariable{};
         std::jthread mEventProcessingThread
         {
            [this]()
            {
               while (true)
               {
                  std::unique_lock uniqueLock{ mMutex };
                  mConditionVariable.wait(uniqueLock,
                     [this]()
                     {
                        return not mEventQueue.getQueue().empty() or not mRunThread;
                     });

                  if (not mRunThread)
                     break;

                  mEventQueue.fetchNextEvent();
                  uniqueLock.unlock();
                  mEventQueue.processEvent();
               }
            }
         };
         std::vector<Reference<SoundEffect>> mActiveSoundEffects;
         std::vector<std::unique_ptr<SoundEffect>> mLoadedSoundEffects;
         Reference<Music> mActiveMusic{};
         std::unique_ptr<Music> mLoadedMusic{};
         bool mRunThread{ true };
         bool mMuted{};
   };
}

#endif