/*
www.sourceforge.net/projects/tinyxml
Original code by Lee Thomason (www.grinninglizard.com)

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#pragma once
#include "TiXmlCursor.hpp"
#include "TiXmlEncoding.hpp"
#include "TiXmlQueryType.hpp"
#include "TiXmlEntity.hpp"
#include "TiXmlNodeType.hpp"
#include "TiXmlError.hpp"

#include "TiXmlVisitor.hpp"
#include "TiXmlBase.hpp"
#include "TiXmlNode.hpp"
#include "TiXmlAttribute.hpp"
#include "TiXmlAttributeSet.hpp"
#include "TiXmlElement.hpp"
#include "TiXmlComment.hpp"
#include "TiXmlText.hpp"
#include "TiXmlDeclaration.hpp"
#include "TiXmlUnknown.hpp"
#include "TiXmlDocument.hpp"
#include "TiXmlHandle.hpp"
#include "TiXmlPrinter.hpp"


#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <string>
#include <iostream>
#include <sstream>
