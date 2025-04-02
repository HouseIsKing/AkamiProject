// Filename: Shortner.hpp
// Created by shiningblack on 4/2/25.
// Reviewer:
// Review Status:

#ifndef ILRD_RD1645_SHORTNER_HPP
#define ILRD_RD1645_SHORTNER_HPP

#include "restbed"

class Shortner
{
public:
    static Shortner& GetInstance();
    std::string Shorten(const std::string& string_);
    std::string Longen(const std::string& string_);

private:
    Shortner();
    static std::string ToString(std::uint32_t id_);
    static uint32_t FromString(std::string string_);
    static constexpr size_t CHARACTERS_COUNT = 62;
    static const char CHARACTERS[CHARACTERS_COUNT];
    static uint32_t m_idvalue[255];
    std::mutex m_mutex;
    std::unordered_map<std::uint32_t, std::string> m_map;
    std::uint32_t m_id;
};


#endif //ILRD_RD1645_SHORTNER_HPP
