#ifndef ENGINE_GAME_GENERATOR_H_
#define ENGINE_GAME_GENERATOR_H_

#include <string>
#include "engine/game.h"

struct GameGenerator {
  static Game ReadFromFile(const std::string& path);
};

#endif   // !ENGINE_GAME_GENERATOR_H_
