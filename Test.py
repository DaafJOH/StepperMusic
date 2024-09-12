import mido
midi = mido.MidiFile("Pig.mid", clip=True)
#print(len(midi.tracks))
swapList = []
wasOn = 0
wasSwap = 0
for trackIndex, track in enumerate(midi.tracks):
    for messageIndex, message in enumerate(track):
        if message.type == "note_on":
            if not wasOn: wasOn = 1; continue
            if track[messageIndex+1].type == "note_off" and track[messageIndex+1].time == 0: wasSwap = 1; swapList.append(messageIndex, messageIndex+1); continue
            ontick = 0
            for i in track[:messageIndex]:
                ontick += i.time
            print("oi mate, this shit is fucked on message", messageIndex, trackIndex, "and on tick of", ontick)
        elif message.type == "note_off":
            if wasSwap: wasSwap = 0; wasOn = 0; continue
            if wasOn: wasOn = 0; continue
            ontick = 0
            for i in track[:messageIndex]:
                ontick += i.time
            print("oi mate, this shit is fucked on message", messageIndex, trackIndex, "and on tick of", ontick)

print(swapList)









"""a = [
    12, 20, 27, 34, 41, 48, 56, 64, 70, 78, 84, 92, 99, 106, 114, 120, 128, 135, 143, 151, 157, 164, 171, 179, 186, 193, 201, 207, 215, 222, 229, 237, 243, 251, 258, 265, 272, 279, 287,
    294, 301, 308, 316, 323, 330, 337, 345, 352, 359, 365, 373, 381, 388, 395, 402, 410, 417, 424, 432, 438, 446, 452, 459, 467, 474, 482, 488, 496, 504, 510, 518, 525, 533, 538, 546, 555,
    561, 569, 575, 582, 591, 597, 605, 611, 620, 625]
b = []
for i in a:
    b.append(int(i*(720/360)))
print(b)"""