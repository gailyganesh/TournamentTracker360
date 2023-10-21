#include "backendToCreateMatch.h"
#include <algorithm>
#include <random>
#include <string>
#include <QDebug>

static constexpr uint8_t NO_OF_PLAYERS_IN_TEAM=2;

using namespace Base::Match;

#include <iostream>

// Function to calculate the factorial of a number
uint64_t factorial(int n) {
    if (n <= 1) {
        return 1;
    }
    return n * factorial(n - 1);
}

// Function to calculate combinations (n choose k)
uint64_t Combination(int n, int k) {
    if (n < k || n < 0 || k < 0) {
        return 0; // Invalid input
    }
    return factorial(n) / (factorial(k) * factorial(n - k));
}

Base::Player::Player(int number, std::string name)
    :mNumber(number)
    ,mName(name)
    ,mPoints(0)
    ,mWinMatches(0)
{
}

Base::Team::Team(std::vector<Player> members)
    :mMembers(members)
{
}


bool Base::Player::operator==(const Base::Player& opponent) const
{
    return(mNumber==opponent.mNumber) && (CaseInsensitiveCompare(mName,opponent.mName));
}

bool Base::Team::operator==(const Base::Team& opponent) const
{
    for(auto& player:mMembers)
    {
        if(std::find(opponent.mMembers.begin(),opponent.mMembers.end(),player)==opponent.mMembers.end())
        {
            return false;
        }
    }
    return true;
}

bool MatchCreator::IsMatchCalculationPossible()
{
//    auto no_Teams=mTeamList.size();
//    mMaxMatches=Combination(no_Teams, 2);
//    if(mNo_Matches <= mMaxMatches)
//    {
//        return true;
//    }
    auto no_Players=mPlayersList.size();
    mMaxMatches=(no_Players*(no_Players-1))/(NO_OF_PLAYERS_IN_TEAM+NO_OF_PLAYERS_IN_TEAM);

    if(mNo_Matches<=mMaxMatches)
    {
        return true;
    }
    return false;
}

void MatchCreator::CreateMaxMatchList()
{
    for (uint i = 0; i < mTeamList.size(); i++)
    {
        auto team1 = mTeamList[i];
        if (IsTeamAlreadyScheduled(team1))
        {
            continue;
        }
        for (uint j = i + 1; j < mTeamList.size(); j++)
        {
            auto team2 = mTeamList[j];
            if (IsTeamAlreadyScheduled(team2) || IsBothTeamsHaveSamePlayer(team1, team2))
            {
                continue;
            }
            mMatches.push_back({ team1,team2 });
            mScheduledTeams.push_back(team1);
            mScheduledTeams.push_back(team2);
            break;
        }
    }
}

bool MatchCreator::IsPlayerAlreadyExist(const Base::Player& player)const
{
    auto it = std::find(mPlayersList.begin(), mPlayersList.end(), player);
    if(it != mPlayersList.end())
    {
        return true;
    }
    return false;
}

bool MatchCreator::IsBothTeamsHaveSamePlayer(Team& team1, Team& team2)
{
    for (auto& player:team2.mMembers)
    {
        if(std::find(team1.mMembers.begin(),team1.mMembers.end(),player)!=team1.mMembers.end())
        {
            return true;
        }
    }
    return false;
}

void MatchCreator::GenerateTeams(bool isRepeatationAllowed)
{
    auto no_Players=mPlayersList.size();
    auto no_Teams=Combination(no_Players, NO_OF_PLAYERS_IN_TEAM);

    for(int i=0; i<no_Players-1; ++i)
    {
        for (int j=i+1; j<no_Players; ++j)
        {
            mTeamList.push_back(Team({mPlayersList[i],mPlayersList[j]}));
        }
    }
    if(mTeamList.size()!=no_Teams)
    {
        std::runtime_error("TeamList is not prepared correctely");
    }
}

void MatchCreator::CalculateMatches(bool isRepeatationAllowed)
{
    std::random_device rd;
    std::mt19937 g(rd());
    while (IsMatchesNeedsRecalculation())
    {
        mScheduledTeams.clear();
        mMatches.clear();
        std::shuffle(mTeamList.begin(), mTeamList.end(), g);
        CreateMaxMatchList();
        //qDebug() << "calculated number of matches: " << mMatches.size();
    }
    if (isRepeatationAllowed)
    {
        while (mMatches.size() != mNo_Matches)
        {
            mMatches.push_back(GetMatch());
        }
    }
    std::shuffle(mMatches.begin(), mMatches.end(), g);
    while (mMatches.size() != mNo_Matches)
    {
        if (mMatches.size() < mNo_Matches)
        {
            std::runtime_error("Number asked to create was failed");
            return;
        }
        
        mMatches.pop_back();
    }

    //for(int matchIdx=0; matchIdx<mNo_Matches; matchIdx++)
    //{
    //    //if(mTeamList.size()==2)
    //    //{
    //    //    if(IsBothTeamsHaveSamePlayer(mTeamList[0],mTeamList[1]))
    //    //    {
    //    //        //need to reset everything
    //    //        mTeamList.clear();
    //    //        mScheduledTeams.clear();
    //    //        mMatches.clear();
    //    //        GenerateTeams(isRepeatationAllowed);
    //    //        CalculateMatches(isRepeatationAllowed);
    //    //    }
    //    //}
    //    auto match=GetMatch(isRepeatationAllowed, matchIdx);
    //    mMatches.push_back(match);
    //    /*mTeamList.erase(std::remove(mTeamList.begin(), mTeamList.end(), match.first),mTeamList.end());
    //    mTeamList.erase(std::remove(mTeamList.begin(), mTeamList.end(), match.second),mTeamList.end());*/

    //    qDebug()<<"TeamList size: "<<mTeamList.size();

    //    mScheduledTeams.push_back(match.first);
    //    mScheduledTeams.push_back(match.second);
    //}



}

bool MatchCreator::IsTeamAlreadyScheduled(Team team)
{
//    if(scheduleTeamIndices.empty())
//    {
//        if(std::find(scheduleTeamIndices.begin(),scheduleTeamIndices.end(),index)!=scheduleTeamIndices.end())
//        {
//            return true;
//        }
//        return false;
//    }
    for(auto& itr: mMatches)
    {
        if(itr.first==team || itr.second==team)
        {
            return true;
        }
    }
    return false;
}

tMatch MatchCreator::GetMatch(bool isRepeatationAllowed, int createdMatchCount)
{
    auto firstTeam=GetRandomTeam();
//    while (IsTeamAlreadyScheduled(firstTeam))
//    {
//        firstTeam=GetRandomTeam();
//    }
    auto secondTeam=GetRandomTeam();
//    while (IsTeamAlreadyScheduled(secondTeam) || IsBothTeamsHaveSamePlayer(firstTeam,secondTeam))
//    {
//        secondTeam=GetRandomTeam();
//    }
    while(IsBothTeamsHaveSamePlayer(firstTeam,secondTeam))
    {
        secondTeam=GetRandomTeam();
    }
    tMatch match={firstTeam,secondTeam};

    //if(!(createdMatchCount>mMaxMatches) && !isRepeatationAllowed)
    //{
        while(IsMatchAlreadyExist(match))
        {
            return GetMatch();
        }
    //}
    return match;
}

bool MatchCreator::IsMatchAlreadyExist(tMatch& match)
{
    if(!mMatches.empty())
    {
        auto itr=std::find(mMatches.begin(),mMatches.end(),match);
        std::swap(match.first,match.second);
        auto itr_swap=std::find(mMatches.begin(),mMatches.end(),match);
        if(itr!=mMatches.end()||itr_swap!=mMatches.end())
        {
            return true;
        }
    }
    return false;
}

bool MatchCreator::IsMatchesNeedsRecalculation()
{
    return !((mMatches.size() == mMaxMatches) || (mMatches.size() >= mNo_Matches));
}

bool MatchCreator::AddToPlayersList(int number, std::string name)
{
    auto player = Base::Player(number,name);
    if(IsPlayerAlreadyExist(player))
    {
        return false;
    }
    mPlayersList.push_back(player);
    return true;
}

void MatchCreator::RestPlayerList()
{
    mPlayersList.clear();
    mTeamList.clear();
}

void MatchCreator::RestMatches()
{
    mMatches.clear();
}

std::vector<Base::Player> MatchCreator::GetPlayerList()
{
    return mPlayersList;
}

Base::Team MatchCreator::GetRandomTeam()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, mTeamList.size() - 1);

    int randomIndex = dis(gen);
    auto randomIter = std::next(mTeamList.begin(), randomIndex);

    return *randomIter;
}


bool Base::Player::CaseInsensitiveCompare(const std::string& str1, const std::string& str2) const
{
    return str1.size() == str2.size() &&
           std::equal(str1.begin(), str1.end(), str2.begin(),
                      [](char c1, char c2) {
                          return std::toupper(c1) == std::toupper(c2);
                      });
}

