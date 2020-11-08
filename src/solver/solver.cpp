#include "solver/solver.h"
#include <cassert>
#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>

namespace {
  namespace chrono = std::chrono;
}

void Solver::SequentialBacktracking(Game& initial_game) {
  const size_t size = initial_game.GetSize();
  std::vector<bool> is_used(size * size, false);
  found_solution_ = false;
  // game->DisplayBoard();

  start_ = chrono::high_resolution_clock::now();
  SolveNextPieceRecursive({0, 0}, initial_game, is_used);
}

void Solver::SolveNextPieceRecursive(const BoardPosition position,
                                     Game& current_game,
                                     std::vector<bool>& is_used) {
  if (found_solution_) {
    return;
  }

  if (current_game.Win()) {
    found_solution_ = true;

    auto resolve_time_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::high_resolution_clock::now() - start_);
    std::cout << "Resolved in " << resolve_time_ms.count() << "ms\n";

    // current_game.DisplayBoard();
    return;
  }

  for (Game::PieceId piece_id = 0; piece_id < is_used.size(); ++piece_id) {
    if (is_used.at(piece_id)) {
      continue;
    }

    if (current_game.TryMovePieceToBoard(piece_id, position)) {
      int32_t next_row_id  = position.row_id;
      int32_t next_cold_id = position.col_id + 1;
      if (position.col_id == current_game.GetSize() - 1) {
        ++next_row_id;
        next_cold_id = 0;
      }
      is_used.at(piece_id) = true;

      SolveNextPieceRecursive({next_row_id, next_cold_id}, current_game, is_used);
      if (found_solution_) {
        return;
      }

      current_game.ExtractPieceFromBoard(position);
      is_used.at(piece_id) = false;
    }
  }
}

void Solver::ParallelBacktrackingTask(const Game& initial_game,
                                      const Game::PieceId initial_piece_id) {
  if (found_solution_) {
    return;
  }

  Game current_game = initial_game.Clone();
  const size_t size = initial_game.GetSize();
  std::vector<bool> is_used(size * size, false);

  current_game.TryMovePieceToBoard(initial_piece_id, {0, 0});
  is_used.at(initial_piece_id) = true;

  SolveNextPieceRecursive({0, 1}, current_game, is_used);
}

void Solver::ParallelBacktracking(const Game& game, ThreadPool& pool) {
  found_solution_ = false;
  // game->DisplayBoard();
  const size_t size = game.GetSize();

  start_ = std::chrono::high_resolution_clock::now();
  for (Game::PieceId id = 0; id < size * size; ++id) {
    if (!found_solution_) {
      pool.push(&Solver::ParallelBacktrackingTask, this, game, id);
    }
  }
}
