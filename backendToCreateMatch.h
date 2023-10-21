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

class Team
{
public:
    Team(std::vector<Player> members);
    bool operator==(const Team& opponent) const;
    std::vector<Player> mMembers;
};

namespace Match
{
//typedef std::vector<Player> tTeam;
typedef std::pair<Base::Team,Base::Team> tMatch;

class MatchCreator
{
private:
    Base::Team GetRandomTeam();
    tMatch GetMatch(bool isRepeatationAllowed=false, int createdMatchCount=0);
    bool IsMatchAlreadyExist(tMatch& match);
    bool IsMatchesNeedsRecalculation();
    bool IsPlayerAlreadyExist(const Player& player)const;
    bool IsBothTeamsHaveSamePlayer(Base::Team& team1, Base::Team& team2);
    void CreateMaxMatchList();
    bool IsTeamAlreadyScheduled(Team team);
    uint64_t mMaxMatches=0;
    std::vector<Team>mScheduledTeams;

public:
    MatchCreator() = default;
    std::vector<Base::Team> mTeamList;
    std::vector<tMatch> mMatches;
    std::vector<Player> mPlayersList;
    void SetNoOfMatches(int no_Matches);
    bool IsMatchCalculationPossible();
    void GenerateTeams(bool isRepeatationAllowed=false);
    void CalculateMatches(bool isRepeatationAllowed=false);
    bool AddToPlayersList(int number, std::string name);
    void RestPlayerList();
    void RestMatches();
    std::vector<Player> GetPlayerList();
    int mNo_Matches=0;
};
} //Match
} //Base
#endif // BACKENDTOCREATEMATCH_H
