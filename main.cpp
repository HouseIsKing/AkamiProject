#include "Shortner.hpp"

void PostMethodHandler(const std::shared_ptr<restbed::Session>& session_,
                       Shortner& shortner_)
{
    const auto request = session_->get_request();
    const size_t contentLength = request->get_header("Content-Length", 0);
    session_->fetch(contentLength,
                    [&shortner_](
                    const std::shared_ptr<restbed::Session>& current_session_,
                    const restbed::Bytes& body_)
                    {
                        const std::string result = shortner_.Shorten(
                            reinterpret_cast<const char*>(body_.data()));
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
    Shortner& shortner = Shortner::GetInstance();
    const auto settings = std::make_shared<restbed::Settings>();
    settings->set_port(8080);
    const auto getResource = std::make_shared<restbed::Resource>();
    const auto postResource = std::make_shared<restbed::Resource>();
    getResource->set_path("/[a-zA-Z0-9]+");
    getResource->set_method_handler(
        "GET", [&shortner](const std::shared_ptr<restbed::Session>& session_)
        {
            GetMetodHandler(session_, shortner);
        });
    postResource->set_path("/shorten/post");
    postResource->set_method_handler(
        "POST", [&shortner](const std::shared_ptr<restbed::Session>& session_)
        {
            PostMethodHandler(session_, shortner);
        });
    restbed::Service service;
    service.publish(getResource);
    service.publish(postResource);
    service.start(settings);
    return 0;
}
