#!/usr/bin/env python

import json
from sys import argv,stderr

canonical = json.loads(open(argv[1],"r").read())
test = json.loads(open(argv[2],"r").read())

data_good = canonical['data']
data_test = test['data']


def compare(dict1, dict2):
    for key in dict1:
        if not key in dict2:
            return False
        if dict1[key] != dict2[key]:
            return False
    return True
for (good, test) in zip(data_good, data_test):
    if not compare(good, test):
        stderr.write("Test data is missing data found in canonical set.\n")
        exit(1)

    if not compare(test, good):
        stderr.write("Test data is extra data not found in canonical set.\n")
        exit(1)
