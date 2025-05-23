#include <downloader/downloader.h>
#include <cpr/cpr.h>
#include <fstream>
#include <iostream>
#include <fmt/core.h>
#include <fmt/color.h>

#include "log/log.h"
// progress bar
void Downloader::download(const std::string &url, const std::string &outputFilePath)
{
#ifdef _WIN32
    std::string name = outputFilePath.substr(outputFilePath.find_last_of("\\") + 1);
#else
    std::string name = outputFilePath.substr(outputFilePath.find_last_of("/") + 1);
#endif
    // Perform the HTTP GET request
    cpr::Response response = cpr::Get(cpr::Url{url},cpr::ReserveSize{1024 * 1024 * 8},cpr::ProgressCallback([&](cpr::cpr_off_t download_total, cpr::cpr_off_t download_now, cpr::cpr_off_t upload_total, cpr::cpr_off_t upload_now, intptr_t user_data) -> bool
                                                                           { Log::log(std::format("Downloading {} : {:.2f}%", name.c_str(),((double)download_now / download_total) * 100.0),Type::E_DISPLAY,"\r");return true; }));

    // Check if the download was successful
    if (response.status_code == 200)
    {
        // Open a file in binary mode to write the ZIP content
        std::ofstream outputFile(outputFilePath, std::ios::binary);

        // Write the response content to the file
        outputFile.write(response.text.c_str(), response.text.size());
        outputFile.close();
        Log::log(std::format("file downloaded and saved as {}", outputFilePath));
    }
    else
    {
        Log::log(std::format("Failed to download file. Status code:{}", response.status_code));
    }
}
