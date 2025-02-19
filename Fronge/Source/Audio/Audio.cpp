#include <SDL_mixer.h>

#include "Audio.hpp"
#include "froch.hpp"
#include "Resources/Music/Implementation/MusicImpl.hpp"
#include "Resources/SoundEffect/Implementation/SoundEffectImpl.hpp"
#include "Utility/Assert.hpp"
#include "Utility/VariantVisitor.hpp"

namespace fro
{
   Audio::Audio()
      : mEventQueue
      {
         VariantVisitor
         {
            [this](LoadPlayMusicEvent&& event)
            {
               auto loadedMusic{ std::make_unique<Music>(std::move(event.filePath)) };

               if (Mix_PlayMusic(loadedMusic->getImplementation().getSDLMusic(), -1) == 0)
               {
                  Logger::info("loaded and played Music with ID {}!",
                     loadedMusic->getID());

                  mActiveMusic = loadedMusic.get();
                  mLoadedMusic = std::move(loadedMusic);
               }
               else
                  Logger::warn("failed to play Music with ID {} ({})",
                     loadedMusic->getID(), Mix_GetError());
            },

            [this](PlayMusicEvent&& event)
            {
               Reference<Music> const& music{ event.music };

               if (not music.valid())
                  Logger::warn("failed to play Music (passed Reference to Music is not valid)");
               else if (Mix_PlayMusic(music->getImplementation().getSDLMusic(), -1) == 0)
               {
                  Logger::info("played Music with ID {}!",
                     music->getID());

                  mActiveMusic = music;
                  mLoadedMusic.reset();
               }
               else
                  Logger::warn("failed to play Music with ID {} ({})",
                     music->getID(), Mix_GetError());
            },

            [this](PauseMusicEvent&&)
            {
               if (not mActiveMusic.valid())
                  Logger::warn("failed to pause Music (no active Music)");
               else if (Mix_PausedMusic())
                  Logger::warn("failed to pause Music (Music is not playing)");
               else
                  Logger::info("paused music with ID {}!",
                     mActiveMusic->getID());

               Mix_PauseMusic();
            },

            [this](ResumeMusicEvent&&)
            {
               if (not mActiveMusic.valid())
                  Logger::warn("failed to resume Music (no active Music)");
               else if (not Mix_PausedMusic())
                  Logger::warn("failed to resume Music with ID {} (music is already playing)",
                     mActiveMusic->getID());
               else
                  Logger::info("resumed Music with ID {}!",
                     mActiveMusic->getID());

               Mix_ResumeMusic();
            },

            [this](StopMusicEvent&&)
            {
               if (not mActiveMusic.valid())
                  Logger::warn("failed to stop music (no active Music)");
               else
                  Logger::info("stopped Music with ID {}!",
                     mActiveMusic->getID());

               Mix_HaltMusic();
               mActiveMusic.reset();
               mLoadedMusic.reset();
            },

            [this](LoadPlaySoundEffectEvent&& event)
            {
               auto loadedSoundEffect{ std::make_unique<SoundEffect>(std::move(event.filePath)) };

               int const channel{
                  Mix_PlayChannel(event.channel, loadedSoundEffect->getImplementation().getSDLSoundEffect(), 0)
               };
               if (channel not_eq -1)
               {
                  Logger::info("loaded and played SoundEffect with ID {} on channel {}!",
                     loadedSoundEffect->getID(), channel);

                  mActiveSoundEffects[channel] = loadedSoundEffect.get();
                  mLoadedSoundEffects[channel] = std::move(loadedSoundEffect);
               }
               else
               {
                  if (event.channel == -1)
                     Logger::warn("failed to play SoundEffect with ID {} on first free channel ({})",
                        loadedSoundEffect->getID(), Mix_GetError());
                  else
                     Logger::warn("failed to play SoundEffect with ID {} on channel {} ({})",
                        loadedSoundEffect->getID(), event.channel, Mix_GetError());

                  loadedSoundEffect.reset();
               }
            },

            [this](PlaySoundEffectEvent&& event)
            {
               Reference<SoundEffect> const& soundEffect{ event.soundEffect };

               if (not soundEffect.valid())
                  Logger::warn("failed to play sound effect (passed Reference to SoundEffect is not valid)");
               else
               {
                  int& channel{ soundEffect->mChannel };
                  channel = Mix_PlayChannel(event.channel, soundEffect->getImplementation().getSDLSoundEffect(), 0);
                  if (channel not_eq -1)
                  {
                     Logger::info("played SoundEffect with ID {} on channel {}!",
                        soundEffect->getID(), channel);

                     mActiveSoundEffects[channel] = soundEffect;
                     mLoadedSoundEffects[channel].reset();
                  }
                  else if (event.channel == -1)
                     Logger::warn("failed to play SoundEffect with ID {} on first free channel ({})",
                        soundEffect->getID(), Mix_GetError());
                  else
                     Logger::warn("failed to play SoundEffect with ID {} on channel {} ({})",
                        soundEffect->getID(), event.channel, Mix_GetError());
               }
            },

            [this](PauseSoundEffectEvent&& event)
            {
               if (event.channel not_eq -1)
                  internalPauseSoundEffect(event.channel);
               else
               {
                  Logger::info("pausing all sound effect channels...");
                  for (int channel{}; channel < getAmountOfChannels(); ++channel)
                     internalPauseSoundEffect(channel);
               }
            },

            [this](ResumeSoundEffectEvent&& event)
            {
               if (event.channel not_eq -1)
                  internalResumeSoundEffect(event.channel);
               else
               {
                  Logger::info("resuming all sound effect channels...");
                  for (int channel{}; channel < getAmountOfChannels(); ++channel)
                     internalResumeSoundEffect(channel);
               }
            },

            [this](StopSoundEffectEvent&& event)
            {
               if (event.channel not_eq -1)
                  internalStopSoundEffect(event.channel);
               else
               {
                  Logger::info("stopping all sound effect channels...");
                  for (int channel{}; channel < getAmountOfChannels(); ++channel)
                     internalStopSoundEffect(channel);
               }
            }
         }
      }
    , mActiveSoundEffects(getAmountOfChannels())
    , mLoadedSoundEffects(getAmountOfChannels())
   {
      if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1)
         exception("failed to open audio device ({})", Mix_GetError());

      Logger::info("initialized Audio!");
   }

   Audio::~Audio()
   {
      Mix_CloseAudio();

      mRunThread = false;
      mConditionVariable.notify_one();

      Logger::info("shut down Audio!");
   }

   void Audio::playMusic(std::string filePath)
   {
      pushEvent<LoadPlayMusicEvent>(std::move(filePath));
   }

   void Audio::playMusic(Reference<Music> const music)
   {
      pushEvent<PlayMusicEvent>(music);
   }

   void Audio::pauseMusic()
   {
      pushEvent<PauseMusicEvent>();
   }

   void Audio::resumeMusic()
   {
      pushEvent<ResumeMusicEvent>();
   }

   void Audio::stopMusic()
   {
      pushEvent<StopMusicEvent>();
   }

   Reference<Music> Audio::getActiveMusic()
   {
      return mActiveMusic;
   }

   void Audio::playSoundEffect(std::string filePath, int const channel)
   {
      if (channel < -1 or channel >= getAmountOfChannels())
         exception("channel {} is out of range!", channel);

      pushEvent<LoadPlaySoundEffectEvent>(std::move(filePath), channel);
   }

   void Audio::playSoundEffect(Reference<SoundEffect> const soundEffect, int const channel)
   {
      if (channel < -1 or channel >= getAmountOfChannels())
         exception("channel {} is out of range!", channel);

      pushEvent<PlaySoundEffectEvent>(soundEffect, channel);
   }

   void Audio::pauseSoundEffect(int const channel)
   {
      if (channel < -1 or channel >= getAmountOfChannels())
         exception("channel {} is out of range!", channel);

      pushEvent<PauseSoundEffectEvent>(channel);
   }

   void Audio::resumeSoundEffect(int const channel)
   {
      if (channel < -1 or channel >= getAmountOfChannels())
         exception("channel {} is out of range!", channel);

      pushEvent<ResumeSoundEffectEvent>(channel);
   }

   void Audio::stopSoundEffect(int const channel)
   {
      if (channel < -1 or channel >= getAmountOfChannels())
         exception("channel {} is out of range!", channel);

      pushEvent<StopSoundEffectEvent>(channel);
   }

   Reference<SoundEffect> Audio::getActiveSoundEffect(int const channel)
   {
      assert(channel >= 0 and channel < getAmountOfChannels(),
         "channel {} is out of range!", channel);

      return mActiveSoundEffects[channel];
   }

   int Audio::getAmountOfChannels()
   {
      return MIX_CHANNELS;
   }

   void Audio::setMute(bool const mute)
   {
      mMuted = mute;
      Mix_MasterVolume(mMuted ? 0 : MIX_MAX_VOLUME);
      Mix_VolumeMusic(mMuted ? 0 : MIX_MAX_VOLUME);
   }

   bool Audio::isMuted()
   {
      return mMuted;
   }

   void Audio::internalPauseSoundEffect(int const channel)
   {
      Reference<SoundEffect> const& activeSoundEffect{ mActiveSoundEffects[channel] };

      if (not activeSoundEffect.valid())
         Logger::warn("failed to pause sound effect on channel {} (no active SoundEffect in channel)",
            channel);
      else if (Mix_Paused(channel))
         Logger::warn("failed to pause SoundEffect with ID {} on channel {} (channel is not playing)",
            activeSoundEffect->getID(), channel);
      else
         Logger::info("paused SoundEffect with ID {} on channel {}!",
            activeSoundEffect->getID(), channel);

      Mix_Pause(channel);
   }

   void Audio::internalResumeSoundEffect(int const channel)
   {
      Reference<SoundEffect> const& activeSoundEffect{ mActiveSoundEffects[channel] };

      if (not activeSoundEffect.valid())
         Logger::warn("failed to resume sound effect on channel {} (no active SoundEffect in channel)",
            channel);
      else if (not Mix_Paused(channel))
         Logger::warn("failed to resume SoundEffect with ID {} on channel {} (channel is already playing)",
            activeSoundEffect->getID(), channel);
      else
         Logger::info("paused SoundEffect with ID {} on channel {}!",
            activeSoundEffect->getID(), channel);

      Mix_Resume(channel);
   }

   void Audio::internalStopSoundEffect(int const channel)
   {
      Reference<SoundEffect> const& activeSoundEffect{ mActiveSoundEffects[channel] };

      if (not activeSoundEffect.valid())
         Logger::warn("failed to stop sound effect on channel {} (no active sound effect)",
            channel);
      else
         Logger::info("stopped SoundEffect with ID {} on channel {}!",
            activeSoundEffect->getID(), channel);

      Mix_HaltChannel(channel);
      mLoadedSoundEffects[channel].reset();
   }
}