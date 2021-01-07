#ifndef LISNR_PROJECT_ENCODER_H
#define LISNR_PROJECT_ENCODER_H

#include <cstdlib>
#include <string>
#include <vector>

static size_t callback_buffer(void *contents, size_t size, size_t nmemb, void *user_data);
std::vector<unsigned char> fetch_image(char* url);
void encode(std::vector<unsigned char> data, std::string fileName);

#endif //LISNR_PROJECT_ENCODER_H
