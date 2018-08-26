from flask import Flask, Response, send_file
import os
app = Flask(__name__)

@app.route('/')
def hello_world():
	return "Hello, World!"
	
@app.route('/user/<int:post_id>')
def show_post(post_id):
	return 'Pose %d' % post_id
	
@app.route("/image/<imageid>")
def index(imageid):
	filename = "{}.jpg".format(imageid)
	# image = file(filename)
	# resp = Response(image, mimetype="image/jpeg")
	# return resp
	return send_file(filename, mimetype="image/jpeg")
	