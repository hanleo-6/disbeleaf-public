#pragma once

#include <iostream>
#include <fstream>
#include <sstream>


#include "Player.h"
#include "Camera.h"
#include "Alga.h"

class GameStructure {
    public:
        GameStructure();
        void StartNewRun(Player * player, AlgaeManager * algae_manager, Camera * camera);
        void UpdateScore(int score);
        int GetHighscore();
        int GetScore();
        int GetTime();
        bool IsGameOver();
        bool InRun();
        void TickTime(float dt);
        bool Save(bool load);

        
    private:
        int mCurScore;
        int mHighscore;
        // need both mInRun and mGameIsOver as first start needs different behaviour
        bool mInRun;
        bool mGameIsOver;
        float mRunTime; // keeps track of current run's time left
        const float MAX_RUN_TIME = 31.0f; // 1 above 30 as initial tick is instant
};

