//////////////////////////////////////////////////////////////////////
// XMLDocument.h	:	Create in-Memory XML Like Structure.		//
// Version			:	2.0											//
// -----------------------------------------------------------------//
// Copyright © Venkata Chekuri, 2016								//
// All rights granted provided that this notice is retained.		//
// -----------------------------------------------------------------//
// Language			:	C++ 11										//
// Platform			:	MSI GE62 2QD, Core-i7, Windows 10			//
// Application		:	XML											//
// -----------------------------------------------------------------//
// Author			:	Venkata Bharani Krishna Chekuri				//
//						vbchekur@syr.edu							//
//////////////////////////////////////////////////////////////////////

/*
 * PACKAGE OPERATIONS
 * ------------------
 * This package provides a public structure Node and public class XMLDocument.
 * 
 * Node Structure mimics Tag present in an XML File. It has all of components 
 * which an XML Tag Has [Tag, Attributes(Optional), Value(optional) & Children).
 * There is also a Method in the Struct to legibly print the Node Information.
 * 
 * XMLDocument Class mimics an XML File. It holds a Tree Type Data Structure to
 * store XML Tags (in this case Nodes). Along with this it also stores XML headers
 * like Version & Encoding. There are also methods present in this class which
 * allows the user to Query XML.
 *
 * DEPENDANT FILES
 * ---------------
 * XMLDocument.h, XMLDocument.cpp, Utilities.h, Utilities.cpp
 *
 * CHANGELOG
 * ---------
 * Ver 2.0 : 01/10/2017
 *	- Added Support For VERBOSE Mode.
 *
 * Ver 1.0 : 01/08/2017
 *	- First Release.
 */

#ifndef XML_DOCUMENT_H
#define XML_DOCUMENT_H

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <iostream>

/** Structure to Hold The Basic XML Tag, Tag Attributes, Tag Value
  * and Pointers to Associated Child Elements.
  * Also Has a Method to Display All the Node Information.
  * 
  * Dependant Class(es) or Structure(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
struct Node {
	std::string tag;
	std::string value;
	std::vector<std::pair<std::string, std::string>> attributes;
	std::vector<Node *> children;

	void ShowInfo() {
		std::stringstream ss;
		ss << "\n Node Info";
		ss << "\n ---------";
		ss << "\n [Tag]\t\t: " << tag;
		ss << "\n [Value]\t: " << value;
		ss << "\n [Attributes]\t: ";
		for (std::pair<std::string, std::string> pr : attributes) {
			ss << "\n   --" << pr.first << "\t- " << pr.second;
		}
		ss << "\n Children Count : " << children.size();
		std::cout << ss.str();
	};
};

/** Class to Create XML Like Structure But in a Tree Like Format
  * so that Querying the XML is Faster and Easier.
  *
  * Dependant Class(es) or Structure(s) - N/A
  *
  * Ver : 2.0
  *		- Added VERBOSE Mode.
  * Ver : 1.0
  *		- First Release.
  */
class XMLDocument {
private:
	Node * root;
	std::string version;
	std::string encoding;
	bool VERBOSE;

	void getElementByTagHelper(std::vector<Node *> &vec, Node * node, const std::string & _tagname);
	void getElementByValueHelper(std::vector<Node *> &vec, Node * node, const std::string & _value);
	void ShowXMLTreeHelper(std::string & tree, Node * _node, int indent = 0);
public:
	XMLDocument(bool verbose = false, std::string _version = "1.0", std::string _encoding = "UTF-8");
	~XMLDocument();

	// Methods associated with constructing XML
	void ShowNode(Node * node);
	void AddAttribute(Node* _node, std::string _attribute, std::string _value = "");
	void AddChild(Node * _child, Node * _parent);
	void AddChild(Node * _node);
	Node * CreateNode(std::string _tag, std::string _value);
	std::string ShowXMLTree(bool print = false);
	void SetVerbose(bool verbose);

	// Methods associated with querying XML
	std::vector<Node *> getElementByTag(const std::string & _tagName);
	std::vector<Node *> getElementByValue(const std::string & _value);
	std::vector<Node *> getDescendants(const Node * _node);

	std::string getVersion();
	void setVersion(std::string & _version);
	std::string getEncoding();
	void setEncoding(std::string & _encoding);
	Node * getRoot();
};
#endif // !XML_DOCUMENT_H