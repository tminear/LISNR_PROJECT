/**
 * Author: Tim Minear (tminear13@gmail.com)
 *
 * Task:

 * Develop a C/C++ library and executable that performs the following actions:

 * Retrieves an image from a specified URL.
 * Base64 (RFC 4648) encodes this image.
 * Returns the Base64 encoded version of the image.
 * Writes the Base64 data to a specified file.

 * Notes:

    The library shall provide an API that takes the URL to the desired image as in input parameter and provides the
    Base64 encoded version of the image as an output parameter. The executable shall take as command-line parameters the
    URL for the image to be retrieved and a file to which the Base64 encoded data shall be written.  It should use the
    previously described library to perform the image retrieval and encoding. It will then write the Base64 data to the
    specified file. Exhaustive error handling is not required, but the application should behave in a predictable and
    describable fashion. Feel free to use any 3rd party libraries with the exception of the Base64 encoding, provide an
    implementation of the Base64 encoder.

    Specify any dependencies or other requirements introduced through the use of any 3rd party libraries.
 */

#include <cstdlib>
#include <iostream>

#include "curl/curl.h"
#include "encoder.h"

/**
 * Main executable for LISNR_PROJECT library, will be used to test functions from library
 *
 * @param argc - Number of command line arguments
 * @param argv - Command Line Arguments
 * @return 0 for EXIT_SUCCESS, -1 for Failure
 */
int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Usage: %s <url>\n", argv[0]);
		return -1;
	}

	std::string curl_data = fetch_image(argv[1]);

	if (curl_data.empty())
	{
		printf("Failed to download file: %s\n", argv[1]);
		return -1;
	}

	return 0;
}
