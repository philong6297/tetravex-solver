#ifndef ENGINE_GAME_H_
#define ENGINE_GAME_H_

#include <vector>

// Tetravex piece containing 4 values
struct Piece {
  int32_t top    = 0;
  int32_t left   = 0;
  int32_t bottom = 0;
  int32_t right  = 0;
};

struct BoardPosition {
  int32_t row_id;
  int32_t col_id;
};

class Game {
 public:
  using PieceId = int32_t;
  // invalid id ~ there is no piece in the board at a position
  static constexpr PieceId kInvalidPieceId = -1;

  Game();

  // set size for the game and initialize begin state of the game
  Game(size_t size);

  // set new data for piece which have piece_id
  void SetPieceData(PieceId piece_id, const Piece& new_piece_data);

  // return data of piece_id
  Piece GetPieceData(PieceId piece_id) const;

  // return size
  size_t GetSize() const { return size_; }

  // Try to move piece to the board if it sastisfies game condition. Return true if success.
  bool TryMovePieceToBoard(PieceId piece_id, BoardPosition position);

  // Condition checking
  bool CanMovePieceToBoard(PieceId piece_id, BoardPosition position) const;

  // Remove piece from board, board[postion] = kInvalid. return extracted id
  PieceId ExtractPieceFromBoard(BoardPosition position);

  // Get PieceId from board
  PieceId& PieceAtBoard(BoardPosition position);
  const PieceId& PieceAtBoard(BoardPosition position) const;

  // bound checking position
  bool IsInvalidBoardPosition(BoardPosition position) const;

  // print
  void DisplayBoard() const;

  // win condition
  bool Win() const;

  // Create a copy state
  Game Clone() const;

 private:
  const size_t size_ = 0;
  // pieces contain data ~ pieces = new Pieces[NxN]
  // piece = {{1,2,3,4}, {4,5,6,7}, ...}
  std::vector<Piece> pieces_{};
  // board track id of the piece ~ board = new Pieces[NxN] flatten
  std::vector<PieceId> board_{};
};

#endif   // !ENGINE_GAME_H_
