/*
 * Copyright (C) 2013-2014 Phokham Nonava
 *
 * Use of this source code is governed by the MIT license that can be
 * found in the LICENSE file.
 */
#ifndef PULSE_SEARCH_H
#define PULSE_SEARCH_H

#include "protocol.h"
#include "position.h"
#include "movegenerator.h"
#include "evaluation.h"

#include <memory>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace pulse {

/**
 * This class implements our search in a separate thread to keep the main
 * thread available for more commands.
 */
class Search {
public:
  Search(Protocol& protocol);

  void newDepthSearch(Position& position, int searchDepth);
  void newNodesSearch(Position& position, uint64_t searchNodes);
  void newTimeSearch(Position& position, uint64_t searchTime);
  void newInfiniteSearch(Position& position);
  void newClockSearch(Position& position,
    uint64_t whiteTimeLeft, uint64_t whiteTimeIncrement, uint64_t blackTimeLeft, uint64_t blackTimeIncrement, int movesToGo);
  void newPonderSearch(Position& position,
    uint64_t whiteTimeLeft, uint64_t whiteTimeIncrement, uint64_t blackTimeLeft, uint64_t blackTimeIncrement, int movesToGo);
  void reset();
  void start();
  void stop();
  void ponderhit();
  void quit();
  void run();

private:
  /**
   * This is our search timer for time & clock & ponder searches.
   */
  class Timer {
  public:
    Timer(bool& timerStopped, bool& doTimeManagement, int& currentDepth, const int& initialDepth, std::atomic_bool &abort);

    void start(uint64_t searchTime);
    void stop();
  private:
    std::mutex mutex;
    std::condition_variable condition;
    std::thread thread;

    bool& timerStopped;
    bool& doTimeManagement;
    int& currentDepth;
    const int& initialDepth;

    std::atomic_bool& abort;

    void run(uint64_t searchTime);
  };

  class Semaphore {
  public:
    Semaphore(int permits);

    void acquire();
    void release();
    void drainPermits();
  private:
    int permits;
    std::mutex mutex;
    std::condition_variable condition;
  };

  std::thread thread;
  Semaphore wakeupSignal;
  Semaphore runSignal;
  Semaphore stopSignal;
  std::recursive_mutex sync;
  Protocol& protocol;
  bool running = false;
  bool shutdown = false;

  Position position;
  Evaluation evaluation;

  // We will store a MoveGenerator for each ply so we don't have to create them
  // in search. (which is expensive)
  std::array<std::array<MoveGenerator, Depth::MAX_PLY>, 500> moveGenerators;

  // Depth search
  int searchDepth;

  // Nodes search
  uint64_t searchNodes;

  // Time & Clock & Ponder search
  uint64_t searchTime;
  Timer timer;
  bool timerStopped;
  bool runTimer;
  bool doTimeManagement;

  // Search parameters
  MoveList<RootEntry> rootMoves;
  std::atomic_bool abort;
  std::atomic_uint_fast64_t totalNodes;
  const int initialDepth = 1;
  int currentDepth;
  std::atomic_int_fast32_t currentMaxDepth;
  int currentMove;
  int currentMoveNumber;
  std::array<MoveVariation, Depth::MAX_PLY + 1> pv;

  void checkStopConditions();
  void updateSearch(int ply);
  void searchRoot(int depth, int alpha, int beta);
  void searchThread(int i, Position position, int depth, std::atomic_int_fast32_t &alpha, int beta, std::mutex &mutex, std::array<MoveVariation, Depth::MAX_PLY + 1> pv);
  int search(Position &position, int depth, int alpha, int beta, int ply, std::array<MoveGenerator, Depth::MAX_PLY> &moveGenerators, std::array<MoveVariation, Depth::MAX_PLY + 1> &pv);
  int quiescent(Position &position, int depth, int alpha, int beta, int ply, std::array<MoveGenerator, Depth::MAX_PLY> &moveGenerators, std::array<MoveVariation, Depth::MAX_PLY + 1> &pv);
  void savePV(int move, MoveVariation& src, MoveVariation& dest);
};

}

#endif
