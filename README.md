PbNotify + Extras From HackMIT 2013
=======

PbNotify is a Remote Pebble Notification API powered by Flask and MongoDB that can easily be set up to provide a public-facing bridge between the Internet at large and local notifications on your Pebble smartwatch. It is accompanied by a watchface on the Pebble that communicates with your smartphone via httpebble. This is ideal for many situations, as essentially any web app or server that could make a single API call can send notifications straight to your wrist. This was inspired by the need to be notified in a convenient manner when long-running commands on a remote server completed. Not only does PbNotify allow such notifications, but it also allows for a plethora of information to be sent along with the simple ping, such as the last command's exit status.

Also found here is a quick API explorer that was hacked together for an entirely different project I had in mind.

Simply start up the server, entering in your own secret key and database information, or try the demo at http://yasyf.scripts.mit.edu:8080/. After creating an account/logging in, you will be given a userid which can then be used with the following calls.

###create new notification
####/notification/create/
#####{endpoint}/api/notification/create/&lt;userid&gt;/&lt;source&gt;/&lt;text&gt;
	userid: your generated userid
	source: the originating entity of your notification
	text: the text of your notification
	
	returns: json-encoded notificationid
	
	$ curl "http://yasyf.scripts.mit.edu:8080/api/notification/create/5250a011dabae068d13ee5f4/Test/HelloWorld"
	{"notificationid": "525119b8796af044768ee0db"}
	$ 

###retrieve most recent notification
####/notification/get/
#####{endpoint}/api/notification/get/&lt;userid&gt;
	userid: your generated userid
	
	returns: httpebble-friendly json-encoded notification (source and text)
	
	$ curl "http://yasyf.scripts.mit.edu:8080/api/notification/get/5250a011dabae068d13ee5f4/525119b8796af044768ee0db"
	{"1": "Test", "2": "HelloWorld"}
	$ 

###retrieve notification
####/notification/get/
#####{endpoint}/api/notification/get/&lt;userid&gt;/&lt;notificationid&gt;
	userid: your generated userid
	notificationid: the notificationid returned from a previous `/notification/create/` call

	returns: httpebble-friendly json-encoded notification (source and text)
	
	$ curl "http://yasyf.scripts.mit.edu:8080/api/notification/get/5250a011dabae068d13ee5f4"
	{"1": "Test", "2": "HelloWorld"}
	$


###mark most recent notification as delivered
####/notification/delivered/
#####{endpoint}/api/notification/delivered/&lt;userid&gt;
	userid: your generated userid

	returns: json-encoded success or failure
	
	$ curl "http://yasyf.scripts.mit.edu:8080/api/notification/delivered/5250a011dabae068d13ee5f4"
	{"status": "success"}
	$


