#!/usr/bin/env python

import hashlib, datetime, json, calendar
from pymongo import MongoClient
from bson.objectid import ObjectId
from bson import json_util

client = MongoClient(open('db.txt',"r").read().strip())
db = client.hackmit
users = db.users
notifications = db.notifications

def isset_credentials(username, password):
	return (len(username) > 0) and (len(password) > 0)

def validate_credentials(username, password):
	return isset_credentials(username, password) and (users.find({"username": username}).count() == 0)

def check_credentials(username, password):
	return isset_credentials(username, password) and (users.find({"username": username}).count() != 0) and (sha1(password) == users.find({"username": username})[0]["password"])

def get_userid(username):
	return str(users.find({"username": username})[0]["_id"])

def check_userid(userid):
	return (users.find({"_id": ObjectId(userid)}).count() == 1)

def check_notificationid(notificationid):
	return (notifications.find({"_id": ObjectId(notificationid)}).count() == 1)
	
def sha1(text):
	m = hashlib.sha1()
	m.update(text)
	return m.hexdigest()

def json_encode(k,v):
	return json.dumps({k: v})

	
def create_user(username, password):
	return str(users.insert({"username": username, "password": sha1(password)}))
	
def create_notification(userid, source, text):
	return json_encode("notificationid", str(notifications.insert({"userid": userid, "time": datetime.datetime.utcnow(), "source": source, "text": text, "delivered": "false"})))

def get_notification(notificationid, userid):
	obj = notifications.find({"_id": ObjectId(notificationid), "userid": userid})[0]
	return json.dumps({"1": obj["source"], "2": obj["text"]})

def get_most_recent_notification(userid):
	notificationid = str(notifications.find({"userid": userid}).sort("date", -1)[0]["_id"])
	return get_notification(notificationid, userid)

def compare_ids(userid, notificationid):
	return (userid == notifications.find({"_id": ObjectId(notificationid)})[0]["userid"])
	
def show_error(text):
	return json_encode("error",text)
		
def mark_notification_delivered(notificationid, delivered):
	if delivered == True:
		status = "true"
	else:
		status = "false"
	return str(notifications.update({"_id": ObjectId(notificationid)}, {"$set": {"delivered": status}}))