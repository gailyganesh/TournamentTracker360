#ifndef BACKENDTOCREATEMATCH_H
#define BACKENDTOCREATEMATCH_H

#include<map>
#include<utility>
#include<iostream>
#include<vector>

namespace Base
{
class Player
{
public:
    Player(int number=0, std::string name="");
    bool operator==(const Player& opponent) const;
public:
    int mNumber;
    std::string mName;
    int mPoints;
    int mWinMatches;
private:
    bool CaseInsensitiveCompare(const std::string& str1, const std::string& str2) const;
};

namespace Match
{
typedef std::pair<Player,Player> tMatch;

class MatchCreator
{
private:
    Player GetRandomPlayer();
    tMatch GetMatch(bool isRepeatationAllowed=false, int createdMatchCount=0);
    bool IsMatchAlreadyExist(tMatch& match);
    bool IsPlayerAlreadyExist(const Player& player)const;
    int mMaxMatches=0;
public:
    MatchCreator() = default;
    std::vector<tMatch> mMatches;
    std::vector<Player> mPlayersList;
    int mNo_Matches=0;
    bool IsMatchCalculationPossible();
    void CalculateMatches(bool isRepeatationAllowed=false);
    bool AddToPlayersList(int number, std::string name);
    void RestPlayerList();
    void RestMatches();
    std::vector<Player> GetPlayerList();
};
} //Match
} //Base
#endif // BACKENDTOCREATEMATCH_H
