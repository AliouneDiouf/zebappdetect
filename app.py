
from re import DEBUG, sub
from flask import Flask, render_template, request, redirect, send_file, url_for,Response
from werkzeug.utils import secure_filename, send_from_directory
import os
import subprocess
import glob
from multiprocessing import Process, Queue
import detect
from importlib import import_module
import cv2 
import sys
import numpy as np
from flaskwebgui import FlaskUI



app = Flask(__name__)

#ui = FlaskUI(app, width=1000, height=1000) 

uploads_dir = os.path.join(app.instance_path, 'uploads')

os.makedirs(uploads_dir, exist_ok=True)

print(sys.version)
  





  
@app.route("/")
def hello_world():
    return render_template('index.html')


@app.route("/detect", methods=['POST'])
def detecte():
    if not request.method == "POST":
        return
    video = request.files['video']
    video.save(os.path.join(uploads_dir, secure_filename(video.filename)))
    print(video)
    result = subprocess.run([sys.executable, 'detect.py', '--source', os.path.join(uploads_dir, secure_filename(video.filename))],capture_output=True, text=True)
    print('------------')
    print('voici les resultats :',result)
    num1=0
    numret=0
    num2=0
    numadv=0
    numdead=0
    numtot=0
    
    if 'returned' in str(result):
        ret= str(result).find('returned')
        numret = str(result)[ret-2]
        
    
    if 'stage1' in str(result):
        un= str(result).find('stage1')
        num1 = str(result)[un-2]
        
    if 'stage2-4' in str(result):  
        deux= str(result).find('stage2-4')
        num2 = str(result)[deux-2]
    
    if 'advanced' in str(result):  
        adv= str(result).find('advanced')
        numadv = str(result)[adv-2]
    
    if 'dead' in str(result):  
        dead= str(result).find('dead')
        numdead = str(result)[dead-2]
    
  

    numtot = int(num1) +  int(num2) +  int(numadv) +  int(numret) +  int(numdead)
    
    
    print('Les stats::::: ', num1,num2,numadv,numret,numdead,numtot)

    # return os.path.join(uploads_dir, secure_filename(video.filename))
    obj = secure_filename(video.filename)
    #gg= max(glob.glob(os.path.join("/runs/detect", '*/')), key=os.path.getmtime)
    directory = 'static'
    gg= max([os.path.join(directory,d) for d in os.listdir(directory)], key=os.path.getmtime)
    print(gg)
    gb=os.path.join(gg, obj)
    gb=[gb,num1,num2,numadv,numret,numdead,numtot]
    
    


    return gb



def testDevice(source):
    cap = cv2.VideoCapture(source) 
    if cap is None or not cap.isOpened():
        print('Warning: unable to open video source: ', source)
    
    
@app.route("/opencam", methods=['GET'])
def opencam():
    print("Proceeding on LIVE-------------PLEASE WAIT A MOMENT")
    #subprocess.run([sys.executable , 'detect.py', '--source', '0' ])
    subprocess.run([sys.executable , 'detect.py', '--source', '0' ])
#'rtsp://192.168.56.1:8554/mystream'
    print('------------')
    directory = 'static'
    gg= max([os.path.join(directory,d) for d in os.listdir(directory)], key=os.path.getmtime)
    gg+='/0.mp4'
    print(gg)
    return gg

    

@app.route('/return-files', methods=['GET'])
def return_file():
    obj = request.args.get('obj')
    loc = os.path.join("runs/detect", obj)
    print(loc)
    try:
        return send_file(os.path.join("runs/detect", obj), attachment_filename=obj)
        #return send_from_directory(loc, obj)
    except Exception as e:
        return str(e)
    
    
@app.route('/live')
def live():
    return render_template('live.html')
@app.route('/about')
def about():
    return render_template('about.html')

@app.route('/test')
def test():
    return render_template('test.html')
@app.route('/contact')
def contact():
    return render_template('contact.html')

if __name__ == "__main__":
    #ui.run()
    app.run()
    #FlaskUI(app=app, server="flask").run()







# @app.route('/display/<filename>')
# def display_video(filename):
# 	#print('display_video filename: ' + filename)
# 	return redirect(url_for('static/video_1.mp4', code=200))