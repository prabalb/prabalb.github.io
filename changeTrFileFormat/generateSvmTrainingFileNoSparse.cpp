#include <iostream>
#include <fstream>

#include <map>
#include <vector>
#include <string>
#include <cstring>

#include <cassert>

using namespace std;

int main() {
  std::map<int, std::vector<string> > features;

  // school
  features[1].push_back("G");
  features[1].push_back("M");

  // sex
  features[2].push_back("F");
  features[2].push_back("M");

  // age --- no entry
  
  // address
  features[4].push_back("U");
  features[4].push_back("R");

  // famsize
  features[5].push_back("L");
  features[5].push_back("G");

  // pstatus
  features[6].push_back("T");
  features[6].push_back("A");

  // medu --- no entry

  // fedu --- no entry

  // mjob
  features[9].push_back("T");
  features[9].push_back("H");
  features[9].push_back("C");
  features[9].push_back("A");
  features[9].push_back("O");

  // fjob
  features[10].push_back("T");
  features[10].push_back("H");
  features[10].push_back("C");
  features[10].push_back("A");
  features[10].push_back("O");

  // reason
  features[11].push_back("H");
  features[11].push_back("R");
  features[11].push_back("C");
  features[11].push_back("O");

  // guardian
  features[12].push_back("M");
  features[12].push_back("F");
  features[12].push_back("O");

  // traveltime --- no entry
  
  // studytime --- no entry
  
  // failures --- no entry

  // schoolsup
  features[16].push_back("Y");
  features[16].push_back("N");

  // famsup
  features[17].push_back("Y");
  features[17].push_back("N");

  // paid
  features[18].push_back("Y");
  features[18].push_back("N");

  // activities
  features[19].push_back("Y");
  features[19].push_back("N");

  // nursery
  features[20].push_back("Y");
  features[20].push_back("N");

  // higher
  features[21].push_back("Y");
  features[21].push_back("N");

  // internet
  features[22].push_back("Y");
  features[22].push_back("N");

  // romantic
  features[23].push_back("Y");
  features[23].push_back("N");

  // famrel --- no entry

  // freetime --- no entry
  
  // goout --- no entry

  // health --- no entry

  std::ofstream outfile;
  outfile.open("mat_svm_train_mat_3.txt");

  std::ifstream infile;
  infile.open("student_svm.txt");

  assert(infile.is_open());

  while (!infile.eof()) {
    int counter = 1;
    std::string ss("");
    char buf[100];
    infile.getline(buf, 100);

    const char* token[8] = {};
    token[0] = strtok(buf, ",");
    if(!token[0]) continue;

    std::string entry(token[0]);
    token[1] = strtok(0, ",");
    std::string output(token[1]);
    ss += output == "1" ? "+1 " : "-1 ";
    //std::cout << entry << std::endl;
    for(size_t i = 0, j = i + 1; i != entry.length(); i++, j++) {
      std::string value(1, entry[i]);
      //std::cout << "value: " << value << std::endl;
      //std::cout << "j: " << j << std::endl;
      if(features.find(j) != features.end()) {
        //std::cout << "got categorical feature ...\n";
        std::vector<string> vals = features[j];
        for(size_t k = 0; k < vals.size(); k++, counter++) {
          if(vals[k] != value) {
            //std::cout << "vals[k] = " << vals[k] << ", value = " << value << std::endl;
            ss += std::to_string(counter);
            ss += ":0 ";
            continue;
          }
          //std::cout << "counter: " << counter << std::endl;
          ss += std::to_string(counter);
          ss += ":1 ";
          //std::cout << ss << std::endl;
        }
        //std::cout << "counter: " << counter << std::endl;
      } else {
        ss += std::to_string(counter);
        ss += ":";
        ss += value;
        ss += " ";
        counter++;
      }
    }
    outfile << ss << "\n"; 
  }

  infile.close();
  outfile.close();

  return 0;

}
