//
//  namesAPI.cc - a microservice demo program
//
// James Skon
// Kenyon College, 2022
//

#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include "NameMap.h"
#include "NameEntry.h"
#include "httplib.h"

using namespace httplib;
using namespace std;

const int port = 5002;
const string path = "namedata/";
const string lastFile = path+"dist.all.last";
const string maleFile = path+"dist.male.first";
const string femaleFile = path+"dist.female.first";

// Build the name maps                                                                                           
NameMap lastMap(lastFile);
NameMap femaleMap(femaleFile);
NameMap maleMap(maleFile);

std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

int main(void) {
  Server svr;
  // Check if files opens and maps were built
  if (!lastMap.NameMapSuccess() || !femaleMap.NameMapSuccess()|| !maleMap.NameMapSuccess()) {
    cout << "File open failure!" << endl;
    return 0;
  }
  cout << "Name files loaded..." << endl;

  
  svr.Get("/", [](const Request & /*req*/, Response &res) {
    res.set_header("Access-Control-Allow-Origin","*");
    res.set_content("name/name/type (type=Male,Female,Last)", "text/plain");
  });

  svr.Get(R"(/name/(.*)/(.*))", [&](const Request& req, Response& res) {
    res.set_header("Access-Control-Allow-Origin","*");
    string name = req.matches[1];
    string type = req.matches[2];
    string result = "";
    
    vector<NameEntry> matchList; // place to store the matches

    /* Convert name to upper case*/
    transform(name.begin(), name.end(), name.begin(), ::toupper);

    /* Convert type to lower case*/
    transform(type.begin(), type.end(), type.begin(), ::tolower);

    // Lookup the name in the appropriate name map                                                                   
    if (type.compare("last")==0) {
      matchList = lastMap.getMatches(name);
    } else if (type.compare("female")==0){
      matchList = femaleMap.getMatches(name);
    } else if (type.compare("male")==0){
      matchList = maleMap.getMatches(name);
    } else {   
      result = "Invalid type: "+ type;
      cout << result << endl;
      return;
    }
    /* Results are in JSON for up to 10 names */
    result =  "{\"results\":[";
    for (long unsigned int i = 0; i < matchList.size(); i++) {
      result += "{\"name\":\"" + matchList.at(i).name + "\",";
      result += "\"percent\":" + matchList.at(i).percent + ",";
      result += "\"rank\":" + matchList.at(i).rank + "}";
      if (i<matchList.size()-1)
	result +=  ","; // Only put out this comma if not last entry.                                                   
    }
    result +=  "]}";
    cout << result << endl;
    res.set_content(result, "text/plain");
    res.status = 200;
  });
  
  cout << "Server listening on port " << port << endl;
  svr.listen("0.0.0.0", port);

}
