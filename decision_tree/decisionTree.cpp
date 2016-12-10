/*
 * Decision Tree
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */

#include "./decisionTree.h"

#include <sys/time.h>

#include <fstream>
#include <cstring>
#include <cmath>
#include <cassert>
#include <stdlib.h>

bool Utils::populateAttrInfo(std::string trainFileName) {
  std::ifstream file;
  file.open(trainFileName.c_str());

  if(!file.is_open()) {
    std::cout << "Could not open the training file specified, please try again ..." << std::endl;
    return false;
  }

  while (!file.eof()) {
    char buf[100];
    file.getline(buf, 100);

    const char* token[8] = {};
    token[0] = strtok(buf, " ");
    if(!token[0]) continue;
    std::string attrName(token[0]);

    if(attrName.find(",") == string::npos) {
      std::vector<string> possVals;
      int val_counter = 1;
      token[val_counter] = strtok(0, " ");
      const char* ch = token[val_counter];
      while(ch)
      {
        possVals.push_back(std::string(ch));
        val_counter++;
        token[val_counter] = strtok(0, " ");
        ch = token[val_counter];
      }
      attributeInfo[attrName] = possVals;
      attrNames.push_back(attrName);

    } else {
      const char* tok[8] = {};
      tok[0] = strtok(buf, ",");
      if(!tok[0]) continue;
      std::string entry(tok[0]);
      std::vector<Attribute*> attrVec;
      for(size_t i = 0; i != entry.length(); i++) {
        std::string name = attrNames[i]; 
        std::string value(1, entry[i]);
        Attribute *attr = new Attribute(name, value);
        attrVec.push_back(attr);
      }
      tok[1] = strtok(0, " ");
      allExamples.push_back(std::make_pair(attrVec, std::string(tok[1])));
    }
  }
  file.close();
  return true;
}

void Utils::randomSplitDataSet() {
  assert(allTrainingExamples.size() == 0 && testSet.size() == 0);
  int total_data_set_size = allExamples.size();
  int startPos = 0, endPos = 0;
  while(1) {
    startPos = rand() % total_data_set_size;
    endPos = startPos + int(total_data_set_size)/5 - 1;
    if(endPos <= total_data_set_size - 1) break;
  }

  for(int pos = startPos; pos <= endPos; pos++) {
    std::vector<Attribute*> attrVec = allExamples[pos].first;
    std::string testExample;
    for(size_t i = 0; i < attrVec.size(); i++) {
      testExample += attrVec[i]->getAttrVal();
    }
    testSet.push_back(std::make_pair(testExample, allExamples[pos].second));
  }

  allTrainingExamples = allExamples;
  for(int pos = startPos; pos <= endPos; pos++) {
    allTrainingExamples.erase(allTrainingExamples.begin() + pos);
  }
}

void Utils::crossSplitDataSet(int counter) {
  assert(allTrainingExamples.size() == 0 && testSet.size() == 0);
  int total_data_set_size = allExamples.size();
  int startPos = (counter - 1) * (total_data_set_size / 10);
  int endPos = (counter == 10) ? (startPos + floor(float(total_data_set_size) / 10) - 1 + (total_data_set_size % 10))
                               : (startPos + floor(float(total_data_set_size) / 10) - 1);

  for(int pos = startPos; pos <= endPos; pos++) {
    std::vector<Attribute*> attrVec = allExamples[pos].first;
    std::string testExample;
    for(size_t i = 0; i < attrVec.size(); i++) {
      testExample += attrVec[i]->getAttrVal();
    }
    testSet.push_back(std::make_pair(testExample, allExamples[pos].second));
  }

  allTrainingExamples = allExamples;
  for(int pos = startPos; pos <= endPos; pos++) {
    allTrainingExamples.erase(allTrainingExamples.begin() + pos);
  }
}

void Utils::resetDB() {
  allTrainingExamples.clear();
  testSet.clear();
  DecisionTree::getDecisionTree().clearDT();
}

void Utils::performRandomSplitBasedValidation() {
  std::cout << "Performing random-split based validation ..." << std::endl;
  const int num_random_split = 10;
  std::cout << "Following result is for " << num_random_split << " random splits ..." << std::endl;
  std::cout << "Test Run   |   Accuracy on Test Set (%)" << std::endl;
  double avg_accuracy = 0.0;
  int counter = 1;
  while(counter <= num_random_split) {
    resetDB();
    randomSplitDataSet();
    DecisionTree::getDecisionTree().trainDecisionTree(Utils::getUtils().getAllTrainingExamples(), 
                                                      Utils::getUtils().getAllAttrNames(), 
                                                      Utils::getUtils().getAllTrainingExamples());
    //DecisionTree::getDecisionTree().printDecisionTree();

    double accuracy = reportAccuracy(counter);
    avg_accuracy += accuracy;
    std::string format = (counter == 10) ? "         |   " : "          |   ";
    std::cout << counter << format << accuracy << std::endl;
    counter++;
  }
  avg_accuracy /= num_random_split;

  std::cout << "\nAverage accuracy(%): " << avg_accuracy << std::endl;
}

void Utils::perform10FoldXValidation() {
  std::cout << "Performing 10-fold cross-validation ..." << std::endl;
  const int num_split = 10;
  std::cout << "Following result is for " << num_split << " splits ..." << std::endl;
  std::cout << "Test Run   |   Accuracy on Test Set (%)" << std::endl;
  double avg_accuracy = 0.0;
  int counter = 1;
  while(counter <= num_split) {
    resetDB();
    crossSplitDataSet(counter);
    DecisionTree::getDecisionTree().trainDecisionTree(Utils::getUtils().getAllTrainingExamples(), 
                                                      Utils::getUtils().getAllAttrNames(), 
                                                      Utils::getUtils().getAllTrainingExamples());
    //DecisionTree::getDecisionTree().printDecisionTree();

    double accuracy = reportAccuracy(counter);
    avg_accuracy += accuracy;
    std::string format = (counter == 10) ? "         |   " : "          |   ";
    std::cout << counter << format << accuracy << std::endl;
    counter++;

  }
  avg_accuracy /= num_split;

  std::cout << "\nAverage accuracy(%): " << avg_accuracy << std::endl;
}

double Utils::reportAccuracy(int counter) {
  assert(testSet.size() > 0);
  int num_correct_pred = 0;
  for(size_t i = 0; i < testSet.size(); i++) {
    std::string pattern = testSet[i].first;
    std::string actual_output = testSet[i].second;
    std::string predicted_output;
    bool retVal = DecisionTree::getDecisionTree().getClassification(pattern, predicted_output);
    assert(retVal);
    if(predicted_output.compare(actual_output) == 0) {
      num_correct_pred++;
    }
  }
  return (double) num_correct_pred * 100 / testSet.size();
}

double Utils::calculateEntropy(int num_yes, int num_no) {
  double entropy = 0.0;

  double r1 = (double) num_yes / (num_yes + num_no);
  double r2 = (double) num_no  / (num_yes + num_no);
  
  double r11 = (r1 == 0) ? 0 : (r1 * (log(r1) / log(2)));
  double r22 = (r2 == 0) ? 0 : (r2 * (log(r2) / log(2)));

  entropy = (r11 + r22) == 0 ? 0 : -1 * (r11 + r22);

  return entropy;
}

double Utils::get_wall_time()
{
  struct timeval time;
  if (gettimeofday(&time,NULL)) {
    // Handle error
    return 0;
  }
  return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

double Utils::get_cpu_time()
{
  return (double)clock() / CLOCKS_PER_SEC;
}

void DecisionTree::trainDecisionTree(Examples examples, 
                           std::vector<std::string> attributes, 
                           Examples parents_examples,
                           std::string rNode,
                           std::string edge)
{
  //std::cout << "size of examples: " << examples.size() << ", size of parents_examples: " << parents_examples.size() << std::endl;
  if(examples.size() == 0) {
    std::string plurality = getPlurality(parents_examples);
    if(rootNode.size() == 0) {
      rootNode = plurality;
    }
    assert(rNode.size() > 0 && edge.size() > 0);
    dTree[rNode][edge] = plurality;
    return;
  } 

  std::string classification = allEgHaveSameClassification(examples);
  if(classification.size() > 0) {
    if(rootNode.size() == 0) {
      rootNode = classification;
    }
    assert(rNode.size() > 0 && edge.size() > 0);
    dTree[rNode][edge] = classification;
    return;
  }

  if(attributes.size() == 0) {
    std::string plurality = getPlurality(examples);
    if(rootNode.size() == 0) {
      rootNode = plurality;
    }
    assert(rNode.size() > 0 && edge.size() > 0);
    dTree[rNode][edge] = plurality;
    return;
  }

  std::string most_important_attr = attributes[0];
  double m_importance = getAttrImportance(most_important_attr, examples);
  for(size_t i = 1; i < attributes.size(); i++) {
    double importance = getAttrImportance(attributes[i], examples);
    if(importance > m_importance) {
      most_important_attr = attributes[i];
      m_importance = importance;
    }
  }

  if(rNode.size() == 0 && edge.size() == 0) {
    rootNode = most_important_attr;
  } else {
    assert(rNode.size() > 0 && edge.size() > 0);
    dTree[rNode][edge] = most_important_attr;
  }

  std::vector<std::string> possVals = Utils::getUtils().getAttributeInfo()[most_important_attr];
  for(std::vector<std::string>::iterator vIter = possVals.begin(); vIter != possVals.end(); vIter++) {
    std::string val = *vIter;
    Examples exs;
    for(Examples::iterator eIter = examples.begin(); eIter != examples.end(); eIter++) {
      std::vector<Attribute*> attrVec = (*eIter).first;
      for(std::vector<Attribute*>::iterator aIter = attrVec.begin(); aIter != attrVec.end(); aIter++) {
        std::string attrName = (*aIter)->getAttrName();
        if(most_important_attr.compare(attrName) != 0) continue;
        std::string attrVal = (*aIter)->getAttrVal();
        if(val.compare(attrVal) != 0) continue;
        exs.push_back(std::make_pair(attrVec, (*eIter).second));
      }
    }

    std::vector<std::string> new_attribute_list;  
    for(std::vector<std::string>::iterator aIter = attributes.begin(); aIter != attributes.end(); aIter++) {
      std::string attr = *aIter;
      if(attr.compare(most_important_attr) == 0) continue;
      new_attribute_list.push_back(attr);
    }
   
    trainDecisionTree(exs, new_attribute_list, examples, most_important_attr, val);
  }
  return;
}

std::string DecisionTree::allEgHaveSameClassification(Examples& examples)
{
  std::string classification;
  bool allExamplesHaveSameClassification = true;
  for(Examples::iterator eIter = examples.begin(); eIter != examples.end(); eIter++) {
    if(classification.size() == 0) {
      classification = (*eIter).second;
    } else {
      if(classification.compare((*eIter).second) != 0) {
        allExamplesHaveSameClassification = false;
        break;
      }
    }
  }
  return allExamplesHaveSameClassification ? classification : std::string("");
}

std::string DecisionTree::getPlurality(Examples& examples)
{
  int parent_set_yes = 0, parent_set_no = 0;
  for(Examples::iterator eIter = examples.begin(); eIter != examples.end(); eIter++) {
    if(((*eIter).second).compare("1") == 0) {
      parent_set_yes++;
    } else {
      parent_set_no++;
    }
  }

  return (parent_set_yes >= parent_set_no) ? std::string("1") : std::string("-1");
}

double DecisionTree::getAttrImportance(std::string& attribute, Examples& examples)
{
  int parent_set_yes = 0, parent_set_no = 0;
  for(Examples::iterator eIter = examples.begin(); eIter != examples.end(); eIter++) {
    if(((*eIter).second).compare("1") == 0) {
      parent_set_yes++;
    } else {
      parent_set_no++;
    }
  }

  double parent_entropy = Utils::getUtils().calculateEntropy(parent_set_yes, parent_set_no);

  std::map<std::string, std::pair<int, int> > childMap;
  for(Examples::iterator eIter = examples.begin(); eIter != examples.end(); eIter++) {
    std::vector<Attribute*> attrVec = (*eIter).first;
    for(std::vector<Attribute*>::iterator aIter = attrVec.begin(); aIter != attrVec.end(); aIter++) {
      Attribute *attr = *aIter;
      assert(attr);
      if((attr->getAttrName()).compare(attribute) != 0) continue;
      if(((*eIter).second).compare("1") == 0) {
        childMap[attr->getAttrVal()].first++;
      } else {
        childMap[attr->getAttrVal()].second++;
      }
    }
  }

  double weighted_avg_child_entropy = 0.0;
  for(std::map<std::string, std::pair<int, int> >::iterator cIter = childMap.begin(); cIter != childMap.end(); cIter++) {
    int child_set_yes = (cIter->second).first;
    int child_set_no  = (cIter->second).second;
    double child_entropy = Utils::getUtils().calculateEntropy(child_set_yes, child_set_no);
    weighted_avg_child_entropy += child_entropy * (child_set_yes + child_set_no) / (parent_set_yes + parent_set_no);
  }

  return (parent_entropy - weighted_avg_child_entropy);
  
}

bool DecisionTree::getClassification(std::string& input, std::string& output) {
  if(rootNode.size() == 0 || dTree.size() == 0) {
    std::cout << "Decision Tree has not been built yet. Please build the Decision Tree first ..." << std::endl;
    return false;
  }

  if(input.length() != Utils::getUtils().getAllAttrNames().size()) {
    std::cout << "Pattern length is incorrect. Please run the program again with correct pattern length ..." << std::endl;
    return false;
  }

  std::map<std::string, std::string> attrValMap;
  for(size_t i = 0; i < input.length(); i++) {
    std::string value(1, input[i]);
    attrValMap[Utils::getUtils().getAllAttrNames()[i]] = value;
  }

  output = rootNode;
  while(output.compare("1") != 0 && output.compare("-1") != 0) {
    if(attrValMap[output].size() == 0) {
      std::cout << "Incorrect pattern entered. Please run the program again with a correct pattern ..." << std::endl;
      return false;
    }
    output = dTree[output][attrValMap[output]];
  }

  //std::cout << "Classification for the entered pattern is: " << output << std::endl;

  return true;
}

void DecisionTree::printDecisionTree() {
  std::cout << "root node: " << rootNode << std::endl;
  std::map<std::string, std::map<std::string, std::string> >::iterator dtIter;
  for(dtIter = dTree.begin(); dtIter != dTree.end(); dtIter++) {
    std::string node = dtIter->first;
    std::cout << node << ": ";

    std::map<std::string, std::string> edgeChildMap = dtIter->second;
    std::map<std::string, std::string>::iterator ecIter;
    for(ecIter = edgeChildMap.begin(); ecIter != edgeChildMap.end(); ecIter++) {
      std::cout << ecIter->first << " ---> " << ecIter->second << ", ";
    }
    std::cout << "\n\n";
  }
}

void DecisionTree::clearDT() {
  rootNode.clear();
  dTree.clear();
}
