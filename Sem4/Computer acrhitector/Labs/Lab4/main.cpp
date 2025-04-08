#include <iostream>
#include <chrono>
#include <thread>

#include "fmod.hpp"
#include "fmod_errors.h"

void ERRCHECK(FMOD_RESULT result)
{
  if (result != FMOD_OK)
  {
    std::cerr << "FMOD error: " << FMOD_ErrorString(result) << '\n';
    exit(-1);
  }
}

int main()
{
  //first
/*  {
    std::cout << "First task: just playing sounds from mp3 file\n";
    FMOD::System * system = nullptr;
    FMOD::Sound * sound = nullptr;
    FMOD::Channel * channel = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    result = system->createSound("Lab4/music.mp3", FMOD_DEFAULT, nullptr, &sound);
    ERRCHECK(result);
    result = system->playSound(sound, nullptr, false, &channel);
    std::cout << "Press Enter to stop\n";
    std::cin.get();
    sound->release();
    system->close();
    system->release();
  }*/

  //second
  {
    std::cout << "Second task: play 3d sound\n";
    FMOD::System * system = nullptr;
    FMOD::Sound * sound = nullptr;
    FMOD::Channel * channel = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_3D_RIGHTHANDED, nullptr);
    result = system->createSound("Lab4/music.mp3", FMOD_3D, nullptr, &sound);
    ERRCHECK(result);
    result = sound->setMode(FMOD_3D);
    result = sound->set3DMinMaxDistance(1.0f, 100.0f);
    FMOD_VECTOR listenerPos = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR listenerVel = {0.0f, 0.0f, 0.0f};
    result = system->set3DListenerAttributes(0, &listenerPos, &listenerVel, nullptr, nullptr);
    FMOD_VECTOR soundPos = {0.0f, 0.0f, 0.0f};
    FMOD_VECTOR soundVel = {0.0f, 0.0f, 0.0f};
    result = system->playSound(sound, 0, false, &channel);
    result = channel->set3DAttributes(&soundPos, &soundVel);
    for (float x = -1000.0f; x <= 1000.0f; x += 10.0f) {
      soundPos.x = x;
      result = channel->set3DAttributes(&soundPos, &soundVel);
      system->update();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    std::cout << "Press Enter to exit...\n";
    std::cin.get();
    sound->release();
    system->close();
    system->release();
  }

  //third
  {
    std::cout << "Third task: play sound with different parameters\n";
    FMOD::System* system = nullptr;
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
    FMOD::DSP* echoDSP = nullptr;
    FMOD_RESULT result = FMOD::System_Create(&system);
    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
    result = system->createSound("Lab4/music.mp3", FMOD_DEFAULT, nullptr, &sound);
    ERRCHECK(result);
    result = system->playSound(sound, nullptr, false, &channel);
    result = system->createDSPByType(FMOD_DSP_TYPE_ECHO, &echoDSP);
    result = echoDSP->setParameterFloat(FMOD_DSP_ECHO_DELAY, 500.0f);   // Задержка (в мс)
    result = echoDSP->setParameterFloat(FMOD_DSP_ECHO_FEEDBACK, 1.0f); // Количество повторов (отражений)
    result = echoDSP->setParameterFloat(FMOD_DSP_ECHO_WETLEVEL, 1.0f); // Громкость отражений
    result = echoDSP->setParameterFloat(FMOD_DSP_ECHO_DRYLEVEL, 50.0f); // Громкость оригинального звука
    result = channel->addDSP(0, echoDSP);
    float volume = 1.0f;
    float pitch = 1.0f;
    char command;
    std::cout << "Press '1' to increase volume, '2' to decrease volume, '3' to increase pitch, '4' to decrease pitch, 'q' to quit.\n";
    while (true)
    {
      bool isPlaying;
      channel->isPlaying(&isPlaying);
      if (!isPlaying)
      {
        break;
      }
      std::cin >> command;
      if (command == '1')
      {
        if (volume < 1.0f)
        {
          volume += 0.1f;
        }
      } else if (command == '2')
      {
        if (volume > 0.0f)
        {
          volume -= 0.1f;
        }
      }
      else if (command == '3')
      {
        if (pitch < 2.0f)
        {
          pitch += 0.1f;
        }
      }
      else if (command == '4')
      {
        if (pitch > 0.5f)
        {
          pitch -= 0.1f;
        }
      }
      else if (command == 'q')
      {
        break;
      }
      result = channel->setVolume(volume);
      result = channel->setPitch(pitch);

      ERRCHECK(result);
      std::cout << "Current volume: " << volume * 100 << "%;" << " current speed: " << pitch * 100 << "%\n";
    }
    echoDSP->release();
    sound->release();
    system->close();
    system->release();
  }
  return 0;
}
