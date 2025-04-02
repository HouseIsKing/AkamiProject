// Filename: Shortner.cpp
// Created by shiningblack on 4/2/25.
// Reviewer:
// Review Status: 
//

#include "Shortner.hpp"

Shortner::Shortner() : m_id(1)
{
    for (int i = 'a'; i <= 'z'; ++i)
    {
        m_idvalue[i] = i - 'a';
    }
    for (int j = 'A'; j <= 'Z'; ++j)
    {
        m_idvalue[j] = j - 'A' + 26;
    }
    for (int i = '0'; i <= '9'; ++i)
    {
        m_idvalue[i] = i - '0' + 52;
    }
}

const char Shortner::CHARACTERS[] = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
    'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D',
    'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S',
    'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9'
};

uint32_t Shortner::m_idvalue[] = {};

std::string Shortner::Shorten(const std::string& string_)
{
    uint32_t id;
    {
        std::unique_lock lock(m_mutex);
        id = m_map.emplace(m_id++, string_).first->first;
    }

    return ToString(id);
}

std::string Shortner::Longen(const std::string& string_)
{
    const uint32_t id = FromString(string_);
    std::unique_lock lock(m_mutex);
    return m_map.at(id);
}

std::string Shortner::ToString(std::uint32_t id_)
{
    std::string result;
    while(id_ != 0)
    {
        (void)result.append(1, CHARACTERS[id_ % CHARACTERS_COUNT]);
        id_ /= CHARACTERS_COUNT;
    }

    return result;
}

uint32_t Shortner::FromString(std::string string_)
{
    uint32_t result = 0;
    for (const char c : string_)
    {
        result *= CHARACTERS_COUNT;
        result += m_idvalue[c];
    }
    return result;
}

Shortner& Shortner::GetInstance()
{
    static Shortner instance;
    return instance;
}