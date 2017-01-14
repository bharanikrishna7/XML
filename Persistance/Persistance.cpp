//////////////////////////////////////////////////////////////////////
// Persistance.cpp	:	Provides Interface to Read (or) Write XML	//
//						Document to (or) from Disk.					//
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

#include "Persistance.h"

/** Constructor for Persistance Package/Class.
  * By Default XML Protection is Turned Off.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 2.0
  *		- Removed Modes and MagicQuotesR.
  *		- VERBOSE Mode Initialization.
  * Ver : 1.0
  *		- First Release.
  */
Persistance::Persistance(std::string _file, bool _MagicQuoteProtection, bool verbose) {
	SetVerbose(verbose); 
	SetPath(_file);
	MagicQuoteProtection = _MagicQuoteProtection;
	
	if(VERBOSE) {
		if (_MagicQuoteProtection)
			std::cout << "\n [VERBOSE] : Magic Quote Protection Mechanism (Write) = Enabled";
		else
			std::cout << "\n [VERBOSE] : Magic Quote Protection Mechanism (Write) = Disabled";
	}

	// XML Protection Mechanism While Writing to Disk
	MagicQuotesW.insert(std::make_pair("<", ";opBracket"));
	MagicQuotesW.insert(std::make_pair(">", ";clBracket"));
	MagicQuotesW.insert(std::make_pair("\"", ";dbQuotes"));
	MagicQuotesW.insert(std::make_pair("/", ";bkSlash"));
	MagicQuotesW.insert(std::make_pair("\n", ";nlWhiteSpace"));
	MagicQuotesW.insert(std::make_pair("\t", ";tbWhiteSpace"));
	MagicQuotesW.insert(std::make_pair("=", ";eqSymbol"));
}

/** Method to Set File Path to Write or Read XML Content.
  * 
  * Dependant Function(s) - N/A
  * 
  * Ver : 1.0
  *		- First Release.
  */
void Persistance::SetPath(std::string _path) {
	filePath = _path;

	if (VERBOSE)
		std::cout << "\n [VERBOSE] : File Path = \"" << filePath << "\"";
}

/** Method to Write the XML Document to Disk.
  * 
  * Dependant Function(s) - Persistance::PreviewToDisk
  *
  * Ver : 1.0
  *		- First Release.
  */
void Persistance::ToDisk(XMLDocument * xDoc) {
	std::ofstream myfile;
	std::ifstream ifs(filePath);
	
	if (!ifs.good()) {
		std::cout << "\n [WARNING] # File : \"" << filePath << "\" doesnot exists. Creating the file";
	}
	else {
		std::cout << "\n Writing to File : \"" << filePath << "\"";
	}

	myfile.open(filePath);
	myfile << PreviewToDisk(xDoc);
}

/** Method to Generate XML File Which will be written to the File 
  * Path Set Using the Constructor Function. 
  * This Function will Print the XML to Console When the DisplayOnConsole
  * Variable is Set to True.
  * 
  * Dependant Function(s) - Persistance::PreviewToDiskHelper, XMLDocument::getVersion, 
  *							XMLDocument::getEncoding, XMLDocument::getRoot
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string Persistance::PreviewToDisk(XMLDocument * xDoc, bool DisplayOnConsole) {
	std::stringstream ss;
	ss << "<?xml version=\"" << xDoc->getVersion() << "\" encoding=\"" << xDoc->getEncoding() << "\"?>";
	
	for (Node * node : xDoc->getRoot()->children) {
		std::string nodeString;
		PreviewToDiskHelper(nodeString, node, 0);
		ss << nodeString;
	}
	ss << "\0";

	if (DisplayOnConsole)
		std::cout << ss.str();

	return ss.str();
}

/** Method to Generate Indent to Make the XML Easy to Understand for Humans.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void Persistance::generateIndent(std::string & str, int indent) {
	str.clear();
	str.push_back('\n');
	for (int i = 0; i < indent; i++)
		str.push_back('\t');
}

/** Helper Function for Persistance::PreviewToDisk Which Helps in Generating the
  * XML File Using XML Document. This Generated String Usually will be Written to
  * Disk.
  *
  * Dependant Function(s) - Persistance::MagicQuoteProtection, Persistance::generateIndent
  *
  * Ver : 1.0
  *		- First Release.
  */
void Persistance::PreviewToDiskHelper(std::string & str, Node * _node, int indent) {
	if (_node == nullptr)
		return;

	std::string tag = _node->tag;
	std::string val = _node->value;
	
	if (MagicQuoteProtection) {
		XMLProtectionMagicQuotes(tag);
		XMLProtectionMagicQuotes(val);
	}

	std::string indentStr;
	generateIndent(indentStr, indent);

	str.append(indentStr);
	str.append("<" + tag);
	
	if (!_node->attributes.empty()) {
		for (std::pair<std::string, std::string> pr : _node->attributes) {
			if (MagicQuoteProtection) {
				XMLProtectionMagicQuotes(pr.first);
				XMLProtectionMagicQuotes(pr.second);
			}
			str.append(" " + pr.first + "=\"" + pr.second + "\"");
		}
	}

	str.append(">");

	if (!_node->children.empty()) {
		for (Node * child : _node->children) {
			PreviewToDiskHelper(str, child, indent + 1);
		}
	}

	if (!_node->children.empty()) {
		str.append(indentStr + "\t");
	}
	str.append(val);
	if (!_node->children.empty())
		str.append(indentStr);
	str.append("</" + _node->tag + ">");
}

/** Method to Change Certain Characters Before Writing the XML to the Disk
  * or While Reading From Disk. This will Prevent the Users From Corrupting
  * the XML by Inserting XML Tags in the Value Field(s).
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 2.0
  *		- Removed MagicQuote Protection Modes. Now this is only used for Writing
  *		  XML File to Disk.
  *	Ver : 1.0
  *		- First Release.
  */
void Persistance::XMLProtectionMagicQuotes(std::string & str) {
	std::string protectedStr;
	std::string chStr;
	for (char ch : str) {
		chStr = ch;
		if (MagicQuotesW.find(chStr) != MagicQuotesW.end()) {
			protectedStr.append(MagicQuotesW[chStr]);
		}
		else {
			protectedStr.push_back(ch);
		}
	}
	str = protectedStr;
}

/** Method to Read XML File and Generate XML Document Structure using it.
  *
  * Dependant Function(s) - Toker::Compute, Toker::Toker
  *
  * Ver : 2.0
  *		- Updated Toker to be a Pointer as Toker Constructor Has Been Updated
  *		  to Support MagicQuoteProtection & VERBOSE Mode.
  *		- Modified the Interface to Return XMLDocument (Previously Returned Boolean)
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument * Persistance::ToXDoc() {
	using namespace XMLScanner;
	Toker * toker = new Toker(VERBOSE, MagicQuoteProtection);
	return toker->Compute(filePath);
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
void Persistance::SetVerbose(bool verbose) {
	VERBOSE = verbose;
	if (VERBOSE)
		std::cout << "\n [VERBOSE] : Verbose Mode Turned On for Persistance";
}

#ifdef TEST_PERSISTANCE

#include "../Utilities/Utilities.h"

using namespace Utilities;
/** Method to Create XML Document For Testing Purpose.
  *
  * Dependant Function(s) : XMLDocument::XMLDocument, XMLDocument::AddChild
  *							StringHelper::Title
  *
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument * CreateXDoc() {
	std::cout << "\n Create Nodes";
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
	xDoc->AddChild(node1, nullptr);
	xDoc->AddChild(node2, node1);
	xDoc->AddChild(node3, node1);
	xDoc->AddChild(node4, node1);
	xDoc->AddChild(node5, node1);
	xDoc->AddChild(node6, node1);
	xDoc->AddChild(node7);

	std::cout << "\n Added 7 Nodes to XML Document Structure";
	return xDoc;
}

/** Method to Test Persistance Package.
  * 
  * Dependant Function(s) : CreateXDoc, Persistance::Persistance, Persistance::PreviewToDisk,
  *							Persistance::ToDisk, StringHelper::Title, Timer::Begin, Timer::End, 
  *							Timer::TimeForTask
  *
  * Ver : 2.0
  *		- Updated Read From XML Task (Since ToXDoc Function was Changed).
  * Ver : 1.0
  *		- First Release.
  */
int main() {
	Timer time;
	time.Begin();
	StringHelper::Title("Testing Persistance Package", '=');
	StringHelper::Title("Create XMLDocument");
	XMLDocument * xDoc;
	xDoc = CreateXDoc();

	std::cout << "\n\n";

	StringHelper::Title("Create Persistance Instance");
	Persistance * persist = new Persistance("testPersistance.xml", true, true);
	std::cout << "\n Persistance Instance Created !";
	std::cout << "\n\n";

	StringHelper::Title("XML Document to String Preview");
	std::cout << "\n";
	std::cout << persist->PreviewToDisk(xDoc);
	std::cout << "\n\n";

	StringHelper::Title("Write XML Preview to XML File");
	persist->ToDisk(xDoc);
	std::cout << "\n\n";

	StringHelper::Title("Read From XML File"); 
	XMLDocument * xDocNew;
	xDocNew = persist->ToXDoc();
	if (xDocNew != nullptr) {
		std::cout << "\n Read Operation Complete !";

		std::cout << "\n\n Show XML Info: ";
		xDocNew->ShowXMLTree(true);
	}
	std::cout << "\n\n ";
	
	time.End();
	std::cout << "\n ";
	time.TimeForTask();


	std::cout << "\n\n\n ";
}
#endif // TEST_PERSISTANCE