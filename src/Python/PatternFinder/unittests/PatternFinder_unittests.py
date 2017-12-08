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

    3. This notice must not be removed or altered from any source distribution.
"""

import unittest
from PatternFinder import *

class PatternFinderTest(unittest.TestCase):

    def test_Tests(self):
        pattern = Pattern.Transform('456?89?B')
        data1 = bytearray("\x01\x23\x45\x67\x89\xAB\xCD\xEF")
        data2 = bytearray("\x01\x23\x45\x66\x89\x6B\xCD\xEF")
        data3 = bytearray("\x11\x11\x11\x11\x11\x11\x11\x11")

        found, FoundOffset = Pattern.Find(data1, pattern)
        self.assertTrue(found and (FoundOffset == 2), msg="Test 1 failed...")

        found, FoundOffset = Pattern.Find(data2, pattern)
        self.assertTrue(found and (FoundOffset == 2), msg="Test 2 failed...")

        found, FoundOffset = Pattern.Find(data3, pattern)
        self.assertFalse(found, msg="Test 3 failed...")

        print "Done testing!"
        
    def test_SignatureTest(self):
        sig1 = Signature('pattern1', '456?89?B')
        sig2 = Signature('pattern2', '1111111111')
        sig3 = Signature('pattern3', 'AB??EF')
        sig4 = Signature('pattern4', '45??67')
        signatures = [sig1, sig2, sig3, sig4]
        
        data = bytearray("\x01\x23\x45\x67\x89\xAB\xCD\xEF\x45\x65\x67\x89")
        
        result = SignatureFinder.Scan(data, signatures)

        for sigs in result:
            print "Found %s at %d" % (sigs.Name, sigs.FoundOffset)
            
if __name__ ==  '__main__':
    unittest.main()
