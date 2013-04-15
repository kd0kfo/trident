#!/usr/bin/env python

import json
j = json.loads(open("test.json","r").read())

print(j['data'])

