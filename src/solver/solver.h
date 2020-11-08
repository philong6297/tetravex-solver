#ifndef SOLVER_SOLVER_H_
#define SOLVER_SOLVER_H_

#include <atomic>
#include <chrono>
#include "engine/game.h"
#include "solver/thread_pool.h"

class Solver {
 public:
  void SequentialBacktracking(Game& initial_game);

  void SolveNextPieceRecursive(BoardPosition position,
                               Game& current_game,
                               std::vector<bool>& is_used);

  void ParallelBacktrackingTask(const Game& initial_game, Game::PieceId initial_piece_id);

  void ParallelBacktracking(const Game& initial_game, ThreadPool& pool);

 private:
  std::atomic<bool> found_solution_{false};
  std::chrono::high_resolution_clock::time_point start_;
};

#endif   // !SOLVER_SOLVER_H_
