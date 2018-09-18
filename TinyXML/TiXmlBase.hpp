#pragma once
#include <stdio.h>
#include <assert.h>
#include <string>
#include "TiXmlError.hpp"
#include "TiXmlEntity.hpp"
#include "TiXmlCursor.hpp"
#include "TiXmlEncoding.hpp"

/** TiXmlBase is a base class for every class in TinyXml.
It does little except to establish that TinyXml classes
can be printed and provide some utility functions.

In XML, the document and elements can contain
other elements and other types of nodes.

@verbatim
A Document can contain:	Element	(container or leaf)
Comment (leaf)
Unknown (leaf)
Declaration( leaf )

An Element can contain:	Element (container or leaf)
Text	(leaf)
Attributes (not on tree)
Comment (leaf)
Unknown (leaf)

A Decleration contains: Attributes (not on tree)
@endverbatim
*/

class TiXmlParsingData;

class TiXmlBase
{
	friend class TiXmlNode;
	friend class TiXmlElement;
	friend class TiXmlDocument;

public:
	TiXmlBase() : userData(0) {}
	virtual ~TiXmlBase() {}

	/**	All TinyXml classes can print themselves to a filestream
	or the string class (TiXmlString in non-STL mode, std::string
	in STL mode.) Either or both cfile and str can be null.

	This is a formatted print, and will insert
	tabs and newlines.

	(For an unformatted stream, use the << operator.)
	*/
	virtual void Print(FILE* cfile, int depth) const = 0;

	/**	The world does not agree on whether white space should be kept or
	not. In order to make everyone happy, these global, static functions
	are provided to set whether or not TinyXml will condense all white space
	into a single space or not. The default is to condense. Note changing this
	value is not thread safe.
	*/
	static void SetCondenseWhiteSpace(bool condense) { condenseWhiteSpace = condense; }

	/// Return the current white space setting.
	static bool IsWhiteSpaceCondensed() { return condenseWhiteSpace; }

	/** Return the position, in the original source file, of this node or attribute.
	The row and column are 1-based. (That is the first row and first column is
	1,1). If the returns values are 0 or less, then the parser does not have
	a row and column value.

	Generally, the row and column value will be set when the TiXmlDocument::Load(),
	TiXmlDocument::LoadFile(), or any TiXmlNode::Parse() is called. It will NOT be set
	when the DOM was created from operator>>.

	The values reflect the initial load. Once the DOM is modified programmatically
	(by adding or changing nodes and attributes) the new values will NOT update to
	reflect changes in the document.

	There is a minor performance cost to computing the row and column. Computation
	can be disabled if TiXmlDocument::SetTabSize() is called with 0 as the value.

	@sa TiXmlDocument::SetTabSize()
	*/
	int Row() const { return location.row + 1; }
	int Column() const { return location.col + 1; }	///< See Row()

	void  SetUserData(void* user) { userData = user; }	///< Set a pointer to arbitrary user data.
	void* GetUserData() { return userData; }	///< Get a pointer to arbitrary user data.
	const void* GetUserData() const { return userData; }	///< Get a pointer to arbitrary user data.

															// Table that returs, for a given lead byte, the total number of bytes
															// in the UTF-8 sequence.
	static const int utf8ByteTable[256];

	virtual const char* Parse(const char* p,
		TiXmlParsingData* data,
		TiXmlEncoding encoding /*= TIXML_ENCODING_UNKNOWN */) = 0;

	/** Expands entities in a string. Note this should not contian the tag's '<', '>', etc,
	or they will be transformed into entities!
	*/
	static void EncodeString(const std::string& str, std::string* out);
protected:

	static const char* SkipWhiteSpace(const char*, TiXmlEncoding encoding);

	inline static bool IsWhiteSpace(char c)
	{
		return (isspace((unsigned char)c) || c == '\n' || c == '\r');
	}
	inline static bool IsWhiteSpace(int c)
	{
		if (c < 256)
			return IsWhiteSpace((char)c);
		return false;	// Again, only truly correct for English/Latin...but usually works.
	}

	static bool	StreamWhiteSpace(std::istream * in, std::string * tag);
	static bool StreamTo(std::istream * in, int character, std::string * tag);

	/*	Reads an XML name into the string provided. Returns
	a pointer just past the last character of the name,
	or 0 if the function has an error.
	*/
	static const char* ReadName(const char* p, std::string* name, TiXmlEncoding encoding);

	/*	Reads text. Returns a pointer past the given end tag.
	Wickedly complex options, but it keeps the (sensitive) code in one place.
	*/
	static const char* ReadText(const char* in,				// where to start
		std::string* text,			// the string read
		bool ignoreWhiteSpace,		// whether to keep the white space
		const char* endTag,			// what ends this text
		bool ignoreCase,			// whether to ignore case in the end tag
		TiXmlEncoding encoding);	// the current encoding

									// If an entity has been found, transform it into a character.
	static const char* GetEntity(const char* in, char* value, int* length, TiXmlEncoding encoding);

	// Get a character, while interpreting entities.
	// The length can be from 0 to 4 bytes.
	inline static const char* GetChar(const char* p, char* _value, int* length, TiXmlEncoding encoding)
	{
		assert(p);
		if (encoding == TIXML_ENCODING_UTF8)
		{
			*length = utf8ByteTable[*((const unsigned char*)p)];
			assert(*length >= 0 && *length < 5);
		}
		else
		{
			*length = 1;
		}

		if (*length == 1)
		{
			if (*p == '&')
				return GetEntity(p, _value, length, encoding);
			*_value = *p;
			return p + 1;
		}
		else if (*length)
		{
			//strncpy( _value, p, *length );	// lots of compilers don't like this function (unsafe),
			// and the null terminator isn't needed
			for (int i = 0; p[i] && i < *length; ++i) {
				_value[i] = p[i];
			}
			return p + (*length);
		}
		else
		{
			// Not valid text.
			return 0;
		}
	}

	// Return true if the next characters in the stream are any of the endTag sequences.
	// Ignore case only works for english, and should only be relied on when comparing
	// to English words: StringEqual( p, "version", true ) is fine.
	static bool StringEqual(const char* p, const char* endTag, bool ignoreCase, TiXmlEncoding encoding);

	static const char* errorString[TiXmlError::TIXML_ERROR_STRING_COUNT];

	TiXmlCursor location;

	/// Field containing a generic user pointer
	void*			userData;

	// None of these methods are reliable for any language except English.
	// Good for approximation, not great for accuracy.
	static int IsAlpha(unsigned char anyByte, TiXmlEncoding encoding);
	static int IsAlphaNum(unsigned char anyByte, TiXmlEncoding encoding);
	inline static int ToLower(int v, TiXmlEncoding encoding)
	{
		if (encoding == TIXML_ENCODING_UTF8)
		{
			if (v < 128) return tolower(v);
			return v;
		}
		else
		{
			return tolower(v);
		}
	}
	static void ConvertUTF32ToUTF8(unsigned long input, char* output, int* length);

private:
	TiXmlBase(const TiXmlBase&);				// not implemented.
	void operator=(const TiXmlBase& base);	// not allowed.

	static const int NUM_ENTITY = 5;
	static const int MAX_ENTITY_LENGTH = 6;
	static TiXmlEntity entity[NUM_ENTITY];
	static bool condenseWhiteSpace;
};
