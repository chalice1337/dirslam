#include <iostream>
#include <curl/curl.h>
#include <fstream>

size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

int main(int argc, char* argv[]) {
    std::cout << "\nmade by chalice1337\n\n";

    if (argc != 5 || std::string(argv[1]) != "-l" || std::string(argv[3]) != "-w") {
        std::cerr << "ERROR!, Invalid Format. Usage /.dirslam -l https://website.com/ -w wordlist.txt\n";
        return 1;
    }
    
    std::string targeturl = argv[2];
    std::string wordlistpath = argv[4];

    std::ifstream wordlistFile(wordlistpath);
    if (!wordlistFile.is_open()) {
        std::cerr << "Error: Could not find " << wordlistpath << "\n";
        return 1;
    }
   CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing libcurl\n";
        return 1;
    }

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

    std::string directory;
    while (std::getline(wordlistFile, directory)) {
        std::string url = targeturl + directory;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        
        std::string response;
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "Error: " << curl_easy_strerror(res) << std::endl;
        } else {
            long http_code;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 200) {
                std::cout << "Found: /" << url << " (Status 200 | Valid)\n";
            }
        }
    }

    curl_easy_cleanup(curl);
    wordlistFile.close();
    return 0;
}

