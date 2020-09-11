#include <iostream>
#include <ctime>

#include "./server.h"
#include "./handlers/test.cpp"
#include "./common/resify.h"

using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime()
{
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "(%Y/%m/%d %X)", &tstruct);

    return buf;
}

std::string dump_headers(const Headers &headers)
{
    std::string s;
    char buf[BUFSIZ];

    for (auto it = headers.begin(); it != headers.end(); ++it)
    {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
        s += buf;
    }

    return s;
}

std::string log(const Request &req, const Response &res)
{
    std::string s;
    char buf[BUFSIZ];

    // s += "\n";
    std::string now = currentDateTime();

    snprintf(buf, sizeof(buf), "%s %s:%d - %s %s",
             //  req.version.c_str(), // HTTP/1.1 HTTP/2
             //  std::put_time(&tm, "%c %Z"),
             now.c_str(),
             req.remote_addr.c_str(),
             req.remote_port,
             req.method.c_str(),
             req.path.c_str());
    s += buf;

    std::string query;
    for (auto it = req.params.begin(); it != req.params.end(); ++it)
    {
        const auto &x = *it;
        snprintf(buf, sizeof(buf), "%c%s=%s",
                 (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
                 x.second.c_str());
        query += buf;
    }
    snprintf(buf, sizeof(buf), "%s", query.c_str());
    s += buf;

    // s += dump_headers(req.headers);

    // s += "--------------------------------\n";

    snprintf(buf, sizeof(buf), ", code %d res_data: ", res.status);
    s += buf;
    // s += dump_headers(res.headers);
    // s += "\n";

    if (!res.body.empty())
    {
        s += res.body;
    }

    s += "\n";

    return s;
}

int main()
{

    if (!server.is_valid())
    {
        printf("server has an error...\n");
        return -1;
    }

    const int PORT = 1234;
    const char *SERVER_DOMAIN = "localhost";

    TestHandlers();

    server.set_error_handler([](const Request & req, Response &res) {
        // const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
        // char buf[BUFSIZ];
        // snprintf(buf, sizeof(buf), fmt, res.status);
        // res.set_content(buf, "text/html");
        struct ResponseData *pData;
        if (res.status == 404)
        {
            pData = new struct ResponseData(404, "Not Found");
        }else {
            pData = new struct ResponseData();
        }
        cout << "res.headers:" << dump_headers(res.headers) << endl;
        Resify::error(res, pData);
    });

    server.set_logger([](const Request &req, const Response &res) {
        printf("%s", log(req, res).c_str());
    });

    std::cout << "Server starts on " << SERVER_DOMAIN << ":" << PORT << std::endl;
    server.listen(SERVER_DOMAIN, PORT);
}
