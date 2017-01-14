//////////////////////////////////////////////////////////////////////////
// XMFileProcessor.cpp	:	Reads XML File Stream and Creates an XML	//
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

#include <stack>
#include <cctype>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include "XMLFileProcessor.h"

namespace XMLScanner {
	/** Structure to Store Context Information. The
	  * Context Info Includes :
	  *		- istream
	  *		- XML Document
	  *		- XML Scope Information
	  *		- Current States & Available States
	  *		- Magic Quotes Protection Scheme Information
	  *
	  * This Structure is the Most Crucial Part of the Tokenizer
	  * Since it Holds all the Information.
	  * 
	  * Dependant Class(es) or Structure(s) - N/A
	  * 
	  * Ver : 2.0
	  *		- Updated Scope To be A Vector of Node Pointers From Vector
	  *		  of Strings.
	  *		- Removed Node Variable (Used to be std::vector<std::pair<std::string, Node *>>).
	  *		- Added VERBOSE Mode Support.
	  * Ver : 1.0
	  *		- First Release.
	  */
	struct Context {
		Context();
		~Context();

		std::string token;
		std::istream* _pIn;

		int prevChar;
		int currChar;
		size_t _lineCount;
		
		XMLDocument * xDoc;

		std::stack<Node *> _scope;
		std::string _version;
		std::string _encoding;
		bool isCorrupt = false;

		bool VERBOSE;

		State * _pState;
		State * _pEatWhitespace;
		State * _pEatOpenTag;
		State * _pEatCloseTag;
		State * _pEatValue;
		State * _pEatHeaders;

		bool MagicQuotes;
		std::unordered_map<std::string, std::string> MagicQuotesR;
		void XMLProtectionMagicQuotes(std::string & str);
	};

	/** Template Class for Different State Classes. These derived States 
	  * will help in processing the XML File (each state will have different
	  * eatChars methods to process specific tokens).
	  *
	  * Dependant Class(es) or Structure(s) - Toker, Context
	  * 
	  * Ver : 1.0
	  *		- First Release.
	  */
	class State {
		friend class Toker;
	public:
		using Token = std::string;
		State();
		State(const State&) = delete;
		State& operator=(const State&) = delete;
		virtual ~State();
		void Attach(std::istream * pIn);
		virtual void EatChars() = 0;
		void ConsumeChars() {
			_pContext->_pState->EatChars();
			_pContext->_pState = NextState();
		}
		std::vector<char> specialChars = { '<', '>' };
		bool CanRead() { return _pContext->_pIn->good(); }
		std::string GetTok() { return _pContext->token; }
		bool HasTok() { return _pContext->token.size() > 0; }
		State * NextState();
		size_t CurrentLineCount() { return _pContext->_lineCount; }
		void SetContext(Context * pContext);
	protected:
		Context * _pContext;
		bool CollectChar();
		Token makeString(int ch);
	};
}

using namespace XMLScanner;
using Token = std::string;

/** Function to Modify String So That Protected XML Files
  * i.e The One's Whose Tokens are Modified will be Reverted
  * to Original Value(s). 
  * More information related to this will be provided in "ReadMe.md"
  * File.
  *
  * Dependant Function(s) - N/A
  * 
  * Ver : 1.0
  *		- First Release.
  */
void Context::XMLProtectionMagicQuotes(std::string & str) {
	for (std::pair<std::string, std::string> pr : MagicQuotesR) {
		size_t start_pos = 0;
		while (str.find(pr.first, start_pos) != std::string::npos) {
			start_pos = str.find(pr.first, start_pos);
			str.replace(start_pos, pr.first.length(), pr.second);
			start_pos += pr.second.length();
		}
	}
}

/** Function to Set the Current Context.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void State::SetContext(Context * pContext) {
	_pContext = pContext;
}

/** Function to Attach File Stream to the State.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
void State::Attach(std::istream * pIn) {
	_pContext->_pState = _pContext->_pEatHeaders;
	_pContext->_pIn = pIn;
}

/** Function to Read Next Character From File Stream.
  * We are Also Updating prevChar & lineCount.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
bool State::CollectChar() {
	if (_pContext->_pIn->good())
	{
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
		return true;
	}
	return false;
}

/** Function to Convert Character into String.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
State::Token State::makeString(int ch)
{
	Token temp;
	return temp += ch;
}

/** Function to Transition Into Next State. Here
  * we Analyze the Current and/or Previous and/or
  * Next Character to Select the Next State. 
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 2.0
  *		- Added VERBOSE Mode Support.
  * Ver : 1.0
  *		- First Release.
  */
State* State::NextState()
{
	if (!(_pContext->_pIn->good()))
	{
		return nullptr;
	}
	int chNext = _pContext->_pIn->peek();
	if (chNext == EOF)
	{
		_pContext->_pIn->clear();
		/* if peek() reads end of file character, EOF, then eofbit is set and
		 * _pIn->good() will return false.  clear() restores state to good
		 */
	}
	
	/* The following tests must come first */
	if (_pContext->currChar == '<' && chNext == '?')
	{
		if(_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = EatHeaders";
		return _pContext->_pEatHeaders;
	}
	if (_pContext->currChar == '<' && chNext != '/')
	{
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = EatOpenTag";
		return _pContext->_pEatOpenTag;
	}
	if (_pContext->currChar == '<' && chNext == '/')
	{
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = EatCloseTag";
		return _pContext->_pEatCloseTag;
	}
	if (std::isspace(_pContext->currChar))
	{
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = EatWhiteSpace";
		return _pContext->_pEatWhitespace;
	}
	if (!std::isspace(_pContext->currChar) && _pContext->currChar != '>')
	{
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = EatValue";
		return _pContext->_pEatValue;
	}
	if (!_pContext->_pIn->good())
	{
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : STATE = No Match (Default to EatWhiteSpace)";
		return _pContext->_pEatWhitespace;
	}
	
	/* If _pContext->currChar is not a state transition char
	 * then send it to Eat Value State.
	 */
	return _pContext->_pEatWhitespace;
}

/** State Which will Perform the Functions when the File Stream
  * Characters are Whitespace(s).
  *
  * Dependant Class(es) or Structure(s) - State, Context
  *
  * Ver : 1.0
  *		- First Release.
  */
class EatWhitespace : public State {
public:
	EatWhitespace(Context * pContext) { _pContext = pContext; }
	
	/** Function to Act Immediately After Transitioning into this
	  * State.
	  * This Function Will Simply Keep on Reading Next Character
	  * Until a Non-Whitespace Character is Encountered.
	  *
	  * Dependant Function(s) - State::CollectChar
	  * 
	  * Ver : 1.0
	  *		- First Release.
	  */
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				return;
		} while (std::isspace(_pContext->currChar));
	}
};

/** State Which will Perform the Functions when the File Stream
  * Begins with XML Open Tag.
  *
  * Dependant Class(es) or Structure(s) - State, Context
  *
  * Ver : 2.0
  *		- Removed TagHelper Function. No Longer Needed.
  * Ver : 1.0
  *		- First Release.
  */
class EatOpenTag : public State {
public:
	EatOpenTag(Context * pContext) { _pContext = pContext; }
	
	/** Function to Act Immediately After Transitioning into this
	* State.
	* This Function Will Simply Keep on Reading Next Character
	* Until one of the specialChars Character is Encountered.
	*
	* Dependant Function(s) - State::CollectChar, EatOpenTag::SetNodeInfo
	*
	* Ver : 2.0
	*		- Not Using Nodes Anymore.
	*		- Scope Variable Changed. Now it Takes Node Pointer.
	*/
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				return;
			if(std::find(specialChars.begin(), specialChars.end(), _pContext->currChar) == specialChars.end())
				_pContext->token.push_back(_pContext->currChar);
		} while (_pContext->currChar != '>');

		if (!_pContext->token.empty()) {
			Node * newNode = new Node();
			SetNodeInfo(newNode);
			if (_pContext->_scope.empty())
				_pContext->xDoc->AddChild(newNode);
			else
				_pContext->xDoc->AddChild(newNode, _pContext->_scope.top());
			_pContext->_scope.push(newNode);
			if (_pContext->VERBOSE)
				std::cout << "\n [VERBOSE] : Pushed New Element \"" << _pContext->_scope.top()->tag << "\" onto Scope Stack";
		}
	}
private:
	/** Funtion to Set the Node Information to Node Pointer. This Method will
	  * Process Open Tag State Token to Get Tag Name and it's Associated Attributes
	  * and Adds them to "node" Variable.
	  *
	  * Dependant Function(s) - StringHelper::Split, Context::XMLProtectionMagicQuotes
	  *
	  * Ver : 2.0
	  *		- Added VERBOSE Mode Feature.
	  * Ver : 1.0
	  *		- First Release.
	  */
	void SetNodeInfo(Node * node) {
		std::vector<std::string> tokSplit = Utilities::StringHelper::Split(_pContext->token, ' ');
		/* MagicQuotes Protection */
		if (_pContext->MagicQuotes) {
			_pContext->XMLProtectionMagicQuotes(tokSplit[0]);
		}
		node->tag = tokSplit[0];
		for (size_t i = 1; i < tokSplit.size(); i++) {
			std::string attributeNM, attributeVal;
			int mode = 0;
			for (char ch : tokSplit[i]) {
				if (ch == '"') {
					mode++;
					continue;
				}

				if (mode == 0)
					attributeNM.push_back(ch);
				else  if (mode == 1)
					attributeVal.push_back(ch);
				else
					break;
			}
			attributeNM.pop_back();
			if (_pContext->MagicQuotes) {
				_pContext->XMLProtectionMagicQuotes(attributeNM);
				_pContext->XMLProtectionMagicQuotes(attributeVal);
			}
			node->attributes.push_back(std::make_pair(attributeNM, attributeVal));
		}
	}
};


/** State Which will Perform the Functions when the File Stream
  * Begins with XML Close Tag.
  *
  * Dependant Class(es) or Structure(s) - State, Context
  *
  * Ver : 2.0
  *		- Added voidChars Vector. This List will be Used Instead of
  *		  specialChars to Ignore Storing Characters Into Token Variable.
  *		- Modified EatChars Function.
  *	Ver : 1.0
  *		- First Release.
  */
class EatCloseTag : public State {
public:
	EatCloseTag(Context * pContext) { _pContext = pContext; }
	
	/** Function to Act Immediately After Transitioning into this
	  * State.
	  * This Function Will Simply Keep on Reading Next Character
	  * Until one of the voidChars Character is Encountered.
	  *
	  * Dependant Function(s) - State::CollectChar
	  *
	  * Ver : 2.0
	  *		- Not Using Nodes Anymore.
	  *		- Scope Variable Changed. Now it Takes Node Pointer.
	  *		- Uses voidChars now Instead of specialChars.
	  * Ver : 1.0
	  *		- First Release.
	  */
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (!CollectChar())
				return;
			if (std::find(voidChars.begin(), voidChars.end(), _pContext->currChar) == voidChars.end())
				_pContext->token.push_back(_pContext->currChar);
		} while (_pContext->currChar != '>');

		if (_pContext->MagicQuotes)
			_pContext->XMLProtectionMagicQuotes(_pContext->token);
		if (_pContext->token != _pContext->_scope.top()->tag)
			_pContext->isCorrupt = true;
		if (_pContext->VERBOSE)
			std::cout << "\n [VERBOSE] : Popping the Element \"" << _pContext->_scope.top()->tag << "\" from Scope Stack";
		if (!_pContext->_scope.empty())
			_pContext->_scope.pop();
	}
private:
	std::vector<char> voidChars = { '<', '>', '/' };
};


/** State Which will Perform the Functions when the File Stream
  * is the XML Value Field.
  *
  * Dependant Class(es) or Structure(s) - State, Context
  *
  * Ver : 2.0
  *		- Modified EatChars Function.
  * Ver : 1.0
  *		- First Release.
  */
class EatValue : public State {
public:
	EatValue(Context * pContext) { _pContext = pContext; }
	
	/** Function to Act Immediately After Transitioning into this
	* State.
	* This Function Will Simply Keep on Reading Next Character
	* Until one of the specialChars Character is Encountered.
	*
	* Dependant Function(s) - State::CollectChar, Context::XMLProtectionMagicQuotes
	*
	* Ver : 2.0
	*		- Not Using Nodes Anymore.
	*		- Scope Variable Changed. Now it Takes Node Pointer.
	* Ver : 1.0
	*		-First Release. 
	*/
	virtual void EatChars() {
		_pContext->token.clear();
		do {
			if (std::find(specialChars.begin(), specialChars.end(), _pContext->currChar) == specialChars.end())
				_pContext->token.push_back(_pContext->currChar);
			if (!CollectChar())
				return;
		} while (_pContext->currChar != '<' && (!std::isspace(_pContext->currChar) || _pContext->currChar == ' '));
		
		Utilities::StringHelper::Trim(_pContext->token);
		if (_pContext->MagicQuotes)
			_pContext->XMLProtectionMagicQuotes(_pContext->token);
		_pContext->_scope.top()->value = _pContext->token;
	}
};

/** State Which will Perform the Functions when the File Stream
  * is XML Headers.
  *
  * Dependant Class(es) or Structure(s) - State, Context
  *
  * Ver : 1.0
  *		- First Release.
  */
class EatHeaders : public State {
public:
	EatHeaders(Context * pContext) { _pContext = pContext; }
	
	/** Function to Act Immediately After Transitioning into this
	  * State.
	  * This Function Will Simply Keep on Reading Next Character
	  * Until one of the '>' Character is Encountered.
	  *
	  * Dependant Function(s) - State::CollectChar, EatHeaders::SetHeaders
	  *
	  * Ver : 1.0
	  *		- First Release.
	  */
	virtual void EatChars() {
		_pContext->token.clear();
		bool first = true;
		do {
			if (!CollectChar())
				return;
			if (first) {
				first = false;
				continue;
			}
			_pContext->token.push_back(_pContext->currChar);
		} while (_pContext->currChar != '>');
		_pContext->token.pop_back();
		SetHeaders();
	}
private:
	/** Function to Set XML Header Attributes (Version & Encoding)
	  * by Evaluating Token in EatHeaders State.
	  *
	  * Dependant Function(s) - EatHeaders::RemoveAllWhitespaces, StringHelper::FindSubStr
	  * 
	  * Ver : 1.0
	  *		- First Release.
	  */
	void SetHeaders() {
		std::string nospace = RemoveAllWnitespaces(_pContext->token);
		int pos = 0;
		pos = Utilities::StringHelper::FindSubStr(nospace, "version=\"");
		if (pos != -1) {
			pos += 9;
			std::string ver;
			for (size_t i = pos; nospace[i] != '"'; i++) {
				ver.push_back(nospace[i]);
			}
			_pContext->_version = ver;
			_pContext->xDoc->setVersion(ver);
		}

		pos = Utilities::StringHelper::FindSubStr(nospace, "encoding=\"");
		if (pos != -1) {
			pos += 10;
			std::string enc;
			for (size_t i = pos; nospace[i] != '"'; i++) {
				enc.push_back(nospace[i]);
			}
			_pContext->_encoding = enc;
			_pContext->xDoc->setEncoding(enc);
		}
	}

	/** Function to Remove All Whitespaces in a String.
	  *
	  * Dependant Function(s) - N/A
	  *
	  * Ver : 1.0
	  *		- First Release.
	  */
	std::string RemoveAllWnitespaces(std::string src) {
		std::string str;
		for (char ch : src) {
			if (std::isspace(ch))
				continue;
			str.push_back(ch);
		}
		return str;
	}
};

/** Default Constructor For Context Structure. This Function
  * will Initialize All the States, XML Document Elements, 
  * Scope, MagicQuotes Protection Enable/Disable and MagicQuotesR
  * Vector Initialization.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
Context::Context() {
	_pEatValue = new EatValue(this);
	_pEatHeaders = new EatHeaders(this);
	_pEatOpenTag = new EatOpenTag(this);
	_pEatCloseTag = new EatCloseTag(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pState = _pEatWhitespace;
	_lineCount = 0;	
	
	xDoc = new XMLDocument();
	_scope.push(xDoc->getRoot());

	MagicQuotes = false;

	/* XML Protection Mechanism While Reading From Disk */
	MagicQuotesR.insert(std::make_pair(";opBracket", "<"));
	MagicQuotesR.insert(std::make_pair(";clBracket", ">"));
	MagicQuotesR.insert(std::make_pair(";dbQuotes", "\""));
	MagicQuotesR.insert(std::make_pair(";bkSlash", "/"));
	MagicQuotesR.insert(std::make_pair(";nlWhiteSpace", "\n"));
	MagicQuotesR.insert(std::make_pair(";tbWhiteSpace", "\t"));
	MagicQuotesR.insert(std::make_pair(";eqSymbol", "\t"));
}

/** Default Destructor For Context Structure. This Function
  * will Delete All the Created States.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
Context::~Context() {
	delete _pEatValue;
	delete _pEatHeaders;
	delete _pEatOpenTag;
	delete _pEatCloseTag;
	delete _pEatWhitespace;
}

/** Default Constructor For State Class. It Does Nothing.
  * 
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
State::State() {};


/** Default Destructor For State Class. It Does Nothing.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
State::~State() {};


/** Constructor For Toker Class. This Function Initializes _pContext, 
  * _pConsumer, VERBOSE and VERBOSE Variable of _pContext too. Also
  * the Magic Quote Protection is Enabled or Disabled here.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 2.0
  *		- Modified the Interface.
  *		- Added Support for MagicQuote Protection & VERBOSE Mode
  * Ver : 1.0
  *		- First Release.
  */
Toker::Toker(bool verbose, bool MagicQuotes) {
	_pContext = new Context();
	_pConsumer = _pContext->_pEatWhitespace;
	_pConsumer->SetContext(_pContext);

	VERBOSE = verbose;
	_pContext->VERBOSE = verbose;
	_pContext->MagicQuotes = MagicQuotes;
	_pContext->xDoc->SetVerbose(VERBOSE);
	if (VERBOSE) {
		std::cout << "\n [VERBOSE] : Verbose Mode Turned On for Toker";
		std::cout << "\n [VERBOSE] : Verbose Mode Turned On for Context";
		if (MagicQuotes)
			std::cout << "\n [VERBOSE] : Magic Quotes Protection (Read) = Enabled";
		else
			std::cout << "\n [VERBOSE] : Magic Quotes Protection (Read) = Disabled";
		std::cout << "\n";
	}
}

/** Default Destructor for Toker Class. This Function Destroys the _pContext
  * Variable and Set's it's to Null Pointer to Prevent Memory Leak.
  *
  * Dependant Function(s) - N/A
  *
  * Ver : 1.0
  *		- First Release.
  */
Toker::~Toker() {
	delete _pContext;
	_pContext = nullptr;
}

/** Function to Attach File Stream to Toker. Returns True if Valid File Exists
  * Else It'll Return False.
  *
  * Dependant Function(s) - State::Attach
  *
  * Ver : 1.0
  *		- First Release.
  */
bool Toker::Attach(std::istream * pIn) {
	if (pIn != nullptr && pIn->good()) {
		_pConsumer->Attach(pIn);
		return true;
	}
	return false;
}

/** Function to Return Token From _pConsumer. Will return String.
  * 
  * Dependant Function(s) - State::CanRead, State::ConsumeChars, 
  *							State::HasTok, State::GetTok
  *
  * Ver : 1.0
  *		- First Release.
  */
std::string Toker::GetTok() {
	while (true) {
		if (!_pConsumer->CanRead())
			return  "";
		_pConsumer->ConsumeChars();
		if (_pConsumer->HasTok())
			break;
	}
	return _pConsumer->GetTok();
}

/** Function to Check if We Can Read From XML File. Returns True
  * if we can Read Data else it'll Return False.
  *
  * Dependant Function(s) - State::CanRead
  *
  * Ver : 1.0
  *		- First Release.
  */
bool Toker::CanRead() {
	return _pConsumer->CanRead();
}

/** Function to Return The Number of Lines Read From the Input File
  * Stream.
  *
  * Dependant Function(s) - State::CurrentLineCount
  *
  * Ver : 1.0
  *		- First Release.
  */
size_t Toker::CurrentLineCount() {
	return _pConsumer->CurrentLineCount();
}

/** Function to Read XML File Token By Token And Generate a XML Document.
  * 
  * Dependant Function(s) - Toker::Attach, Toker::GetTok,
  *							Toker::CurrentLineCount
  *
  * Ver : 2.0
  *		- Added Support for VERBOSE Mode.
  * Ver : 1.0
  *		- First Release.
  */
XMLDocument * Toker::Compute(std::string fileSpec) {
	std::ifstream inFile(fileSpec);
	if (!inFile.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n ";
		return nullptr;
	}

	Attach(&inFile);
	do
	{
		std::string tok = GetTok();
		if (VERBOSE) {
			if (tok == "\n")
				tok = "newline";
			std::cout << "\n -- Token  : " << tok << "\n";
		}
	} while (inFile.good());
	if (VERBOSE)
		std::cout << "\n Line Count : " << CurrentLineCount();
	return _pContext->xDoc;
}


#ifdef TEST_TOKENIZER

#include <exception>

using namespace Utilities;
/** Function to Test Toker and Associated Classes.
  * 
  * Dependant Function(s) - StringHelper::Title, Toker::Toker, 
  *							Toker::Compute, XMLDocument::ShowXMLTree
  *
  * Ver : 2.0
  *		- Updated toker to be a Pointer. This Change was made since Toker
  *		  Constructor has been Updated to Account For MagicQuote Protection
  *		  and Verbose Mode.
  * Ver : 1.0
  *		- First Release.
  */
int main() {
	StringHelper::Title("Testing Tokenizer", '='); 
	std::string fileSpec = "testXMLFileProcessor.xml";
	
	StringHelper::Title("Declaring Toker");
	Toker * toker = new Toker(false, true);
	std::cout << "\n - Complete !\n";
	
	StringHelper::Title("Declaring XML Document Object");
	std::cout << "\n - Complete !\n";
	XMLDocument * xDoc;
	
	StringHelper::Title(std::string("Processing File : \"" + fileSpec + "\" using Toker"));
	xDoc = toker->Compute(fileSpec);
	std::cout << "\n File Processing Complete !";
	std::cout << "\n";
	
	StringHelper::Title("Checking whether XML Document is generated or Not");
	if (xDoc == nullptr) {
		std::cout << "\n XML Document Not Generated";
		std::cout << "\n\n\n ";
		return 0;
	}
	else {
		std::cout << "\n XML Document Successfully Generated";
	}
	std::cout << "\n";

	StringHelper::Title("Display XML Document in Tree Structure");
	xDoc->ShowXMLTree(true);
	std::cout << "\n\n\n ";

	return 0;
}
#endif // TEST_TOKENIZER