#include "engine/game_generator.h"
#include <fstream>
#include <iostream>

Game GameGenerator::ReadFromFile(const std::string& path) {
  size_t size = 0;
  // ~ reader
  std::ifstream infile(path);
  // check error if cannot open file
  if (!infile.is_open()) {
    std::cerr << "cannot open" << std::endl;
  }

  int32_t left, top, right, bottom;
  // read size
  infile >> size >> size;

  // create the game
  Game game(size);

  // set piece for game
  Game::PieceId piece_id = 0;
  while (infile >> left >> top >> right >> bottom) {
    // set piece_id with the piece data
    game.SetPieceData(piece_id, Piece{top, left, bottom, right});
    ++piece_id;
  }

  return game;
}
