#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
	// Convert userp back to a string pointer and append new data
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb; // tell curl how many bytes we handled
}
int main() {
	CURL* curl = curl_easy_init();
	CURLcode res;
	std::string response;
	std::string apiUrl{ "https://api.coinbase.com/v2/prices/BTC-USD/spot" };
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, apiUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK) {
			std::cout << "it failed" << std::endl;
		}
		else {
			json jsonData = json::parse(response);
			std::string price = jsonData["data"]["amount"];
			std::cout << "the price is $" << price << " USD";

		}
		curl_easy_cleanup(curl);
	}

