#include <iostream>
#include "engine/game_generator.h"
#include "solver/solver.h"

int main() {
  std::ios::sync_with_stdio(false);
  // absolute dir = full name of the path
  Game game = GameGenerator::ReadFromFile("D:/cpp-projects/tetravex-solver/data/6x6.txt");

  // {
  //   Solver solver;
  //   std::cout << "sequential:\n";
  //   solver.SequentialBacktracking(game);
  // }

  {
    ThreadPool pool(std::thread::hardware_concurrency());
    std::cout << "parallel:\n";
    Solver solver;
    solver.ParallelBacktracking(game, pool);
  }

  return 0;
}
