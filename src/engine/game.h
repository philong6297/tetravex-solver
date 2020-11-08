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
  using PieceId                            = int32_t;
  static constexpr PieceId kInvalidPieceId = -1;

  explicit Game();
  explicit Game(size_t size);

  void SetPieceData(PieceId piece_id, const Piece& new_piece_data);
  Piece GetPieceData(PieceId piece_id) const;

  size_t GetSize() const { return size_; }

  bool TryMovePieceToBoard(PieceId piece_id, BoardPosition position);
  bool CanMovePieceToBoard(PieceId piece_id, BoardPosition position) const;

  PieceId ExtractPieceFromBoard(BoardPosition position);

  PieceId& PieceAtBoard(BoardPosition position);
  const PieceId& PieceAtBoard(BoardPosition position) const;
  bool IsInvalidBoardPosition(BoardPosition position) const;

  void DisplayBoard() const;

  bool Win() const;

  Game Clone() const;

 private:
  const size_t size_ = 0;
  std::vector<Piece> pieces_{};
  std::vector<PieceId> board_{};
};

#endif   // !ENGINE_GAME_H_
