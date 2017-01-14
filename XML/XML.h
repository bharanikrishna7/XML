//////////////////////////////////////////////////////////////////////
// XML.h			:	Create XML Like In-Memory Structure along	//
//						with options to Read from and Write to, XML	//
//						Document From Disk.							//
// Version			:	1.0											//
// ---------------------------------------------------------------- //
// Copyright © Venkata Chekuri, 2016								//
// All rights granted provided that this notice is retained.		//
// ---------------------------------------------------------------- //
// Source			:	Dr. Jim Fawcett, Syracuse University		//
//						jfawcett@twcny.rr.com						//
// ---------------------------------------------------------------- //
// Language			:	C++ 11										//
// Platform			:	MSI GE62 2QD, Core-i7, Windows 10			//
// Application		:	XML											//
// ---------------------------------------------------------------- //
// Author			:	Venkata Bharani Krishna Chekuri				//
//						vbchekur@syr.edu							//
//////////////////////////////////////////////////////////////////////

/*
 * PACKAGE OPERATIONS
 * ------------------
 * This Package provides users a Data Structure which mimics XML. Apart from
 * That it also Provides Functions to Query the XML as Well as Read XML File
 * Present on Disk and Load it to Memory or Write the XML File Present in
 * Memory to Disk.
 *
 * DEPENDANT FILES
 * ---------------
 * XML.h, XML.cpp, Persistance.h, Persistance.cpp, Tokenizer.h, Tokenizer.cpp,
 * XMLDocument.h, XMLDocument.cpp, Utilities.h, Utilities.cpp
 *
 * CHANGELOG
 * ---------
 * Ver 1.0 : 01/11/2017
 *	- First Release
 */

#ifndef XML_H
#define XML_H

#include "../Persistance/Persistance.h"
#include "../XMLDocument/XMLDocument.h"

class XML {
private:
	XMLDocument * xDoc;
	Persistance * Persist;

	bool MagicQuotes;
	bool VERBOSE;
public:
	XML(bool _MagicQuotes = false, bool _verbose = false);
	~XML();

	// Methods to Make Sure that xDoc And/Or Persist are not NULL
	bool CheckXDoc();
	
	// Methods Associated with XMLDocument
	void ShowNodeInfo(Node * node);
	void AddAttributeToNode(Node* _node, std::string _attribute, std::string _value = "");
	void AddChildToXDoc(Node * _child, Node * _parent);
	void AddChildToXDoc(Node * _node);
	Node * CreateNode(std::string _tag, std::string _value);
	std::string ShowXDocTree(bool print = false);
	void SetVerbose(bool verbose);
	void SetMagicQuotes(bool _MagicQuotes);

	// Methods Associated with Querying XML
	std::vector<Node *> GetElementByTag(const std::string & _tagName);
	std::vector<Node *> GetElementByValue(const std::string & _value);
	std::vector<Node *> GetDescendants(const Node * _node);

	// Methods Associated with Writing to or Reading from an XML File
	void ToDisk(std::string _path);
	std::string PreviewToDisk(bool DisplayOnConsole = false);
	XMLDocument * ToXDoc(std::string _path, bool Force = false);
};
#endif // !XML_H