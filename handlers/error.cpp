#include "../server.h"

void ErrorHandlers()
{
    server.Get("/ping", [](const Request &req, Response &res) {
        res.set_content("Pong!", "text/plain");
    });
}