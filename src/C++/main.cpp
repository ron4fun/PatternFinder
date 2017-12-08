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

#include <iostream>
#include "PatternFinder.h"

using namespace std;

int main()
{
    PatternTByteArray pattern = Pattern::Transform("456?89?B");
    unsigned char data1[] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
    unsigned char data2[] = {0x01, 0x23, 0x45, 0x66, 0x89, 0x6B, 0xCD, 0xEF};
    unsigned char data3[] = {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11};

    int64_t FoundOffset;
    unsigned char *ptr = &data1[0];
    bool found = Pattern::Find(ptr, 8, pattern, FoundOffset);

    if (found && FoundOffset == 2);
    else
        cout << "Test 1 failed..." << endl;

    ptr = &data2[0];
    found = Pattern::Find(ptr, 8, pattern, FoundOffset);

    if (found && FoundOffset == 2);
    else
        cout << "Test 2 failed..." << endl;

    ptr = &data3[0];
    found = Pattern::Find(ptr, 8, pattern, FoundOffset);

    if (!found);
    else
        cout << "Test 3 failed..." << endl;

    cout << "Done testing!" << endl;

    // Test Phase 2
    Signature sig1 = Signature("pattern1", "456?89?B");
    Signature sig2 = Signature("pattern2", "1111111111");
    Signature sig3 = Signature("pattern3", "AB??EF");
    Signature sig4 = Signature("pattern4", "45??67");
    SignatureArray signatures = {sig1, sig2, sig3, sig4};

    string data = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x45, 0x65, 0x67, 0x89};
    cout << sig1 << endl;
    SignatureArray result = SignatureFinder::Scan(data, signatures);

    for (unsigned int i = 0; i < result.size(); i++)
        cout << "Found " << result[i] << " at " << result[i].GetFoundOffset() << endl;

    return 0;
}
