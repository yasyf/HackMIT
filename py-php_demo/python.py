#!/usr/bin/env python
import cgi,cgitb,urllib,json

data = {"data": "24 Tomatoes"}

print "Location: http://hackmit.yasyf.scripts.mit.edu/Yasyf/php.php?data=%s" % (urllib.quote(json.dumps(data))) 
print