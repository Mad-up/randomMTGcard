#include <iostream>
#include <string>
//#include <json/json.h>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h" // Include cpp-httplib header

using namespace std;

int main() {
    try {
        cout<<"Set up a client that follows redirects\n";
        httplib::Client cli("scryfall.com");
        cli.enable_server_certificate_verification(false);
        cli.set_follow_location(true);
        cli.set_default_headers({{"User-Agent", "Mozilla/5.0"}});

        cout<<"Make the request to scryfall.com/random (with redirect handling)\n";
        auto res = cli.Get("/random");
        cout<<"status - "<< res->status<<endl;
        //cout<<"body - "<< res->body<<endl;
        if (res->status == 200) {
            cout<<" status 200: Extract the HTML content\n";
            string html_content = res->body;

            // Search for the image URL in the HTML
            size_t start_pos = html_content.find("<div class=\"card-profile\">");
            if (start_pos != string::npos) {
                start_pos += strlen("<div class=\"card-profile\">");
                size_t end_pos = start_pos + 500;

                if (end_pos != string::npos) {
                    string img_url = html_content.substr(start_pos, end_pos - start_pos);
                    start_pos = img_url.find("src=");
                    end_pos = img_url.find("/>");
                    img_url = img_url.substr(start_pos +4, end_pos - start_pos - 4);
                    // Download the image using curl
                    string cmd = "curl -o card.jpg " + img_url;
                    //cout<<"////////start pos ="<< start_pos<< "end pos = " <<end_pos<<endl;
                    cout<<"string\n\n"<<img_url<<endl;
                    system(cmd.c_str());

                    cout << "Successfully downloaded the random card image as card.jpg" << endl;
                }
            } else {
                cerr << "Could not find the image URL in the HTML." << endl;
                return 1;
            }
        } else {
            cerr << "HTTP Error: Status code " << res->status << endl;
        }
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}

