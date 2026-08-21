#pragma once
#include <QtGlobal>

namespace Const {

// ---------- Scene & grid ----------
inline constexpr int GRID_W = 20;
inline constexpr int GRID_H = 18;
inline constexpr int TILE   = 32;
inline constexpr int SCENE_W = GRID_W * TILE;
inline constexpr int SCENE_H = GRID_H * TILE;

// Each small raster box (pixel)
inline constexpr int RASTER = 5;

// ---------- Timing ----------
inline constexpr int FPS = 60;
inline constexpr int TICK_MS = 1000 / FPS;

// ---------- Player ----------
inline constexpr int PLAYER_Y = SCENE_H - 2 * TILE;
inline constexpr int PLAYER_COOLDOWN_MS = 160;
inline constexpr int PLAYER_MAX_HP = 100;
inline constexpr int PLAYER_LIVES = 3;
inline constexpr int PLAYER_INVULN_MS = 1200;

// ---------- Bullets ----------
inline constexpr qreal BULLET_SPEED = 2.0 * RASTER;
inline constexpr int   BULLET_W_CELLS = 1;
inline constexpr int   BULLET_H_CELLS = 3;

// ---------- Asteroids ----------
inline constexpr qreal AST_MIN_SPEED = 1.2;
inline constexpr qreal AST_MAX_SPEED = 2.8;
inline constexpr int   AST_LEAK_DMG_DEFAULT = 12;
inline constexpr int   AST_SCORE_SMALL = 30;
inline constexpr int   AST_SCORE_MED   = 20;
inline constexpr int   AST_SCORE_LARGE = 10;

// ---------- Mines ----------
inline constexpr qreal MINE_SPEED = 1.6;
inline constexpr qreal MINE_SINE_AMPL = 18.0;
inline constexpr qreal MINE_SINE_FREQ = 0.045;

// ---------- FX ----------
inline constexpr int EXP_FRAMES = 14;
inline constexpr int EXP_DEBRIS = 12;

// ---------- Spawning ----------
inline constexpr int INIT_AST_SPAWN_MS = 1300;
inline constexpr int INIT_MINE_SPAWN_MS = 2600;
inline constexpr int MIN_AST_SPAWN_MS = 650;
inline constexpr int MIN_MINE_SPAWN_MS = 1200;
inline constexpr int DIFF_STEP_MS_AST = 45;
inline constexpr int DIFF_STEP_MS_MINE = 45;
inline constexpr int DIFF_STEP_PERIOD_MS = 2200;

inline constexpr int SHAKE_MS = 220;
inline constexpr qreal SHAKE_INTENSITY = 6.0;



} // namespace Const
