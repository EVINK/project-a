#include "../server.h"
#include "../libs/json.hpp"
#include "../common/resify.h"

using json = nlohmann::json;

void TestHandlers()
{
    server.Get("/ping", [](const Request &req, Response &res) {
        res.set_content("Pong!", "text/plain");
    });

    server.Post("/success", [](const Request &req, Response &res) {
        json data = {
            {"key1", "value1"},
        };
        struct ResponseData *pData = new struct ResponseData(201, "OK", data);
        Resify::success(res, pData);
    });

    server.Post("/error", [](const Request &req, Response &res) {
        json data = {
            {"key1", "value1"},
        };
        struct ResponseData *pData = new struct ResponseData(data);
        Resify::error(res, pData);
    });

    server.Post("/throw_error", [](const Request &req, Response &res) {
        throw std::invalid_argument("received negative value");
    });
}
