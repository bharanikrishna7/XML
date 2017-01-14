//////////////////////////////////////////////////////////////////////////
// XMFileProcessor.h	:	Reads XML File Stream and Creates an XML	//
//							Document.									//
// Version				:	2.0											//
// -------------------------------------------------------------------- //
// Copyright © Venkata Chekuri, 2016									//
// All rights granted provided that this notice is retained.			//
// -------------------------------------------------------------------- //
// Source				:	Dr. Jim Fawcett, Syracuse University		//
//							jfawcett@twcny.rr.com						//
// -------------------------------------------------------------------- //
// Language				:	C++ 11										//
// Platform				:	MSI GE62 2QD, Core-i7, Windows 10			//
// Application			:	XML											//
// -------------------------------------------------------------------- //
// Author				:	Venkata Bharani Krishna Chekuri				//
//							vbchekur@syr.edu							//
//////////////////////////////////////////////////////////////////////////

/*
 * PACKAGE OPERATIONS
 * ------------------
 * This package Provides a public Toker class, private State class and
 * a Struct Context.
 * 
 * The Toker Reads words from std::stream sequentially and Performs 
 * operations depending on the Squence of Tokens it has Read. The Objective
 * of This Toker is to Read an XML File and Convert it into XMLDocument.
 *
 * DEPENDANT FILES
 * ---------------
 * XMFileProcessor.h, XMFileProcessor.cpp, XMLDocument.h,
 * XMLDocument.cpp, Utilities.h, Utilities.cpp
 *
 * CHANGELOG
 * ---------
 * Ver 2.0 : 01/10/2017
 *	- Added Compute Function.
 *	- Added Support For VERBOSE Mode.
 *	- Added Support For Magic Quotes (Read) Protection Mechanism.
 *	- Not Using Nodes Anymore in Context.
 *	- Modified Scope. Now it Stores Pointer to Latest Node Encountered
 *	  by Toker (Previously it stored Tag Name of Latesr Node Encountered
 *	  by Toker).
 *
 * Ver 1.0 : 01/09/2017
 *	- First Release.
 */
#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <iosfwd>
#include <string>
#include <iostream>

#include "../Utilities/Utilities.h"
#include "../XMLDocument/XMLDocument.h"

namespace XMLScanner {
	/* Forward Declaration */
	class State;
	/* Forward Declaration */
	struct Context;

	/** Class Which Will Read An XML File Token By Token to
	  * Generate an XML Document. 
	  * This is a State Based Tokenizer i.e Different States
	  * Will be Used to Display Current State of Input XML 
	  * File Stream.
	  *
	  * Dependant Class(es) or Structure(s) - State, Context, XMLDocument
	  *
	  * Ver : 2.0
	  *		- Added Compute Function.
	  *		- Added Support for VERBOSE Mode.
	  * Ver : 1.0
	  *		- First Release.
	  */
	class Toker {
	public:
		Toker(bool verbose = false, bool MagicQuotes = false);
		Toker(const Toker&) = delete;
		~Toker();
		Toker& operator=(const Toker&) = delete;
		bool Attach(std::istream* pIn);
		XMLDocument * Compute(std::string fileSpec);
	private:
		std::string GetTok();
		bool CanRead();
		size_t CurrentLineCount();

		State * _pConsumer;
		Context * _pContext;
		std::string FilePath;

		bool VERBOSE;
	};
}
#endif // !TOKENIZER_H
