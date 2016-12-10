/*
 * Decision Tree
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */

#include "./decisionTree.h"
#include <stdlib.h>
#include <time.h>

int main() 
{
  std::string trainFileName;
  std::cout << "Enter the training file name: ";
  std::cin >> trainFileName;

  if(!Utils::getUtils().populateAttrInfo(trainFileName)) {
    return 1;
  }
  
  std::string skipRandomSplit("Y");
  std::cout << "Do you want to skip the random-split (80/20) based validation [Y/N]: ";
  std::cin >> skipRandomSplit;
  if(skipRandomSplit.compare("N") == 0) {
    srand(time(NULL));

    double wall0 = Utils::getUtils().get_wall_time();
    double cpu0  = Utils::getUtils().get_cpu_time();

    Utils::getUtils().performRandomSplitBasedValidation();

    double wall1 = Utils::getUtils().get_wall_time();
    double cpu1  = Utils::getUtils().get_cpu_time();

    std::cout << "\nTime(s) : Random-Split Based Validation : Wall Time : " << wall1 - wall0 << ", CPU Time : " << cpu1  - cpu0 << "\n\n";

  } else {
    std::cout << "Skipped random-split based validation ..." << std::endl;
  }
                                                    
  std::string skip10FoldXValidation("N");
  std::cout << "Do you want to skip the 10-fold corss-validation [Y/N]: ";
  std::cin >> skip10FoldXValidation;
  if(skip10FoldXValidation.compare("N") == 0) {

    double wall0 = Utils::getUtils().get_wall_time();
    double cpu0  = Utils::getUtils().get_cpu_time();

    Utils::getUtils().perform10FoldXValidation();

    double wall1 = Utils::getUtils().get_wall_time();
    double cpu1  = Utils::getUtils().get_cpu_time();

    std::cout << "\nTime(s) : 10-fold Cross-Validation : Wall Time : " << wall1 - wall0 << ", CPU Time : " << cpu1  - cpu0 << "\n\n";

  } else {
    std::cout << "Skipped 10-fold corss-validation ..." << std::endl;
  }

  std::cout << "That's all folks!" << std::endl;
  return 0;
}
