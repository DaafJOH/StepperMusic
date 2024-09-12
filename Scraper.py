import cv2, mido
from collections import defaultdict

file = cv2.VideoCapture("input.mp4")

if not file.isOpened: exit()

def ColorCompare(color1:list, color2:list, tolerance) -> bool:
    '''Compares 2 lists with a tolerance'''
    if len(color1) != len(color2): raise "Lists must be the same length!"
    for i in range(len(color1)):
        if color1[i]+tolerance < color2[i] or color1[i]-tolerance > color2[i]: return False
    return True

x, y = int(file.get(3)), int(file.get(4))

midi_file = mido.MidiFile()
trackBlue = mido.MidiTrack()
midi_file.tracks.append(trackBlue)

keyHeight = 650 # Height to the keys from the top in pixels
numOfKeys = 86
pixelsFromLeft = 12 # Amount of pixels to the first key from the left
pixelsFromRight = 9 # Amount of pixels from the last key to the right

onOffTolerance = 100
colorTolerance = 100 # Noise sensitivity
blueColor = [255, 0, 0]
greenColor = [0, 255, 0]

frameStart = 100 #100
frameEnd = 690
startKey = 22
customLocations = True
showImage = False

'''
keyHeight = 300 # Height to the keys from the top in pixels
numOfKeys = 75
pixelsFromLeft = 15 # Amount of pixels to the first key from the left
pixelsFromRight = 0 # Amount of pixels from the last key to the right

onOffTolerance = 100
colorTolerance = 100 # Noise sensitivity
blueColor = [255, 100, 100]
greenColor = [0, 255, 100]

frameStart = 125
frameEnd = 690
startKey = 22
customLocations = False
showImage = False'''

tuneKeys = False

FPS = 30  # Frames per second
BPM = 120  # Beats per minute
RESOLUTION = 480  # Ticks per quarter note

stepSize = (x-pixelsFromLeft-pixelsFromRight)/numOfKeys
if not customLocations: pixelValues = [int(i*stepSize+pixelsFromLeft) for i in range(numOfKeys)]
else: pixelValues = [
    24, 40, 54, 68, 82, 96, 112, 128, 140, 156, 168, 184, 198, 212, 228, 240, 256, 270, 286, 302, 314, 328, 342, 358, 372, 386, 402, 414, 430, 444, 458, 474, 486, 502, 516, 530, 544, 558, 574, 588, 602, 616, 632, 646, 660, 674, 690, 704, 718, 730, 746, 762, 776, 790, 804, 820, 834, 848, 864, 876, 892, 904, 918, 934, 948, 964, 976, 992, 1008, 1020, 1036, 1050, 1066, 1076, 1092, 1110, 1122, 1138, 1150, 1164, 1182, 1194, 1210, 1222, 1240, 1250]
wasValue = [[0, 0, 0] for i in range(len(pixelValues))]
frameNum = 0
deltaF = 0
frameEnd = int(file.get(cv2.CAP_PROP_FRAME_COUNT)-frameEnd)
colorValue = [[0, 0, 0] for i in range(len(pixelValues))]

while file.isOpened():
    ret, frame = file.read()
    frameNum+=1
    if frameNum < frameStart: continue
    elif frameNum > frameEnd: break
    if ret:
        if tuneKeys:
            while True:
                newFrame = frame.copy()
                command = input()
                if command == "pass": pass
                elif command == "n": ret, frame = file.read()
                elif command != "done":
                    i, op, v = command.split(" ")
                    if op == "=": pixelValues[int(i)] = int(v)
                    elif op == "+": pixelValues[int(i)] += int(v)
                    elif op == "-": pixelValues[int(i)] -= int(v)
                    print(pixelValues)
                else: break
                newFrame = cv2.line(newFrame, (0, keyHeight), (x, keyHeight), (0, 255, 0), 1)
                for i in pixelValues: newFrame = cv2.line(newFrame, (i, keyHeight-10), (i, keyHeight+10), (255, 0, 0), 1)
                cv2.imshow("Frame", newFrame)
                if cv2.waitKey(1) == ord('q'): break
            print(pixelValues); break
        if frameNum == frameStart:
            for i, v in enumerate(pixelValues): colorValue[i] = frame[keyHeight][v]
            wasValue = colorValue.copy()
        for i, v in enumerate(pixelValues):
            if not ColorCompare(wasValue[i], frame[keyHeight][v], colorTolerance):
                #wasValue[i] = frame[keyHeight][v].copy()
                if frameNum != frameStart:
                    if ColorCompare(blueColor, frame[keyHeight][v], onOffTolerance):
                        trackBlue.append(mido.Message('note_on', note=i+startKey, velocity=100, time=int((frameNum-deltaF)*33.33*.96)))
                        wasValue[i] = blueColor
                        print("note "+str(i)+" turned on", frame[keyHeight][v])
                    elif ColorCompare(greenColor, frame[keyHeight][v], onOffTolerance):
                        trackBlue.append(mido.Message('note_on', note=i+startKey, velocity=100, time=int((frameNum-deltaF)*33.33*.96)))
                        wasValue[i] = greenColor
                        print("note "+str(i)+" turned on", frame[keyHeight][v])
                    else:
                        if ColorCompare(wasValue[i], blueColor, 1): trackBlue.append(mido.Message('note_off', note=i+startKey, velocity=100, time=int((frameNum-deltaF)*33.33*.96)))
                        elif ColorCompare(wasValue[i], greenColor, 1): trackBlue.append(mido.Message('note_off', note=i+startKey, velocity=100, time=int((frameNum-deltaF)*33.33*.96)))
                        wasValue[i] = colorValue[i]
                        print("note "+str(i)+" turned off")
                    deltaF = frameNum
        if showImage:
            #Newframe = cv2.line(frame, (0, keyHeight), (640, keyHeight), (255, 0, 0), 1)
            #for i in pixelValues: Newframe = cv2.line(Newframe, (i, keyHeight-10), (i, keyHeight+10), (0, 0, 255), 1)
            cv2.imshow("Frame", frame)
            if cv2.waitKey(1) == ord('q'): break

if not tuneKeys: midi_file.save('output.mid')
file.release()
cv2.destroyAllWindows()