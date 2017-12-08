# -*- coding: utf-8 -*-

"""
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
    
    3. This notice may not be removed or altered from any source distribution.
"""

import ctypes
from copy import deepcopy
   
class Nibble(ctypes.Structure):
    """
    Class record emulates the behaviour of C++ structure.
    """
    _fields_ = [("Wildcard", ctypes.c_bool),
                ("Data", ctypes.c_byte)]

class TByte(ctypes.Structure):
    """
    Class record emulates the behaviour of C++ structure.
    """
    _fields_ = [("N1", type(Nibble())),
                ("N2", type(Nibble()))]
    
class Pattern(object):

    @staticmethod
    def Format(_pattern):
        """
        Arg: String
        Returns: String
        """
        tempRes = ""

        for ch in _pattern:
            if (ch >= '0' and ch <= '9') or (ch >= 'A' and ch <= 'F') or (ch >= 'a' and ch <= 'f') or ch == '?':
                tempRes = tempRes + ch

        return tempRes

    @staticmethod
    def Transform(_pattern):
        """
        Arg: String
        Returns: List of TByte
        """
        if not isinstance(_pattern, str):
            raise TypeError("_pattern should be string.")
        
        _pattern = Pattern.Format(_pattern)
        _length = len(_pattern)

        if _length == 0:
            return None

        tempRes = [] 

        if _length % 2 != 0:
          _pattern = _pattern + '?'
          _length += 1

        newbyte = TByte()
        i = 0
        j = 0
     
        for ch in _pattern:
            if ch == '?': # wildcard
                if j == 0:
                    newbyte.N1.Wildcard = True
                else:
                    newbyte.N2.Wildcard = True
            else: # hex
                if j == 0:
                    newbyte.N1.Wildcard = False
                    newbyte.N1.Data = ctypes.c_byte(Pattern.__hexChToInt(ch) & 0xF)
                else:
                    newbyte.N2.Wildcard = False
                    newbyte.N2.Data = ctypes.c_byte(Pattern.__hexChToInt(ch) & 0xF)
            
            j += 1
            if j == 2:
                j = 0
                tempRes.append(deepcopy(newbyte))

        return tempRes
    
    @staticmethod
    def Find(Data, _pattern):
        """
        Arg: bytearray, PatternByteArray
        Returns: Boolean, offsetFound
        """
        if isinstance(Data, str):
            Data = bytearray(Data)
        elif not isinstance(Data, bytearray):
            raise TypeError("Data should be a bytearray or a string.")
        
        offsetFound = -1
        
        if not Data or not _pattern:
            return False, offsetFound
       
        patternSize = len(_pattern)
        if len(Data) == 0 or patternSize == 0:
            return False, offsetFound

        i = 0
        pos = 0
        
        while i < len(Data):
            if Pattern.__matchByte(Data[i], _pattern[pos]):
                # check if the current data byte matches the current pattern byte
                pos += 1
                if pos == patternSize: # everything matched
                    offsetFound = i - patternSize + 1
                    return True, offsetFound
            else: # fix by Computer_Angel
                i = i - pos
                pos = 0 # reset current pattern position

            i += 1

        return False, offsetFound

    @staticmethod
    def __hexChToInt(ch):
        """
        Arg: Char
        Returns: Integer
        """
        if ch >= '0' and ch <= '9':
            return ord(ch) - ord('0')
        elif ch >= 'A' and ch <= 'F':
            return ord(ch) - ord('A') + 10
        elif ch >= 'a' and ch <= 'f':
            return ord(ch) - ord('a') + 10
        return -1
    
    @staticmethod
    def __matchByte(b, p):
        """
        Arg: Byte, TByte
        Returns: Boolean
        """
        N1, N2 = None, None
        if not p.N1.Wildcard: # if not a wildcard we need to compare the data.
            N1 = b >> 4
            if N1 != p.N1.Data: # if the data is not equal b doesn't match p.
                return False

        if not p.N2.Wildcard: # if not a wildcard we need to compare the data.
            N2 = b & 0xF
            if N2 != p.N2.Data: # if the data is not equal b doesn't match p.
                return False

        return True


class Signature(object):
    def __init__(self, _name, _pattern):
        """
        Arg: String, List of TByte OR String
        """
        if not isinstance(_name, str):
            raise TypeError("_name should be string.")
        
        self.__Name = _name
        
        if isinstance(_pattern, str):
            self.__Pattern = Pattern.Transform(_pattern)
        elif isinstance(_pattern, TByte):
            self.__Pattern = _pattern
        else:
            raise ValueError
        
        self.__FoundOffset = -1

    def __GetName(self):
        return self.__Name
    Name = property(fget= __GetName)
    
    def __GetPattern(self):
        return self.__Pattern
    Pattern = property(fget= __GetPattern)
      
    def __GetFoundOffset(self):
        return self.__FoundOffset
    def __SetFoundOffset(self, value):
        self.__FoundOffset = value
    FoundOffset = property(fget= __GetFoundOffset, fset= __SetFoundOffset)
    
    def __str__(self):
        return self.Name
    def __repr__(self):
        return self.Name
    
class SignatureFinder(object):
    @staticmethod
    def Scan(Data, signatures):
        """
        Args: TByte, List of Signature
        Returns: List of Signature
        """
        if isinstance(Data, str):
            Data = bytearray(Data)
        elif not isinstance(Data, bytearray):
            raise TypeError("Data should be a bytearray or a string.")
        
        if not isinstance(signatures, (list, tuple)):
            signatures = [signatures]
                    
        Idx = 0
        found = []

        for Idx in range(len(signatures)):
            isTrue, tempOffset = Pattern.Find(Data, signatures[Idx].Pattern)
            if isTrue:
                signatures[Idx].FoundOffset = tempOffset
                found.append(signatures[Idx])
                
        return found

if __name__ == '__main__':
    pass
