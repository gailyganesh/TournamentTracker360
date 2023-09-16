#include "backendToCreateMatch.h"
#include <algorithm>
#include <random>
#include <string>

using namespace Base::Match;

Base::Player::Player(int number, std::string name)
    :mNumber(number)
    ,mName(name)
    ,mPoints(0)
    ,mWinMatches(0)
{
}

bool Base::Player::operator==(const Base::Player& opponent) const
{
    return(mNumber==opponent.mNumber) && (CaseInsensitiveCompare(mName,opponent.mName));
}

bool MatchCreator::IsMatchCalculationPossible()
{
    auto no_Players=mPlayersList.size();
    mMaxMatches=(no_Players * (no_Players-1))/2;
    if(mNo_Matches <= mMaxMatches)
    {
        return true;
    }
    return false;
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

void MatchCreator::CalculateMatches(bool isRepeatationAllowed)
{
    for(int matchIdx=0; matchIdx<mNo_Matches; matchIdx++)
    {
        auto match=GetMatch(isRepeatationAllowed, matchIdx);
        mMatches.push_back(match);
    }
    std::cout<<"No. of Matches:"<<mMatches.size()<<std::endl;
}

tMatch MatchCreator::GetMatch(bool isRepeatationAllowed, int createdMatchCount)
{
    auto firstPlayer=GetRandomPlayer();
    auto secondPlayer=GetRandomPlayer();
    while(firstPlayer==secondPlayer)
    {
        secondPlayer=GetRandomPlayer();
    }
    tMatch match={firstPlayer,secondPlayer};

    if(!(createdMatchCount>mMaxMatches) && !isRepeatationAllowed)
    {
        while(IsMatchAlreadyExist(match))
        {
            return GetMatch();
        }
    }
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
}

void MatchCreator::RestMatches()
{
    mMatches.clear();
}

std::vector<Base::Player> MatchCreator::GetPlayerList()
{
    return mPlayersList;
}

Base::Player MatchCreator::GetRandomPlayer()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, mPlayersList.size() - 1);

    int randomIndex = dis(gen);
    auto randomIter = std::next(mPlayersList.begin(), randomIndex);

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

