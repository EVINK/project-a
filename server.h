#ifndef _SERVER_H
#define _SERVER_H _SERVER_H

#include "./libs/httplib.h"
using namespace httplib;

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
SSLServer sevver(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
Server server;
#endif

#endif