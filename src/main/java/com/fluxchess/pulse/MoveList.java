/*
 * Copyright (C) 2013-2014 Phokham Nonava
 *
 * Use of this source code is governed by the MIT license that can be
 * found in the LICENSE file.
 */
package com.fluxchess.pulse;

import java.lang.reflect.Array;

/**
 * This class stores our moves for a specific position. For the root node we
 * will populate pv for every root move.
 */
final class MoveList<T extends MoveList.MoveEntry> {

  private static final int MAX_MOVES = 256;

  final T[] entries;
  int size = 0;

  static final class MoveVariation {
    final int[] moves = new int[Depth.MAX_PLY];
    int size = 0;
  }

  static class MoveEntry {
    int move = Move.NOMOVE;
    int value = Value.NOVALUE;
  }

  static final class RootEntry extends MoveEntry {
    final MoveVariation pv = new MoveVariation();
  }

  MoveList(Class<T> clazz) {
    @SuppressWarnings("unchecked")
    final T[] entries = (T[]) Array.newInstance(clazz, MAX_MOVES);
    this.entries = entries;
    try {
      for (int i = 0; i < entries.length; ++i) {
        entries[i] = clazz.newInstance();
      }
    } catch (InstantiationException | IllegalAccessException e) {
      throw new IllegalStateException(e);
    }
  }

  /**
   * Sorts the move list using a stable insertion sort.
   */
  void sort() {
    for (int i = 1; i < size; ++i) {
      T entry = entries[i];

      int j = i;
      while ((j > 0) && (entries[j - 1].value < entry.value)) {
        entries[j] = entries[j - 1];
        --j;
      }

      entries[j] = entry;
    }
  }

  /**
   * Rates the moves in the list according to "Most Valuable Victim - Least Valuable Aggressor".
   */
  void rateFromMVVLVA() {
    for (int i = 0; i < size; ++i) {
      int move = entries[i].move;
      int value = 0;

      int piecetypeValue = PieceType.getValue(Piece.getType(Move.getOriginPiece(move)));
      value += PieceType.KING_VALUE / piecetypeValue;

      int target = Move.getTargetPiece(move);
      if (Piece.isValid(target)) {
        value += 10 * PieceType.getValue(Piece.getType(target));
      }

      assert value >= (PieceType.KING_VALUE / PieceType.KING_VALUE)
          && value <= (PieceType.KING_VALUE / PieceType.PAWN_VALUE) + 10 * PieceType.QUEEN_VALUE;

      entries[i].value = value;
    }
  }

}
