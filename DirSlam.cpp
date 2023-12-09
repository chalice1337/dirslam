#include <iostream>
#include <curl/curl.h>
#include <fstream>

#define LIGHT_BLUE_TEXT "\033[1;34m"
#define GREEN_TEXT "\033[1;32m"
#define RESET_COLOR "\033[0m"

std::string lightBlueColor() {
    return "\033[38;5;39m";
}

std::string greenColor() {
    return "\033[38;5;40m"; 
}

std::string resetColor() {
    return "\033[0m";  
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data) {
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

void printHeader() {
    std::cout <<
    "|============================================================|\n"	    
    "|  ▓█████▄  ██▓ ██▀███    ██████  ██▓    ▄▄▄       ███▄ ▄███▓|\n"
    "|  ██▀ ██▌▓██▒▓██ ▒ ██▒▒██    ▒ ▓██▒   ▒████▄    ▓██▒▀█▀ ██▒ |\n"
    "| ░██   █▌▒██▒▓██ ░▄█ ▒░ ▓██▄   ▒██░   ▒██  ▀█▄  ▓██    ▓██░ |\n"
    "| ░▓█▄   ▌░██░▒██▀▀█▄    ▒   ██▒▒██░   ░██▄▄▄▄██ ▒██    ▒██  |\n"
    "| ░▒████▓ ░██░░██▓ ▒██▒▒██████▒▒░██████▒▓█   ▓██▒▒██▒   ░██▒ |\n"
    "|   ▒▒▓  ▒ ░▓  ░ ▒▓ ░▒▓░▒ ▒▓▒ ▒ ░░ ▒░▓  ░▒▒   ▓▒█░░ ▒░   ░  ░|\n"
    "|  ░ ▒  ▒  ▒ ░  ░▒ ░ ▒░░ ░▒  ░ ░░ ░ ▒  ░ ▒   ▒▒ ░░  ░      ░ |\n"
    "|  ░ ░  ░  ▒ ░  ░░   ░ ░  ░  ░    ░ ░    ░   ▒   ░      ░    |\n"
    "|    ░     ░     ░           ░      ░  ░     ░  ░       ░    |\n"
    "|							     |\n"
    "|\t\t\t\t\t made by chalice1337 |\n"
    "|============================================================|\n\n";

}

int main(int argc, char* argv[]) {
    printHeader();

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

    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 0L);
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
                std::cout << "Found: " << url << lightBlueColor() << " (Status 200 | Valid)\n" << resetColor();
            } else if (http_code == 301) {
                std::cout << "Found: " << url << greenColor() << " (Status 301)\n" << resetColor();
            }
        }
    }

    curl_easy_cleanup(curl);
    wordlistFile.close();
    return 0;
}
