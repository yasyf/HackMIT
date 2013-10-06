#!/usr/bin/env python

# Developed by Yasyf Mohamedali @ HackMIT 2013
# https://github.com/yasyf/HackMIT

from flask import Flask, session, redirect, url_for, escape, request, render_template
from functions import *

app = Flask(__name__)
app.secret_key = open('sk.txt',"r").read().strip()

	
@app.route('/')
def index():
	if 'userid' in session:
		return render_template('index.html', notifications=get_notifications(session["userid"]))
	else:
		return redirect(url_for('login', error="You must log in to do that."))

@app.route('/sms')
def sms_response():
	return twilio_sms_response(request.values.get('Body', ''))

@app.route('/logout')
def logout():
	session.pop('userid')
	return redirect(url_for('index'))

@app.route('/login', methods=['POST', 'GET'])
def login():
	if request.method == 'POST':
		username = request.form.get('username','')
		password = request.form.get('password','')
		if check_credentials(username,password):
			session["userid"] = get_userid(username)
			return redirect(url_for('index'))
		elif validate_credentials(username,password):
			session["userid"] = create_user(username,password)
			return redirect(url_for('index'))
		else:
			return render_template('login.html',error="Your credentials were invalid.")
	else:
		if 'userid' in session:
			return redirect(url_for('index'))
		else:
			return render_template('login.html',error=request.args.get('error', ''))

@app.route('/api/notification/create/<userid>/<source>/<text>', methods=['POST', 'GET'])
def new_notification(userid, source, text):
	if check_userid(userid):
		if len(source) > 0 and len(text) > 0:
			return create_notification(userid, source, text)
		else:
			error = "missing required parameter"
	else:
		error = "invalid userid"
	return show_error(error)

@app.route('/api/notification/get/<userid>', methods=['POST', 'GET'])
def retreive_most_recent_notification(userid):
	if check_userid(userid):
		return get_most_recent_notification(userid)
	else:
		return show_error("invalid userid")

@app.route('/api/notification/get/<userid>/<notificationid>', methods=['POST', 'GET'])
def retreive_notification(userid, notificationid):
	if check_userid(userid) and check_notificationid(notificationid) and compare_ids(userid, notificationid):
		return get_notification(notificationid, userid)
	else:
		return show_error("invalid notificationid for this userid")

@app.route('/api/notification/delivered/<userid>', methods=['POST', 'GET'])
def notification_delivered(userid):
	if check_userid(userid):
		mark_notification_delivered(userid, True)
		return json_encode("status","success")
	else:
		return show_error("invalid notificationid for this userid")

	
if __name__ == '__main__':
    app.run(host='0.0.0.0',port=8080,debug=False)