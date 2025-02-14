#include <string>

std::string caesar_encrypt(const std::string &message, int shift);
std::string vigenere_encrypt(const std::string &message, const std::string &key); 
std::string vernam_encrypt(const std::string &message, const std::string &key); 
std::string playfair_encrypt(const std::string &message, const std::string &key); 

std::string caesar_decrypt(const std::string &message, int shift);
std::string vigenere_decrypt(const std::string &message, const std::string &key); 
std::string vernam_decrypt(const std::string &message, const std::string &key); 
std::string playfair_decrypt(const std::string &message, const std::string &key); 

std::string encrypt(const std::string &message, int choice); 
std::string decrypt(const std::string &message, int choice);

