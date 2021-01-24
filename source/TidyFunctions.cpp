#include "TidyFunctions.h"

std::string to_string(ctmbstr cstr) {
	std::string str;
	int i = 0;
	while (cstr[i] != '\0') {
		str.push_back(cstr[i]);
		i++;
	}
	return str;
}

/* curl write callback, to fill tidy's input buffer...  */
uint write_cb(char* in, uint size, uint nmemb, TidyBuffer* out)
{
	uint r;
	r = size * nmemb;
	tidyBufAppend(out, in, r);
	return r;
}

/* Traverse the document tree */
void dumpNode(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::vector<std::string>>& inputs, std::string tag)
{
	TidyNode child;
	for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name && to_string(name) == tag) {
			/* if it has a name, then it's an HTML tag ... */
			TidyAttr attr;
			std::cout << name;
			std::vector<std::string> attrs;
			/* walk the attribute list */
			for (attr = tidyAttrFirst(child); attr; attr = tidyAttrNext(attr)) {
				if (tidyAttrValue(attr)) {
					std::cout << " " << tidyAttrName(attr) << ": " << to_string(tidyAttrValue(attr));
					attrs.push_back(to_string(tidyAttrName(attr)));
					attrs.push_back(to_string(tidyAttrValue(attr)));
				}
			}
			if (attrs.size() != (size_t)0)
				inputs.push_back(attrs);
			std::cout << std::endl;
		}
		else {
			/* if it doesn't have a name, then it's probably text, cdata, etc... */
			TidyBuffer buf;
			tidyBufInit(&buf);
			//if(tidyNodeGetValue(doc, tnod, &buf))
				//std::cout << buf.bp << std::endl;
			//std::cout << (char*)buf.bp << std::endl;
			// printf("%*.*s\n", indent, indent, buf.bp ? (char*)buf.bp : "");
			// if((char*)buf.bp[1] == "i" && (char*)buf.bp[2] == "n")
			//std::string buferek = (char*)buf.bp;
			//std::string input = "input";
			//std::size_t found = buferek.find(input);
			//if (found != -1) {
				//std::cout << buferek << std::endl;
			//}
			// if (buferek.length() > 2 && buferek[1] == input[0] && buferek[2] == input[1])
			//std::cout << " " << buferek << std::endl;
			//Sleep(10000);
			tidyBufFree(&buf);
		}
		dumpNode(doc, child, indent + 4, inputs, tag); /* recursive */
	}
}

/* Traverse the document tree */
void dumpNodeAttacks(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string> & attrVal, std::string tag1, std::string tag2, std::string attrFirstVal)
{
	TidyNode child;
	for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name && to_string(name) == tag1) {
			/* if it has a name, then it's an HTML tag ... */
			TidyNode nextChild = tidyGetChild(child);
			ctmbstr nextChild_name = tidyNodeGetName(nextChild);
			if (nextChild_name && to_string(nextChild_name) == tag2) {
				TidyAttr attr1;
				for (attr1 = tidyAttrFirst(nextChild); attr1; attr1 = tidyAttrNext(attr1)) {
					if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attrFirstVal) {
						//std::cout << tidyAttrName(attr1) << " " << tidyAttrValue(attr1) << std::endl;
						TidyAttr attr2 = tidyAttrFirst(child);
						if (tidyAttrValue(attr2))
							attrVal.push_back(to_string(tidyAttrValue(attr2)));
						break;
					}
				}
			}
		}
		else {

		}
		dumpNodeAttacks(doc, child, indent + 4, attrVal, tag1, tag2, attrFirstVal); /* recursive */
	}
}

void dumpNodeText(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string> & texts, std::string tag1, std::string tag2, std::string attrFirstVal)
{
	TidyNode child;
	for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name && to_string(name) == tag1) {
			/* if it has a name, then it's an HTML tag ... */
			TidyAttr attr = tidyAttrFirst(child);
			if (tidyAttrValue(attr) && to_string(tidyAttrValue(attr)) == attrFirstVal) {
				TidyNode nextChild = tidyGetChild(child);
				ctmbstr nextChild_name = tidyNodeGetName(nextChild);

				if (nextChild_name && to_string(nextChild_name) == tag2) {
					TidyNode lastChild = tidyGetChild(nextChild);
					ctmbstr lastChild_name = tidyNodeGetName(lastChild);
					if (lastChild_name) {
						continue;
					}
					TidyBuffer buf;
					tidyBufInit(&buf);
					tidyNodeGetText(doc, lastChild, &buf);
					if (buf.bp) {
						std::string text = (char*)buf.bp;
						//text.erase(0, 7);
						//std::cout << text << std::endl;
						texts.push_back(text);
					}
					else
						std::cerr << "buf.bp not exist" << std::endl;
					tidyBufFree(&buf);
				}

			}
		}
		else {
			/* if it doesn't have a name, then it's probably text, cdata, etc... */
		}
		dumpNodeText(doc, child, indent + 4, texts, tag1, tag2, attrFirstVal); /* recursive */
	}
}

void dumpNodeText2(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag1, std::string tag2, std::string attrFirstVal1, std::string attrFirstVal2)
{
	TidyNode child;

	for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
		ctmbstr name = tidyNodeGetName(child);
		if (name && to_string(name) == tag1) {
			/* if it has a name, then it's an HTML tag ... */
			TidyAttr attr = tidyAttrFirst(child);
			if (tidyAttrValue(attr) && to_string(tidyAttrValue(attr)) == attrFirstVal1) {
				TidyNode nextChild;

				for (nextChild = tidyGetChild(child); nextChild; nextChild = tidyGetNext(nextChild)) {
					ctmbstr nextChild_name = tidyNodeGetName(nextChild);
					if (nextChild_name && to_string(nextChild_name) == tag2) {
						TidyAttr attr2 = tidyAttrFirst(nextChild);
						if (tidyAttrValue(attr2) && to_string(tidyAttrName(attr2)) == attrFirstVal2) {
							TidyNode lastChild = tidyGetChild(nextChild);
							ctmbstr lastChild_name = tidyNodeGetName(lastChild);
							if (lastChild_name) {
								continue;
							}
							TidyBuffer buf;
							tidyBufInit(&buf);
							tidyNodeGetText(doc, lastChild, &buf);
							if (buf.bp) {
								std::string text = (char*)buf.bp;
								//text.erase(0, 7);
								//std::cout << text << std::endl;
								texts.push_back(text);
							}
							else
								std::cerr << "buf.bp not exist" << std::endl;
							tidyBufFree(&buf);
						}
					}
				}
			}
		}
		else {
			/* if it doesn't have a name, then it's probably text, cdata, etc... */
		}
		dumpNodeText2(doc, child, indent + 4, texts, tag1, tag2, attrFirstVal1, attrFirstVal2); /* recursive */
	}
}

void dumpNodeGetText(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag1, std::string tag2, std::string attrFirstVal1, int deep)
{
	TidyNode child1;

	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1 && to_string(name1) == "a") {
			TidyAttr attr1 = tidyAttrFirst(child1);
			if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attrFirstVal1) {
				//std::cout << indent << " " << name1 << "{" << std::endl;
				if (deep == 0) {
					std::cout << indent << " " << name1 << "{" << std::endl;
					dumpNodeAllChild(doc, tnod, indent + 4, texts);
					std::cout << "}" << std::endl;
					deep = 1;
				}
				//std::cout << "}" << std::endl;
				deep = 1;
			}
			else {
				deep = 1;
			}
			deep = 1;
		}
		else {
			/* if it doesn't have a name, then it's probably text, cdata, etc... */
		}
		if(deep == 0)
			dumpNodeGetText(doc, child1, indent + 4, texts, tag1, tag2, attrFirstVal1, deep); /* recursive */
	}
}

void dumpNodeAllChild(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts)
{
	TidyNode child1;

	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1) {
			//std::cout << indent << "\t" << name1 << std::endl;
			TidyAttr attr = tidyAttrFirst(child1);
			if (tidyAttrValue(attr)) {
				std::cout << indent << "\t" << name1 << " " << tidyAttrName(attr) << " = " << tidyAttrValue(attr) << std::endl;
			}
			TidyNode child2 = tidyGetChild(child1);
			ctmbstr name2 = tidyNodeGetName(child2);
			if (name2) {
				std::cout << "\t\t" << name2 << std::endl;
				continue;
			}
			TidyAttr attr1 = tidyAttrFirst(child1);
			if (tidyAttrValue(attr1) && to_string(tidyAttrName(attr1)) == "id") {
				TidyBuffer buf;
				tidyBufInit(&buf);
				tidyNodeGetText(doc, child2, &buf);
				if (buf.bp) {
					std::string text = (char*)buf.bp;
					//text.erase(0, 7);
					std::cout << text << std::endl;
					texts.push_back(text);
				}
				else
					std::cerr << "buf.bp not exist" << std::endl;
				tidyBufFree(&buf);
			}
				
		}
		else {
			/* if it doesn't have a name, then it's probably text, cdata, etc... */
		}

		//dumpNodeAllChild(doc, child1, indent + 4, texts); /* recursive */
	}
}


void dumpNodeGetAttrValue(TidyDoc doc, TidyNode tnod, int indent, std::string nameTag, std::vector<std::string> attrName, std::vector<std::string> attrValue, std::string &found) {
	TidyNode child1;
	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1 && to_string(name1) == nameTag) {
			TidyAttr attr1;
			size_t i = 0;
			for (attr1 = tidyAttrFirst(child1); attr1; attr1 = tidyAttrNext(attr1)) {
				if (i == attrName.size()) {
					found = to_string(tidyAttrValue(attr1));
					std::cout << found << std::endl;
					break;
				}

				if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attrValue[i] && to_string(tidyAttrName(attr1)) == attrName[i]) {
					i++;
				}
				else {
					break;
				}
			}
		}
		dumpNodeGetAttrValue(doc, child1, indent + 4, nameTag, attrName, attrValue, found);
	}
}

void dumpNodeGetText1(TidyDoc doc, TidyNode tnod, int indent, std::vector<std::string>& texts, std::string tag, std::string attrName, std::string attrValue) {
	TidyNode child1;
	bool flag = false;
	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1 && to_string(name1) == tag) {
			TidyAttr attr1 = tidyAttrFirst(child1);
			if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attrValue && to_string(tidyAttrName(attr1)) == attrName) {
				attr1 = tidyAttrNext(attr1);
				std::cout << to_string(name1) << std::endl;
				if (tidyAttrValue(attr1)) {
					continue;
				}
				else {
					TidyNode child2;
					child2 = tidyGetChild(child1);
					ctmbstr name2 = tidyNodeGetName(child2);
					if (name2) {
						continue;
					}
					else {
						TidyBuffer buf;
						tidyBufInit(&buf);
						tidyNodeGetText(doc, child2, &buf);
						if (buf.bp) {
							std::string text = (char*)buf.bp;
							std::cout << text << std::endl;
							texts.push_back(text);
						}
						else
							std::cerr << "buf.bp not exist" << std::endl;
						tidyBufFree(&buf);
					}

				}
			}

		}
		else if(flag){
			TidyBuffer buf;
			tidyBufInit(&buf);
			tidyNodeGetText(doc, child1, &buf);
			if (buf.bp) {
				std::string text = (char*)buf.bp;
				std::cout << text << std::endl;
				texts.push_back(text);
			}
			else
				std::cerr << "buf.bp not exist" << std::endl;
			tidyBufFree(&buf);
			flag = false;
		}
		dumpNodeGetText1(doc, child1, indent + 4, texts, tag, attrName, attrValue);
	}
}


void getTextsFromallChildsOfChild(TidyDoc doc, TidyNode tnod, std::vector<std::string>& texts, std::string tag1, std::string attr1Name, std::string attr1Value, bool getChilds) {
	TidyNode child1;
	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1 && to_string(name1) == tag1) {
			TidyAttr attr1 = tidyAttrFirst(child1);
			if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attr1Value && to_string(tidyAttrName(attr1)) == attr1Name) {
				//tag1 = tag2;
				getChilds = true;
			}
			else
				getChilds = false;

		}
		else if (getChilds && !name1) {
			TidyBuffer buf;
			tidyBufInit(&buf);
			tidyNodeGetText(doc, child1, &buf);
			if (buf.bp) {
				std::string text = (char*)buf.bp;
				texts.push_back(text);
			}
			else
				std::cerr << "buf.bp not exist" << std::endl;
			tidyBufFree(&buf);
		}
		getTextsFromallChildsOfChild(doc, child1, texts, tag1, attr1Name, attr1Value, getChilds);
	}
}




void GetAttrValue(TidyDoc doc, TidyNode tnod, int indent, std::string nameTag, std::string attrName, std::string attrValue, std::vector<std::string>&attrVal) {
	TidyNode child1;
	for (child1 = tidyGetChild(tnod); child1; child1 = tidyGetNext(child1)) {
		ctmbstr name1 = tidyNodeGetName(child1);
		if (name1 && to_string(name1) == nameTag) {
			TidyAttr attr1;
			bool takeNext = false;
			for (attr1 = tidyAttrFirst(child1); attr1; attr1 = tidyAttrNext(attr1)) {
				if (tidyAttrValue(attr1) && to_string(tidyAttrValue(attr1)) == attrValue && to_string(tidyAttrName(attr1)) == attrName) {
					takeNext = true;
				}
				else if (takeNext && tidyAttrValue(attr1)) {
					attrVal.push_back(to_string(tidyAttrValue(attr1)));
					break;
				}
			}
		}
		GetAttrValue(doc, child1, indent + 4, nameTag, attrName, attrValue, attrVal);
	}
}