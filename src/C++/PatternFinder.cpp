/*
License
----------
    Copyright (c) 2016 Mbadiwe Nnaemeka Ronald ron2tele@gmail.com

    This software is provided 'as-is', without any express or implied
    warranty. In no event will the author be held liable for any damages
    arising from the use of this software.
    Permission is granted to anyone to use this software for any purpose,
    including commercial applications, and to alter it and redistribute it
    freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not
    claim that you wrote the original software. If you use this software
    in a product, an acknowledgment in the product documentation must be
    specified.

    2. Altered source versions must be plainly marked as such, and must not be
    misrepresented as being the original software.

    3. This notice must not be removed or altered from any source distribution.
*/

#include "PatternFinder.h"

string Pattern::Format(const string &str)
{
    string tempRes;

    for (unsigned int i = 0; i < str.length(); i++)
    {
        char ch = str[i];
        if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F') || (ch >= 'a' && ch <= 'f') || (ch == '?'))
            tempRes = tempRes + ch;
    } // end for

    return tempRes;
} // end function Format

PatternTByteArray Pattern::Transform(const string &str)
{
    string _pattern = Format(str);
    unsigned int _length = _pattern.length();

    if (_length == 0)
        return PatternTByteArray();

    PatternTByteArray tempRes;

    if (_length % 2 != 0)
    {
        _pattern = _pattern + '?';
        _length++;
    } // end if

    TByte newbyte;
    unsigned int j = 0;

    for (unsigned int idx = 0; idx < _length; idx++ )
    {
        char ch = _pattern[idx];

        if (ch == '?') // wildcard
        {
            if (j == 0)
                newbyte.N1.Wildcard = true;
            else
                newbyte.N2.Wildcard = true;
        } // end if
        else // hex
        {
            if (j == 0)
            {
                newbyte.N1.Wildcard = false;
                newbyte.N1.Data = (hexChToInt(ch) & 0xF);
            } // end if
            else
            {
                newbyte.N2.Wildcard = false;
                newbyte.N2.Data = (hexChToInt(ch) & 0xF);
            } // end else
        } // end else


        j++;
        if (j == 2)
        {
            j = 0;
            tempRes.push_back(newbyte);
        } // end if
    } // end for

    return tempRes;
} // end function Format



bool Pattern::Find(const unsigned char *Data, const unsigned int length, const PatternTByteArray &_pattern, int64_t &offsetFound)
{
    offsetFound = -1;

    if (Data == NULL || _pattern.empty())
        return false;

    unsigned int patternSize = _pattern.size();
    if (length == 0 || patternSize == 0)
        return false;

    unsigned int i = 0, pos = 0;
    while (i < length)
    {
        if (matchByte(Data[i], _pattern[pos]))
        {
            // check if the current data byte matches the current pattern byte
            pos++;
            if (pos == patternSize) // everything matched
            {
                offsetFound = i - patternSize + 1;
                return true;
            } // end if
        } // end if
        else // fix by Computer_Angel
        {
            i -= pos;
            pos = 0; // reset current pattern position
        } // end else

        i++;
    } // end while

    return false;
} // end function Find
