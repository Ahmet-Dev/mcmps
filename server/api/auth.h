#ifndef SERVER_AUTH_H
#define SERVER_AUTH_H

#include <string>

bool clientAuthenticate(const std::string &jwtToken);

#endif // SERVER_AUTH_H
