#ifndef _ResponsData
#define _ResponsData _ResponsData

#include "../server.h"
#include "../libs/json.hpp"

using json = nlohmann::json;
using namespace std;

struct ResponseData
{
    int code;
    // bool success;
    std::string msg;
    nlohmann::json data;

    ResponseData()
    {
        // pass
    }

    ResponseData(int c, std::string m)
    {
        code = c;
        msg = m;
    }

    ResponseData(int c, std::string m, json jData)
    {
        code = c;
        msg = m;
        data = jData;
    }

    ResponseData(json jData)
    {
        data = jData;
    }

    // void Destory();
    void Destory()
    {
        // std::cout << "ready to destory heap reference" << std::endl;
        delete this; // free heap memory
    }
};

class Resify
{
public:
    // static void success(Response &res, ResponseData *data);
    static void success(Response &res, ResponseData *pData)
    {
        json resData = {
            {"code", 200},
            {"success", true},
            {"msg", "success"},
            {"data", NULL},
        };

        resify(res, pData, resData);
    }

    static void error(Response &res, ResponseData *pData)
    {

        json resData = {
            {"code", 500},
            {"success", false},
            {"msg", "Internel Error"},
            {"data", NULL},
        };

        resify(res, pData, resData);
    }

private:
    // static void resify(Response &res, struct ResponseData *pData, nlohmann::json resData);
    static void resify(Response &res, struct ResponseData *pData, nlohmann::json resData)

    {
        json _data;

        if (pData->code)
        {
            resData["code"] = pData->code;
        }

        if (pData->msg.empty() == false)
        {
            resData["msg"] = pData->msg;
        }

        if (pData->data != NULL)
        {
            for (json::iterator it = pData->data.begin(); it != pData->data.end(); ++it)
            {
                _data[it.key()] = it.value();
            }
            resData["data"] = _data;
        }

        res.set_content(resData.dump(), "application/json");
        pData->Destory();
    }
};

#endif
