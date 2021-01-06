#include "encoder.h"
#include "curl/curl.h"
#include <cstdlib>
#include <iostream>

/**
 * callback function for use by CURLOTP_WRITEFUNCTION -> overrides fwrite as default callback. Allows us to manipulate
 * the data to just be added to a buffer.
 *
 * https://curl.se/libcurl/c/CURLOPT_WRITEFUNCTION.html
 *
 * @param contents - ptr to the delivered data
 * @param size - according to documentation size is always 1
 * @param nmemb - size of the data
 * @param user_data - ptr to our CURLOPT_WRITEDATA parameter (ie buffer)
 * @return - size of the data transferred
 */
static size_t callback_buffer(void *contents, size_t size, size_t nmemb, void *user_data)
{
	((std::string*)user_data)->append((char*)contents, size * nmemb);
	return size * nmemb;
}

/**
 * Allows us to fetch the url given and uses cURL to retrieve the image. Stores result in a string buffer then returns
 * it.
 *
 * @param url - URL provided from argv
 * @return buffer - loaded buffer of data from the image, "" - empty string if cURL error happened
 */
std::string fetch_image(char* url)
{

	std::string buffer;

	CURL* curl = curl_easy_init();

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback_buffer);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);

	CURLcode rc = curl_easy_perform(curl); //executes the transfer as described by the options

	if (rc)//CURL_OK is 0, nonzero is an error
	{
		printf("Failed to download: %s\n cURL Error: %d\n", url, rc);

		long res_code = 0; //check to see response code given

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
		if (!(res_code == 200 || res_code == 201)) {
			printf("Response code: %ld\n", res_code);
			return "";
		}
	}

	curl_easy_cleanup(curl);

	return buffer;
}

