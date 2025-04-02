#include <iostream>

#include "include/restbed"

class Shortner
{
public:
    Shortner();
    std::string Shorten(const std::string& string_);
    std::string Longen(const std::string& string_);

private:
    static std::string ToString(std::uint32_t id_);
    static uint32_t FromString(std::string string_);
    static constexpr size_t CHARACTERS_COUNT = 62;
    static const char CHARACTERS[CHARACTERS_COUNT];
    static uint32_t m_idvalue[255];
    std::mutex m_mutex;
    std::unordered_map<std::uint32_t, std::string> m_map;
    std::uint32_t m_id;
};

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

void PostMethodHandler(const std::shared_ptr<restbed::Session>& session_,
                       Shortner& shortner_)
{
    const auto request = session_->get_request();
    const size_t contentLength = request->get_header("Content-Length", 0);
    session_->fetch(contentLength,
                    [&shortner_](
                    const std::shared_ptr<restbed::Session>& current_session_,
                    const restbed::Bytes body)
                    {
                        const std::string result = shortner_.Shorten(
                            reinterpret_cast<const char*>(body.data()));
                        current_session_->close(restbed::OK, result, {
                                                    {
                                                        "Content-Length",
                                                        std::to_string(
                                                            result.size())
                                                    },
                                                    {"Connection", "close"}
                                                });
                    });
}

void GetMetodHandler(const std::shared_ptr<restbed::Session>& session_,
                     Shortner& shortner_)
{
    const auto request = session_->get_request();
    std::string result = "Error url requested not found";
    try
    {
        result = shortner_.Longen(request->get_path([](auto string_)
        {
            return string_.substr(1);
        }));
    }
    catch(...)
    {
    }
    session_->close(restbed::NOT_FOUND, result, {
                        {"Content-Length", std::to_string(result.size())},
                        {"Connection", "close"}
                    });
}

int main()
{
    Shortner shortner;
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(8080);
    settings->set_bind_address("0.0.0.0");
    auto get_resource = std::make_shared<restbed::Resource>();
    auto post_resource = std::make_shared<restbed::Resource>();
    get_resource->set_path("/[a-zA-Z0-9]+");
    get_resource->set_method_handler(
        "GET", [&shortner](const std::shared_ptr<restbed::Session>& session_)
        {
            GetMetodHandler(session_, shortner);
        });
    post_resource->set_path("/shorten/post");
    post_resource->set_method_handler(
        "POST", [&shortner](const std::shared_ptr<restbed::Session>& session_)
        {
            PostMethodHandler(session_, shortner);
        });
    restbed::Service service;
    service.publish(get_resource);
    service.publish(post_resource);
    service.start(settings);
    return 0;
}
