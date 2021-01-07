#include "encoder.h"
#include "curl/curl.h"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <fstream>


static const char base64_set[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

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
std::vector<unsigned char> fetch_image(char* url)
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
		printf("Failed to download: %s\ncURL Error: %d\n", url, rc);

		long res_code = 0; //check to see response code given

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &res_code);
		if (!(res_code == 200 || res_code == 201)) {
			printf("Response code: %ld\n", res_code);
			return std::vector<unsigned char>();
		}
	}

	curl_easy_cleanup(curl);

	std::vector<unsigned char> data(buffer.begin(), buffer.end());

	return data;
}

/**
 * Encodes the vector of unsigned characters into Base64 and writes the output to the argv file
 * @param data - vector of unsigned characters obtained from fetching an image
 * @param fileName - argv[2] the name of the desired output file
 */
void encode(std::vector<unsigned char> data, std::string fileName){

	//dealing with 24-bits at a time so this covers if the vector is not a multiple of 3
	if((data.size() % 3) != 0){
		for(size_t i = 0; i < data.size() % 3; ++i){
			data.push_back('\0');
		}
	}

	//open file to write to
	std::ofstream outFile;
	outFile.open(fileName);

	//24 bits is the smallest chunk to break the
	for(size_t i = 0; i < data.size(); i+=3){

		//Base64 inserts newlines every 76 characters,
		if(i % 19 == 0 && i != 0){
			outFile << '\n';
		}

		//grab three bytes at a time to break into 4 6-bit chunks
		std::string bytes = std::bitset<8>(data[i]).to_string() + std::bitset<8>(data[i+1]).to_string()
				+ std::bitset<8>(data[i+2]).to_string();

		outFile << base64_set[std::stoi(bytes.substr(0,6), nullptr, 2)];
		outFile << base64_set[std::stoi(bytes.substr(6,6), nullptr, 2)];
		outFile << base64_set[std::stoi(bytes.substr(12,6), nullptr, 2)];
		outFile << base64_set[std::stoi(bytes.substr(18,6), nullptr, 2)];

	}

	outFile.close();

}

