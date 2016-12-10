/*
 * Decision Tree
 * Author: Prabal Basu (A02049867)
 * Contact: prabalb@aggiemail.usu.edu
 */

#include <iostream>

#include <vector>
#include <map>
#include <utility>
#include <string>

using namespace std;

typedef std::map<std::string, std::vector<std::string> > AttrMap;

class Attribute {

  private:
    std::string attrName;
    std::string value;

  public:
    Attribute(std::string name, std::string val) : attrName(name), value(val) {}
    std::string getAttrName() { return attrName; }
    std::string getAttrVal() { return value; }
};

typedef std::vector<std::pair<std::vector<Attribute*>, std::string> > Examples;

class DecisionTree {

  private:
    std::string rootNode;
    std::map<std::string, std::map<std::string, std::string> > dTree;

    DecisionTree() : rootNode("") {}
    void operator=(DecisionTree&);
    DecisionTree (const DecisionTree&);

  public:
    static DecisionTree& getDecisionTree() {
      static DecisionTree dT;
      return dT;
    }

    void trainDecisionTree(Examples examples, 
                           std::vector<std::string> attributes, 
                           Examples parents_examples,
                           std::string rNode="",
                           std::string egde="");
    void printDecisionTree();
    void clearDT();

    std::string allEgHaveSameClassification(Examples& examples);
    std::string getPlurality(Examples& examples);

    bool getClassification(std::string& input, std::string& output);

    double getAttrImportance(std::string& attribute, Examples& examples);

};


class Utils {

  private:
    Utils() {}
    void operator=(Utils&);
    Utils (const Utils&);

    AttrMap attributeInfo;

    Examples allExamples;
    Examples allTrainingExamples;

    std::vector<std::string> attrNames;
    std::vector<std::pair<std::string, std::string> > testSet;

  public:
    bool populateAttrInfo(std::string trainFileName);
    double calculateEntropy(int num_yes, int num_no);

    AttrMap& getAttributeInfo() { return attributeInfo; }
    Examples& getAllTrainingExamples() { return allTrainingExamples; }

    std::vector<std::string>& getAllAttrNames() { return attrNames; }

    void performRandomSplitBasedValidation();
    void perform10FoldXValidation();
    void randomSplitDataSet();
    void crossSplitDataSet(int counter);
    void resetDB();

    double get_wall_time();
    double get_cpu_time();
    double reportAccuracy(int counter);

    static Utils& getUtils() {
      static Utils utils;
      return utils;
    }
};
