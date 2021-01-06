#ifndef LISNR_PROJECT_ENCODER_H
#define LISNR_PROJECT_ENCODER_H

#include <cstdlib>
#include <string>

static size_t callback_buffer(void *contents, size_t size, size_t nmemb, void *user_data);
std::string fetch_image(char* url);

#endif //LISNR_PROJECT_ENCODER_H
