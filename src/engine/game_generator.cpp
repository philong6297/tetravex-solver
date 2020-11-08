#include "engine/game_generator.h"
#include <fstream>
#include <iostream>

Game GameGenerator::ReadFromFile(const std::string& path) {
  size_t size = 0;
  std::ifstream infile(path);
  if (!infile.is_open()) {
    std::cerr << "cannot open" << std::endl;
  }
  int32_t left, top, right, bottom;
  infile >> size >> size;

  Game game(size);

  Game::PieceId piece_id = 0;
  while (infile >> left >> top >> right >> bottom) {
    game.SetPieceData(piece_id, Piece{top, left, bottom, right});
    ++piece_id;
  }

  return game;
}
