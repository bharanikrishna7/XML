#include "XML.h"

/** Default Constructor for XML Class.
  *
  * Dependant Function(s) - XML::SetVerbose, XML::SetMagicQuotes, XMLDocument::XMLDocument
  *
  * Ver : 1.0
  *		- First Release.
  */
XML::XML(bool _MagicQuotes, bool _verbose) {
	SetVerbose(_verbose); 
	SetMagicQuotes(_MagicQuotes);

	xDoc = new XMLDocument(VERBOSE);
	Persist = nullptr;
}

/** Default Destructor for XML Class. Deallocates Memory
  * Alotted to xDoc and Persist Variables.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
XML::~XML() {
	delete xDoc;
	delete Persist;
}

/** Method to Check Whether XML Document is Present.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
bool XML::CheckXDoc() {
	if (xDoc == nullptr)
		return false;
	return true;
}

/** Method to Enable Magic Quote Protection.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::SetMagicQuotes(bool _MagicQuotes) {
	MagicQuotes = _MagicQuotes;
	if (VERBOSE) {
		if (MagicQuotes)
			std::cout << "\n [VERBOSE] : Magic Quote Protection Mechanism Enabled";
		else
			std::cout << "\n [VERBOSE] : Magic Quote Protection Mechanism Disabled";
	}
}

/** Method to Set Verbose Mode.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::SetVerbose(bool verbose) {
	VERBOSE = verbose;
	if (VERBOSE)
		std::cout << "\n [VERBOSE] : Verbose Mode Turned On For XML";
}

/** Wrapper Method to Show a Node's Information.
  *
  * Dependant Function(s) - NODE::ShowInfo
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::ShowNodeInfo(Node * node) {
	node->ShowInfo();
}

/** Wrapper Method to Add Attributes to a Node Element.
  *
  * Dependant Function(s) - XMLDocument::AddAttribute
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::AddAttributeToNode(Node* _node, std::string _attribute, std::string _value) {
	xDoc->AddAttribute(_node, _attribute, _value);
}

/** Wrapper Method to Child Node to a Node.
  *
  * Dependant Function(s) - XMLDocument::AddChild
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::AddChildToXDoc(Node * _child, Node * _parent) {
	xDoc->AddChild(_child, _parent);
}

/** Wrapper Method to Child Node to a Node.
  *
  * Dependant Function(s) - XMLDocument::AddChild
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::AddChildToXDoc(Node * _node) {
	xDoc->AddChild(_node);
}

/** Wrapper Method to Create A New Node Element
  *
  * Dependant Function(s) - XMLDocument::CreateNode
  *
  * Ver : 1.0
  *		- First Release.
  */
Node * XML::CreateNode(std::string _tag, std::string _value) {
	return xDoc->CreateNode(_tag, _value);
}

/** Wrapper Method to Display XML Document as Displayed by
  * Tree Command in Windows/Linux.
  *
  * Dependant Function(s) - XMLDocument::ShowXMLTree
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string XML::ShowXDocTree(bool print) {
	return xDoc->ShowXMLTree(print);
}

/** Wrapper Method to Query XML Document and Get All Elements
  * Which Have Same Tag as the Parameter.
  *
  * Dependant Function(s) - XMLDocument::getElementByTag
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XML::GetElementByTag(const std::string & _tagName) {
	return xDoc->getElementByTag(_tagName);
}

/** Wrapper Method to Query XML Document and Get All Elements
  * Which Have Same Tag as the Parameter.
  *
  * Dependant Function(s) - XMLDocument::getElementByValue
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XML::GetElementByValue(const std::string & _value) {
	return xDoc->getElementByValue(_value);
}

/** Wrapper Method to Query XML Document and Get Descendants
  * of a Particular Node.
  *
  * Dependant Function(s) - XMLDocument::getDescendants
  *
  * Ver : 1.0
  *		- First Release.
  */
std::vector<Node *> XML::GetDescendants(const Node * _node) {
	return xDoc->getDescendants(_node);
}

/** Wrapper Method to Write XML Document to Disk.
  *
  * Dependant Function(s) - XML::CheckXDoc, Persistance::ToDisk, Persistance::Persistance
  *
  * Ver : 1.0
  *		- First Release.
  */
void XML::ToDisk(std::string _path) {
	Persist = new Persistance(_path, MagicQuotes, VERBOSE);
	if (CheckXDoc())
		Persist->ToDisk(xDoc);
	else
		std::cout << "\n [Error] : XML Document Cannot be nullptr to use this Function";
}

/** Wrapper Method to Preview The XML Stream Before Writing to XML File.
  *
  * Dependant Function(s) - XML::CheckXDoc, Persistance::PreviewToDisk, Persistance::Persistance
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string XML::PreviewToDisk(bool DisplayOnConsole) {
	Persist = new Persistance("temp.xml", MagicQuotes, VERBOSE);
	if (CheckXDoc())
		return Persist->PreviewToDisk(xDoc, DisplayOnConsole);
	else {
		std::string err = "[Error] : XML Document Cannot be nullptr to use this Function";
		if (DisplayOnConsole)
			std::cout << "\n " << err;
		return err;
	}
}

/** Wrapper Method to Read XML File on Disk and Load it's Contents to XML Document (xDoc).
  * It'll Overwrite the Content in xDoc Element. 
  *
  * Dependant Function(s) - Persistance::Persistance, Persistance::ToXDoc
  *
  * Ver : 2.0
  *		- Added Parameter Force. If True it'll Not Prompt User to that xDoc
  *		  is not Empty.
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument * XML::ToXDoc(std::string _path, bool Force) {
	std::string cont;
	if (Force) {
		std::cout << "\n [WARNING] : XML Document is Not Empty. Contents Will be Overwritten";
		Persist = new Persistance(_path, MagicQuotes, VERBOSE);
		xDoc = Persist->ToXDoc();
		std::cout << "\n Writing XML Data to Disk.";
		return xDoc;
	}

	if (xDoc != nullptr) {
		std::cout << "\n [WARNING] : XML Document is Not Empty. Contents Will be Overwritten";
		std::cout << "\n yes or no : ";
		std::cin >> cont;
	}

	Utilities::StringHelper::ToLower(cont);
	if (cont == "y" || cont == "yes") {
		std::cout << "\n Writing XML Data to Disk.";
		Persist = new Persistance(_path, MagicQuotes, VERBOSE);
		xDoc = Persist->ToXDoc();
	}
	else {
		std::cout << "\n Ignoring Write XML Data to Disk Command.";
	}
	return xDoc;
}

#ifdef TEST_XML

#include "../Utilities/Utilities.h"

using namespace Utilities;

void CreateXDoc(XML * xml) {
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
	node7->value = "SomeValue\n <NewNode>MaliciousContent</NewNode>";
	node7->attributes.push_back(std::make_pair("Name", "Malicious"));

	std::cout << "\n Created 7 Nodes\n\n";

	StringHelper::Title("Add Nodes to the XML Document Structure");
	XMLDocument * xDoc = new XMLDocument();
	xml->AddChildToXDoc(node1, nullptr);
	xml->AddChildToXDoc(node2, node1);
	xml->AddChildToXDoc(node3, node1);
	xml->AddChildToXDoc(node4, node1);
	xml->AddChildToXDoc(node5, node1);
	xml->AddChildToXDoc(node6, node1);
	xml->AddChildToXDoc(node7);

	std::cout << "\n Added 7 Nodes to XML Structure (Tree).";
}

int main() {
	StringHelper::Title("Testing XML Package", '=');
	Timer time;
	time.Begin();

	XML * xml = new XML(true, false);
	CreateXDoc(xml);
	std::cout << "\n\n ";

	StringHelper::Title("Testing ShowXDocTree Feature");
	xml->ShowXDocTree(true);
	std::cout << "\n\n ";

	StringHelper::Title("Testing Preview to Disk Feature");
	std::cout << "\n";
	xml->PreviewToDisk(true);
	std::cout << "\n\n ";

	StringHelper::Title("Writing XML Structure(Tree) to Disk");
	xml->ToDisk("testXML.xml");
	std::cout << "\n\n ";

	StringHelper::Title("Reading XML File \"testXML.xml\" From Disk");
	xml->ToXDoc("testXML.xml", true);
	std::cout << "\n\n ";

	time.End();
	time.TimeForTask();
	
	std::cout << "\n\n\n ";
}
#endif // TEST_XML
