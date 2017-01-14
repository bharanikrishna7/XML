//////////////////////////////////////////////////////////////////////
// XMLDocument.cpp	:	Create in Memory XML Like Structure.		//
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

#include "XMLDocument.h"

/** Constructor For XMLDocument Class.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument::XMLDocument(bool verbose, std::string _version, std::string _encoding) {
	SetVerbose(verbose);
	version = _version;
	encoding = _encoding;

	// Root element, will be used only for reference.
	// Will not be present in XML when writing to a Document.
	root = new Node;
	root->value = "root";
}

/** Method to Set the VERBOSE Variable.
  * VERBOSE = True  (Enables Verbose Mode)
  * VERBOSE = False (Disables Verbose Mode)
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::SetVerbose(bool verbose) {
	VERBOSE = verbose;
	if (VERBOSE)
		std::cout << "\n [VERBOSE] : Verbose Mode Turned On for XMLDocument";
}

/** Function to Create An XML Node. This Function Creates 
  * Node Without Any Arrtibutes And Children.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
Node * XMLDocument::CreateNode(std::string _tag, std::string _value) {
	Node * newNode = new Node;
	newNode->tag = _tag;
	newNode->value = _value;
	return newNode;
}

/** Function to Add Arrtibute to A Node.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::AddAttribute(Node* _node, std::string _attribute, std::string _value) {
	if (_node == nullptr) {
		std::cout << "\n [Error] # Invalid Node";
		return;
	}
	for (std::pair<std::string, std::string> att : _node->attributes) {
		if (att.first == _attribute) {
			std::cout << "\n [WARNING] : Attribute already Exists. Not adding same attribute again";
			return;
		}
	}
	_node->attributes.push_back(std::make_pair(_attribute, _value));
}

/** Function to Add Child Node to The Root Node.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::AddChild(Node * _node) {
	for (Node * child : root->children) {
		if (child == _node) {
			std::cout << "\n [WARNING] : The node has already been added to XML";
			return;
		}
	}
	root->children.push_back(_node);
}

/** Function to Add Child to A Specific Node.
  * (Method Overload)
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::AddChild(Node * _child, Node * _parent) {
	if (_parent == nullptr)
		_parent = root;
	for (Node * child : _parent->children) {
		if (child == _child) {
			std::cout << "\n [WARNING] : The node has already been added to XML";
			return;
		}
	}
	_parent->children.push_back(_child);
}

/** Function to Remove Each Node in the XML Structure (Tree).
  * This Function will is Intended to Prevent Memory Loss Issues.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void CleanUpHelper(Node * _root) {
	if (_root->children.size() != 0) {
		while (!_root->children.empty()) {
			Node * node = _root->children.back();
			_root->children.pop_back();
			CleanUpHelper(node);
		}
	}
	else {
		delete _root;
	}
}
/** Function to Get All the Child Elements of a Specific Node.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XMLDocument::getDescendants(const Node * _node) {
	std::vector<Node *> result;
	if (_node == nullptr)
		return result;
	for (Node * child : _node->children) {
		result.push_back(child);
	}
	return result;
}

/** Default Destructor for XMLDocument Class.
  * Deletes All the Node(s) associated with XML Structure (Tree).
  * 
  * Dependant Function(s) - XMLDocument::CleanUpHelper
  *
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument::~XMLDocument() {
	if (VERBOSE)
		std::cout << "\n [VERBOSE] : Destroying All the Nodes";
	CleanUpHelper(root);
}

/** Function to Display All Node Information.
  * 
  * Dependant Function(s) - Node::ShowInfo
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::ShowNode(Node * node) {
	node->ShowInfo();
}

std::string XMLDocument::ShowXMLTree(bool print) {
	std::string result;
	ShowXMLTreeHelper(result, root, 0);
	if (print)
		std::cout << result;
	return result;
}

/** Function to Display XML Structure(Tree) Similar to
  * Output of Tree Command in Windows/Linux
  * Does not Display Attributes.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::ShowXMLTreeHelper(std::string & tree, Node * node, int indent) {
	tree.push_back('\n');
	tree.push_back('|');
	if (indent != 0) {
		int last = indent - 1;
		for (int i = 0; i < indent; i++) {
			if (i == last) {
				if (i != 0) {
					tree.push_back('|');
					tree.append("-----");
				}
				else {
					tree.append("-----");
				}
			} else
				tree.push_back('\t');
		}
	} 

	tree.append(std::string("[" + node->tag + "] : "));
	if (node->value == "")
		tree.append("N/A");
	else
		tree.append(node->value);

	if (node->children.size() != 0) {
		for (size_t i = 0; i < node->children.size(); i++) {
			ShowXMLTreeHelper(tree, node->children[i], indent + 1);
		}
	}
}

/** Function to Perform A Query On XML to Get All Elements With A Specific
  * Tag Name.
  *
  * Dependant Function(s) - XMLDocument::getElementByTagHelper
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XMLDocument::getElementByTag(const std::string & _tagName) {
	std::vector<Node *> results;
	getElementByTagHelper(results, root, _tagName);
	return results;
}

/** Helper Function For GetElementByTag Function.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::getElementByTagHelper(std::vector<Node *> &vec, Node * node, const std::string & _tagname) {
	if (node == nullptr) {
		return;
	}

	if (node != root && node->tag == _tagname) {
		vec.push_back(node);
	}

	if (!node->children.empty()) {
		for (Node * child : node->children) {
			getElementByTagHelper(vec, child, _tagname);
		}
	}
}

/** Function to Perform A Query On XML to Get All Elements With A Specific
  * Value.
  *
  * Dependant Function(s) - XMLDocument::getElementByValueHelper
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XMLDocument::getElementByValue(const std::string & _value) {
	std::vector<Node *> results;
	getElementByValueHelper(results, root, _value);
	return results;
}

/** Helper Function For GetElementByValue.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::getElementByValueHelper(std::vector<Node *> &vec, Node * node, const std::string & _value) {
	if (node == nullptr) {
		return;
	}

	if (node != root && node->value == _value) {
		vec.push_back(node);
	}

	if (!node->children.empty()) {
		for (Node * child : node->children) {
			getElementByValueHelper(vec, child, _value);
		}
	}
}

/** Function to Get XML Version.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string XMLDocument::getVersion() {
	return version;
}


/** Function to Set XML Version.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::setVersion(std::string & _version) {
	version = _version;
}


/** Function to Get XML Encoding.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string XMLDocument::getEncoding() {
	return encoding;
}

/** Function to Set XML Encoding.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XMLDocument::setEncoding(std::string & _encoding) {
	encoding = _encoding;
}

/** Function to Get Root Element of XML.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
Node * XMLDocument::getRoot() {
	return root;
}

#ifdef TEST_XML_DOCUMENT
#include "../Utilities/Utilities.h"

using namespace Utilities;

/** Function To Test XMLDocument Class.
  */
int main() {
	Timer time;
	time.Begin();

	StringHelper::Title("Testing XML Document Package", '=');
	std::cout << "\n";
	StringHelper::Title("Create Nodes");
	Node * node1 = new Node;
	node1->tag = "Movies";
	node1->value = "Star Wars - Episode 5";
	node1->attributes.push_back(std::make_pair("Year", "1980"));

	Node * node2 = new Node;
	node2->tag = "Genre";
	node2->value = "Action";

	Node * node3 = new Node;
	node3->tag = "Actor";
	node3->value = "Mark Hamill";

	Node * node4 = new Node;
	node4->tag = "Date";
	node4->value = "May 17, 1980";

	Node * node5 = new Node;
	node5->tag = "Actor";
	node5->value = "Harrison Ford";

	Node * node6 = new Node;
	node6->tag = "Actor";
	node6->value = "Carrie Fisher";

	Node * node7 = new Node;
	node7->tag = "Node";
	node7->attributes.push_back(std::make_pair("Name", "7"));

	std::cout << "\n Created 7 Nodes\n\n";

	StringHelper::Title("Add Nodes to the XML Document Structure");
	XMLDocument * xDoc = new XMLDocument(true);
	xDoc->AddChild(node1, nullptr);
	xDoc->AddChild(node2, node1);
	xDoc->AddChild(node3, node1);
	xDoc->AddChild(node4, node1);
	xDoc->AddChild(node5, node1);
	xDoc->AddChild(node6, node1);
	xDoc->AddChild(node7);

	std::cout << "\n Added 7 Nodes to XML Document Structure\n\n";

	StringHelper::Title("Show Node Info");
	std::cout << "\n Show Node 1 Info";
	xDoc->ShowNode(node1);
	std::cout << "\n" << "\n Show Node 7 Info";
	xDoc->ShowNode(node7);
	std::cout << "\n\n";

	StringHelper::Title("Show XML Tree Structure");
	xDoc->ShowXMLTree(true);
	std::cout << "\n\n";

	StringHelper::Title("Demonstrate Get Element By Tag Feature");
	std::cout << "\n Tag Name = Actor";
	std::vector<Node *> tagDemo;
	tagDemo = xDoc->getElementByTag("Actor");
	std::cout << "\n Elements with Actor Tag Name are : ";
	if (!tagDemo.empty()) {
		for (Node * node : tagDemo) {
			node->ShowInfo();
			std::cout << "\n";
		}
	}
	else {
		std::cout << "\n No elements with Tag name : Actor were found in the XDocument\n";
	}
	tagDemo.clear();
	std::cout << "\n";

	StringHelper::Title("Demonstrate Get Element By Value Feature");
	std::cout << "\n Value = Action";
	std::vector<Node *> valueDemo;
	valueDemo = xDoc->getElementByValue("Action");
	std::cout << "\n Elements with Value Action are : ";
	if (!valueDemo.empty()) {
		for (Node * node : valueDemo) {
			node->ShowInfo();
			std::cout << "\n";
		}
	}
	else {
		std::cout << "\n No elements with Value : Action were found in the XDocument\n";
	}
	
	time.End();

	std::cout << "\n";
	time.TimeForTask();

	std::cout << "\n\n\n ";
}
#endif // TEST_XML_DOCUMENT