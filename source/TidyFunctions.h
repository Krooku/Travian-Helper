#include <tidy.h>
#include <tidybuffio.h>
#include <vector>
#include <string>
#include <iostream>

std::string to_string(ctmbstr cstr);
/* curl write callback, to fill tidy's input buffer...  */
uint write_cb(char* in, uint size, uint nmemb, TidyBuffer* out);
/* Traverse the document tree */
void dumpNode(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::vector<std::string>>& inputs, std::string tag);
/* Traverse the document tree */
void dumpNodeAttacks(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& attrVal, std::string tag1, std::string tag2, std::string attrFirstVal);
void dumpNodeText(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag1, std::string tag2, std::string attrFirstVal);
void dumpNodeText2(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag1, std::string tag2, std::string attrFirstVal1, std::string attrFirstVal2);

void dumpNodeGetText(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag1, std::string tag2, std::string attrFirstVal1, int deep);

void dumpNodeAllChild(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts);

void dumpNodeGetAttrValue(TidyDoc doc, TidyNode tnod, int indent, std::string nameTag, std::vector<std::string> attrName, std::vector<std::string> attrValue, std::string& found);


void dumpNodeGetText1(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag, std::string attrName, std::string attrValue);


//////////////// TRYHARD

void getTextsFromallChildsOfChild(TidyDoc doc, TidyNode tnod, std::vector<std::string>& texts, std::string tag1, std::string attr1Name, std::string attr1Value, bool getChilds);

void GetAttrValue(TidyDoc doc, TidyNode tnod, int indent, std::string nameTag, std::string attrName, std::string attrValue, std::vector<std::string>& attrVal);

//void getTextsFromAllChild(TidyDoc doc, TidyNode tnod, std::vector<std::string>& texts,)