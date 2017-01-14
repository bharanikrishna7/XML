//////////////////////////////////////////////////////////////////////
// Persistance.h	:	Provides Interface to Read (or) Write XML	//
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

/* 
 * PACKAGE OPERATIONS
 * ------------------
 * This package provides a public class Persistance. The Persistance 
 * class provides methods to Read an XML File and Load it's contents
 * to Memory (XMLDocument). Also it provides methods to Write XMLDocument
 * Structure's Contents to Disk.
 *
 * DEPENDANT FILES
 * ---------------
 * Persistance.h, Persistance.cpp, XMLFileProcessor.h, XMLFileProcessor.cpp, 
 * XMLDocument.h, XMLDocument.cpp, Utilities.h, Utilities.cpp
 *
 * CHANGELOG
 * ---------
 * Ver 2.0 : 01/10/2017
 *	- Added Support For VERBOSE Mode.
 *	- Removed XML Protection Magic Quotes while Reading From XML File.
 *	  (Moved it to XMLFileProcessor Package).
 *
 * Ver 1.0 : 01/09/2017
 *	- First Release.
 */

#ifndef PERSISTANCE_H
#define PERSISTANCE_H

#include "../XMLDocument/XMLDocument.h"
#include "../XMLFileProcessor/XMLFileProcessor.h"

#include <stack>
#include <fstream>
#include <unordered_map>

/** Class To Persist Data as well as To Read Data From
  * Disk (a XML File).
  *
  * Dependant Class(es) or Structure(s) - Toker, XMLDocument, StringHelper
  *
  * Ver : 2.0
  *		- Added Support For VERBOSE Mode.
  *		- Removed XML Protection MagicQuotes While Reading From XML File (Moved
  *		  it to Context Structure).
  * Ver : 1.0
  *		- First Release.
  */
class Persistance {
private:
	std::string filePath;
	std::unordered_map<std::string, std::string> MagicQuotesW;
	std::unordered_map<std::string, std::string> MagicQuotesR;
	bool MagicQuoteProtection = false;

	void PreviewToDiskHelper(std::string & str, Node * _node, int indent = 0);
	void generateIndent(std::string & str, int indent);
	void XMLProtectionMagicQuotes(std::string & str);

	bool VERBOSE;
public:
	Persistance(std::string _file, bool _MagicQuoteProtection = false, bool verbose = false);
	~Persistance() {};
	
	void SetPath(std::string _path);
	void ToDisk(XMLDocument * xDoc);
	std::string PreviewToDisk(XMLDocument * xDoc, bool DisplayOnConsole = false);
	XMLDocument * ToXDoc();

	void SetVerbose(bool verbose);
};
#endif // !PERSISTANCE_H
