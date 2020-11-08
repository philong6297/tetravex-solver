#include "engine/game.h"
#include <stddef.h>
#include <iostream>
#include <string>

Game::Game() : Game(0) {}

Game::Game(const size_t size) :
  size_(size),
  pieces_(size * size, Piece()),         // initialize pieces with default data
  board_(size * size, kInvalidPieceId)   // initislize empty board
{}

void Game::SetPieceData(const PieceId piece_id, const Piece& new_piece_data) {
  pieces_[piece_id] = new_piece_data;
}

Piece Game::GetPieceData(PieceId piece_id) const {
  return pieces_[piece_id];
}

bool Game::TryMovePieceToBoard(const PieceId piece_id, const BoardPosition position) {
  if (CanMovePieceToBoard(piece_id, position)) {
    PieceAtBoard(position) = piece_id;
    return true;
  }

  return false;
}

bool Game::CanMovePieceToBoard(const PieceId piece_id, const BoardPosition position) const {
  // empty game is not possible to move
  if (size_ == 0) {
    return false;
  }

  // position is out of range
  if (IsInvalidBoardPosition(position)) {
    return false;
  }

  // the board already have valid piece at this position
  if (PieceAtBoard(position) != kInvalidPieceId) {
    return false;
  }

  const Piece new_piece = GetPieceData(piece_id);

  //
  // game rule condition on neighbors with bound checking
  //

  const int32_t row_id = position.row_id;
  const int32_t col_id = position.col_id;

  const BoardPosition top_neighbor_position = {row_id - 1, col_id};
  // check bound of neighbor
  if (!IsInvalidBoardPosition(top_neighbor_position)) {
    // get piece id at that position
    const PieceId& top_neighbor_piece_id = PieceAtBoard(top_neighbor_position);
    //
    if (top_neighbor_piece_id != kInvalidPieceId   // if neighbor piece_id exist in board
        && new_piece.top !=
             GetPieceData(top_neighbor_piece_id).bottom   // new top should be = neighbor bottom
    ) {
      return false;
    }
  }

  const BoardPosition bottom_neighbor_position = {row_id + 1, col_id};
  if (!IsInvalidBoardPosition(bottom_neighbor_position)) {
    const PieceId& bottom_neighbor_piece_id = PieceAtBoard(bottom_neighbor_position);
    if (bottom_neighbor_piece_id != kInvalidPieceId &&
        new_piece.bottom != GetPieceData(bottom_neighbor_piece_id).top) {
      return false;
    }
  }

  const BoardPosition left_neighbor_position = {row_id, col_id - 1};
  if (!IsInvalidBoardPosition(left_neighbor_position)) {
    const PieceId& left_neighbor_piece_id = PieceAtBoard(left_neighbor_position);
    if (left_neighbor_piece_id != kInvalidPieceId &&
        new_piece.left != GetPieceData(left_neighbor_piece_id).right) {
      return false;
    }
  }

  const BoardPosition right_neighbor_position = {row_id, col_id + 1};
  if (!IsInvalidBoardPosition(right_neighbor_position)) {
    const PieceId& right_neighbor_piece_id = PieceAtBoard(right_neighbor_position);
    if (right_neighbor_piece_id != kInvalidPieceId &&
        new_piece.right != GetPieceData(right_neighbor_piece_id).left) {
      return false;
    }
  }

  return true;
}

Game::PieceId Game::ExtractPieceFromBoard(const BoardPosition position) {
  // get the id
  PieceId& piece_id_at_pos   = PieceAtBoard(position);
  // temp var to save old id
  PieceId extracted_piece_id = piece_id_at_pos;
  // id = invalid ~ empty
  piece_id_at_pos            = kInvalidPieceId;
  // return temp
  return extracted_piece_id;
}

// return reference ~ the return object is mutable
Game::PieceId& Game::PieceAtBoard(const BoardPosition position) {
  // convert 2D to 1D
  return board_.at(position.row_id * size_ + position.col_id);
}

// return const ref ~ immutable
const Game::PieceId& Game::PieceAtBoard(const BoardPosition position) const {
  return board_[position.row_id * size_ + position.col_id];
}

// bound checking
bool Game::IsInvalidBoardPosition(BoardPosition position) const {
  return position.row_id < 0 || position.col_id < 0 || position.row_id > size_ - 1 ||
         position.col_id > size_ - 1;
}

void Game::DisplayBoard() const {
  for (int32_t row_id = 0; row_id < size_; ++row_id) {
    std::string lines[4] = {"", "", "", ""};
    for (int32_t col_id = 0; col_id < size_; ++col_id) {
      std::string values[4]  = {"x", "x", "x", "x"};
      const PieceId piece_id = PieceAtBoard({row_id, col_id});
      if (piece_id != kInvalidPieceId) {
        const Piece& piece = GetPieceData(piece_id);
        values[0]          = std::to_string(piece.top);
        values[1]          = std::to_string(piece.left);
        values[2]          = std::to_string(piece.right);
        values[3]          = std::to_string(piece.bottom);
      }
      lines[0] += "|---";
      lines[1] += "| " + values[0] + " ";
      lines[2] += "|" + values[1] + " " + values[2];
      lines[3] += "| " + values[3] + " ";
    }
    for (std::string s : lines) {
      std::cout << s + "|" << std::endl;
    }
  }
}

bool Game::Win() const {
  for (const PieceId piece_id : board_) {
    if (piece_id == kInvalidPieceId) {
      return false;
    }
  }
  return true;
}

Game Game::Clone() const {
  Game clone(size_);
  clone.board_.assign(board_.cbegin(), board_.cend());
  clone.pieces_.assign(pieces_.cbegin(), pieces_.cend());
  return clone;
}
