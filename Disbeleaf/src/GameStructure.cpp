#include "GameStructure.h"
using namespace std;

GameStructure::GameStructure() {
    mHighscore = 0;
    mGameIsOver = false;
    mInRun = false;
}

void GameStructure::StartNewRun(Player * player, AlgaeManager * algae_manager, Camera * camera) {
    // Reset game variables & objects (doesn't reset fish)
    mRunTime = MAX_RUN_TIME;
    mCurScore = 0;
    mGameIsOver = false;
    mInRun = true;
    player->Reset();
    camera->Reset(player->mPos);
    algae_manager->InitialGeneration();
}
void GameStructure::TickTime(float dt) {
    mRunTime -= dt;
    if (mRunTime <= 0.0f) {
        mInRun = false;
        mGameIsOver = true;
    }
}

void GameStructure::UpdateScore(int new_score) {
    mCurScore = new_score;
    mHighscore = std::max(mCurScore, mHighscore); // specify std as glm has max function too
}
int GameStructure::GetHighscore() {
    return mHighscore;
}
int GameStructure::GetScore() {
    return mCurScore;
}
int GameStructure::GetTime() {
    return (int) mRunTime;
}
bool GameStructure::IsGameOver() {
    return mGameIsOver;
}
bool GameStructure::InRun() {
    return mInRun;
}

bool GameStructure::Save(bool load) {
    // Save function which saves following data, whilst also handling corrupted save file
    // Info to save:
    // -----------
    // save file id_count
    // highscore
    // -----------
    // Save format:
    // save_id_counter,highscore
    // -----------
    string save_file_name = "disbeleaf_save_file";
    bool save_corrupted = false;
    int info_count = 2;
    string save_id_counter = "0";
    int saved_highscore = 0;
    string cur_contents;
    string new_contents = "";

    // get the (single) line in the save file
    ifstream cur_save_file(save_file_name);
    getline(cur_save_file, cur_contents);
    cur_save_file.close();

    stringstream ss(cur_contents);
    int i = 0;
    while (ss.good()) {
        string info;
        getline(ss, info, ' ');
        // make sure there is info
        if (info.length() == 0) {
            save_corrupted = true;
            break;
        }
        // make sure info is all digits
        for (int i = 0; i < info.length(); i++) {
            if (info[i] < '0' || info[i] > '9') {
                save_corrupted = true;
                break;
            } 
        }
        if (save_corrupted) break;

        switch (i) {
            case 0: // first item is save id counter
                save_id_counter = to_string(stoi(info)+1);
                i++;
                break;
            case 1: // second item is saved highscore
                saved_highscore = stoi(info);
                i++;
                break;
            default: // more than 2 items means save file is not in expected format
                save_corrupted = true;
                break;
        }
    }
    // final check for corrupt save file
    if (i > info_count) {
        save_corrupted = true;
    }
    if (save_corrupted) {
        cout << "Corrupted save file, overwriting with fresh save" << "\n";
    }

    mHighscore = std::max(saved_highscore, mHighscore); // specify std as glm has max function too
    
    if (load) {
        cout << "Game data loaded!" << "\n";
    }
    else {
        // since not loading, overwrite save file with updated data
        new_contents += save_id_counter + " " + to_string(mHighscore);
        ofstream output_save_file(save_file_name, ofstream::out | ofstream::trunc);
        output_save_file << new_contents;
        output_save_file.close();
        cout << "Game data saved!" << "\n";
    }
    return true;
}