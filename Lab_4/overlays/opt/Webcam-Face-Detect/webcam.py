import cv2
import sys

cascPath = sys.argv[1]
imagePath = sys.argv[2]
faceCascade = cv2.CascadeClassifier(cascPath)

video_capture = cv2.VideoCapture(0)
#video_capture.set(15, 0)

while True:
    print '.'
    ret, frame = video_capture.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    faces = faceCascade.detectMultiScale(
        gray,
        scaleFactor=1.1,
        minNeighbors=5,
        minSize=(120, 120),
        flags=cv2.cv.CV_HAAR_SCALE_IMAGE
    )

    # Draw a rectangle around the faces

    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)

    if not isinstance(faces, tuple):
        print "Face detected"
        cv2.cv.SaveImage(imagePath, cv2.cv.fromarray(frame))

# When everything is done, release the capture
video_capture.release()
