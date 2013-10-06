PbNotify + Extras From HackMIT 2013
=======

PbNotify is a Remote Pebble Notification API powered by Flask that can easily be set up to provide a public-facing bridge between the Internet at large and local notifications on your Pebble smartwatch. Also found here is a quick API explorer that was hacked together for an entirely different project I had in mind.

Simply start up the server, entering in your own secret key and database information, or try the demo at http://yasyf.scripts.mit.edu:8080/. After creating an account/logging in, you will be given a userid which can then be used with the following calls.

###create new notification
####/notification/create/
#####{endpoint}/api/notification/create/<userid>/<source>/<text>
	userid: your generated userid
	source: the originating entity of your notification
	text: the text of your notification
	
	returns: json-encoded notificationid

###retrieve most recent notification
####/notification/get/
#####{endpoint}/api/notification/get/<userid>
	userid: your generated userid
	
	returns: httpebble-friendly json-encoded notification (source and text)

###retrieve notification
####/notification/get/
#####{endpoint}/api/notification/get/<userid>/<notificationid>
	userid: your generated userid
	notificationid: the notificationid returned from a previous `/notification/create/` call

	returns: httpebble-friendly json-encoded notification (source and text)


###mark most recent notification as delivered
####/notification/delivered/
#####{endpoint}/api/notification/delivered/<userid>
	userid: your generated userid

	returns: json-encoded success or failure


