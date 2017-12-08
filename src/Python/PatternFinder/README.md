#PatternFinder#


A Python implementation of pattern finder in binary files and byte arrays. Tested on Python 2.7 and worked well. Run the unitest in the test folder for more tests.



Example
---------



    
    >>> from PatternFinder import *
    >>> # Create a pattern instance
	>>> pattern = Pattern.Transform('456?89?B')
	>>> # Have your data in a bytearray
	>>> data = bytearray("\x01\x23\x45\x67\x89\xAB\xCD\xEF")
	>>> found, FoundOffset = Pattern.Find(data, pattern)
	>>> found
	True
	>>> FoundOffset
	2
	>>> # Create a pattern signature
	>>> sig1 = Signature('pattern1', 'AB??EF')
	>>> sig1
	pattern1
	>>> sig2 = Signature('pattern2', '1111111111')
	>>> sig2
	pattern2
	>>> signatures = [sig1, sig2]
	>>> data = bytearray("\x01\x23\x45\x67\x89\xAB\xCD\xEF\x45\x65\x67\x89")
	>>> result = SignatureFinder.Scan(data, signatures)
	>>> for sigs in result:
            print "Found %s at %d" % (sigs.Name, sigs.FoundOffset)
	
	Found pattern1 at 5
	>>> 

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
        
        