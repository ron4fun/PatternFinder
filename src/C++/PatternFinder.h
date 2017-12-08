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

#ifndef PATTERNFINDER_H
#define PATTERNFINDER_H

#include <string>
#include <vector>
#include <sstream>

using namespace std;

struct Nibble
{
    int Data;
    bool Wildcard;
};

struct TByte
{
    Nibble N1;
    Nibble N2;
};

typedef vector<TByte> PatternTByteArray;

class Pattern
{
    public:
        static string Format(const string &data);
        static PatternTByteArray Transform(const string &data);

        static bool Find(const unsigned char *data, const unsigned int length, const PatternTByteArray &_pattern, int64_t &offsetFound);
        static bool Find(const unsigned char *data, const unsigned int length, const PatternTByteArray &_pattern)
        {
            int64_t FoundOffset;
            return Find(data, length, _pattern, FoundOffset);
        } // end function Find
        static bool Find(const string &data, const PatternTByteArray &_pattern, int64_t &offsetFound)
        {
            return Find((const unsigned char *)data.c_str(), data.length(), _pattern, offsetFound);
        }
        static bool Find(const string &data, const PatternTByteArray &_pattern)
        {
            int64_t FoundOffset;
            return Find(data, _pattern, FoundOffset);
        } // end function Find

    private:
        static int hexChToInt(const unsigned char ch)
        {
            if (ch >= '0' && ch <= '9')
                return ch - '0';
            else if (ch >= 'A' && ch <= 'F')
                return ch - 'A' + 10;
            else if (ch >= 'a' && ch <= 'f')
                return ch - 'a' + 10;
            return -1;
        } // end function hexChToInt

        static bool matchByte(const unsigned char b, const TByte &p)
        {
             int N1, N2;

             if (!p.N1.Wildcard) // if not a wildcard we need to compare the data.
             {
                 N1 = b >> 4;
                 if (N1 != p.N1.Data) // if the data is not equal b doesn't match p.
                    return false;
             } // end if

             if(!p.N2.Wildcard) // if not a wildcard we need to compare the data.
             {
                 N2 = b & 0xF;
                 if (N2 != p.N2.Data) // if the data is not equal b doesn't match p.
                    return false;
             } // end if

             return true;
        } // end function matchByte

}; // end class PatternFinder

class Signature
{
    friend ostream &operator<<(ostream &output, const Signature &sig)
    {
        output << sig.GetName();
        return output; // enables cout << a << b << c;
    } // end function operator<<

    friend class SignatureFinder;

public:
    Signature(const string &_name, const string &_pattern)
    : name(_name), pattern(Pattern::Transform(_pattern))
    {} // end cctr
    Signature(const string &_name, const PatternTByteArray &_pattern)
    : name(_name), pattern(_pattern)
    {} // end cctr

    string GetName(void) const { return name; }

    PatternTByteArray GetPattern(void) const { return pattern; }

    int GetFoundOffset(void){ return foundOffset; }

private:
    string name;
    PatternTByteArray pattern;
    int64_t foundOffset;

}; // end class Signature

typedef vector<Signature> SignatureArray;

class SignatureFinder
{
public:
    static SignatureArray Scan(const string &Data, const SignatureArray signature)
    {
        return Scan((const unsigned char *)Data.c_str(), Data.length(), signature);
    } // end function Scan
    static SignatureArray Scan(const string &Data, const Signature signatures)
    {
        return Scan((const unsigned char *)Data.c_str(), Data.length(), signatures);
    } // end function Scan
    static SignatureArray Scan(const unsigned char *Data, const unsigned int length, const Signature signature)
    {
        SignatureArray sigArray = {signature};
        return Scan(Data, length, sigArray);
    } // end function Scan
    static SignatureArray Scan(const unsigned char *Data, const unsigned int length, const SignatureArray signatures)
    {
        int64_t tempOffset;
        bool isTrue;
        SignatureArray found;
        SignatureArray sigs(signatures);

        for ( unsigned int Idx = 0; Idx < sigs.size(); Idx++)
        {
            isTrue = Pattern::Find(Data, length, sigs[Idx].GetPattern(), tempOffset);
            if (isTrue)
            {
                sigs[Idx].foundOffset = tempOffset;
                found.push_back(sigs[Idx]);
            } // end if
        } // end for

        return found;
    } // end function Scan

}; // end class SignatureFinder

#endif // PATTERNFINDER_H
