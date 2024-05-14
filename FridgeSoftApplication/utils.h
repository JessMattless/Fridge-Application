#ifndef UTILS_H
#define UTILS_H
#include <iostream>
#include <ctime>
#include <string>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <random>

std::time_t getCurrentUnixTimestamp();

std::string unixTimestampToString(std::time_t timestamp);

std::string generateHashedPasswordWithSalt(const std::string& password);
bool isPasswordCorrect(const std::string& enteredPassword, const std::string& storedHash);
std::string hashPassword(const std::string& password, const std::string& salt);

#endif // UTILS_H
