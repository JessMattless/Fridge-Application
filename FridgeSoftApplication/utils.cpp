#include "utils.h"

// Gets Current Timestamp
std::time_t getCurrentUnixTimestamp() {
    return std::time(nullptr);
}

// Makes Timestamp Readable, https://stackoverflow.com/questions/60230149/how-to-properly-convert-string-to-stdchronosystem-clocktime-point
std::string unixTimestampToString(std::time_t timestamp) {
    std::tm tm;
    gmtime_r(&timestamp, &tm);

    char buffer[32];
    std::strftime(buffer, 32, "%Y-%m-%d %H:%M:%S", &tm);
    return std::string(buffer);
}


// Generates Random Salt for password, https://inversepalindrome.com/blog/how-to-create-a-random-string-in-cpp
std::string generateSalt(size_t length) {
    std::string chars =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, chars.size() - 1);

    std::string salt;
    for (size_t i = 0; i < length; ++i) {
        salt += chars[distribution(generator)];
    }
    return salt;
}

  // Encrypts password and adds Salt
std::string hashPassword(const std::string& password, const std::string& salt) {
    EVP_MD_CTX* context = EVP_MD_CTX_new();
    if (context == nullptr) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to initialize digest context");
    }
    // Adds salt to password
    std::string saltedPassword = password + salt;
    if (EVP_DigestUpdate(context, saltedPassword.c_str(), saltedPassword.size()) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to update digest");
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    if (EVP_DigestFinal_ex(context, hash, &lengthOfHash) != 1) {
        EVP_MD_CTX_free(context);
        throw std::runtime_error("Failed to finalize digest");
    }

    EVP_MD_CTX_free(context);
    // Formats into human-readable string
    std::stringstream ss;
    for (unsigned int i = 0; i < lengthOfHash; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Creates encrypted password with salt
std::string generateHashedPasswordWithSalt(const std::string& password) {
    std::string salt = generateSalt(16);
    return salt + ":" + hashPassword(password, salt);
}

// Checks to see if password is correct
bool isPasswordCorrect(const std::string& enteredPassword, const std::string& storedHash) {
    auto delimiterPos = storedHash.find(":");
    std::string salt = storedHash.substr(0, delimiterPos);
    std::string originalHash = storedHash.substr(delimiterPos + 1);
    std::string hashed = hashPassword(enteredPassword, salt);
    return hashed == originalHash;
}
