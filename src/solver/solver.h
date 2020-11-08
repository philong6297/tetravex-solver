#ifndef SOLVER_SOLVER_H_
#define SOLVER_SOLVER_H_

#include <atomic>
#include <chrono>
#include "engine/game.h"
#include "solver/thread_pool.h"

class Solver {
 public:
  // single thread backtrack
  void SequentialBacktracking(Game& initial_game);

  // recursive backtrack, solve_next_piece
  void SolveNextPieceRecursive(BoardPosition position,
                               Game& current_game,
                               std::vector<bool>& is_used);

  // parallel backtrack task
  void ParallelBacktrackingTask(const Game& initial_game, Game::PieceId initial_piece_id);

  // parallel backtrack
  void ParallelBacktracking(const Game& initial_game, ThreadPool& pool);

 private:
  // atomic, lock-free concept
  std::atomic<bool> found_solution_{false};
  std::chrono::high_resolution_clock::time_point start_;
};

#endif   // !SOLVER_SOLVER_H_
