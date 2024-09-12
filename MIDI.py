# data type: (pitch, duration, prewait)
import mido, math
midi = mido.MidiFile("Poke.mid", clip=True)

speed = .9 # Hotel 1 Pig .75 Poke .9 Enemy .70
swapList = []
wasOn = 0
wasSwap = 0
ontick = 1
for trackIndex, track in enumerate(midi.tracks):
    for messageIndex, message in enumerate(track):
        if message.type == "note_on":
            if not wasOn: wasOn = 1; continue
            if track[messageIndex+1].type == "note_off" and track[messageIndex+1].time == 0: wasSwap = 1; swapList.append((messageIndex, messageIndex+1, trackIndex)); continue
            ontick = 0
            for i in track[:messageIndex]: ontick += i.time
            print("oi mate, this shit is fucked on message", messageIndex, trackIndex, "and on tick of", ontick)
        elif message.type == "note_off":
            if wasSwap: wasSwap = 0; wasOn = 0; continue
            if wasOn: wasOn = 0; continue
            ontick = 0
            for i in track[:messageIndex]: ontick += i.time
            print("oi mate, this shit is fucked on message", messageIndex, trackIndex, "and on tick of", ontick)

for i in swapList: midi.tracks[i[2]][i[0]], midi.tracks[i[2]][i[1]] = midi.tracks[i[2]][i[1]], midi.tracks[i[2]][i[0]]
if swapList or ontick != 1: exit()

waitTime = 0
preTime = 0
end = [[], [], []]
for trackIndex, track in enumerate(midi.tracks):
    waitTime = 0
    preTime = 0
    for messageIndex, message in enumerate(track):
        if message.type == "note_on": waitTime = message.time + preTime; preTime = 0
        elif message.type == "note_off": end[trackIndex].append((int(1000000/([16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87][(message.note-12)%12]*2**math.floor((message.note-12)/12))), int((message.time + preTime)*(500/midi.ticks_per_beat)*1000), int(waitTime*(500/midi.ticks_per_beat)*1000))); preTime = 0
        else: preTime += message.time

'''
tempMsg = None
speed = .75
end = [[], [], []]
skip = False
for i, track in enumerate(midi.tracks):
    for j, msg in enumerate(track):
        if msg.type == "note_on":
            if track[j+1].type == "note_off" and track[j+1].time == 0 and track[j+1].note == tempMsg.note:
                note = (int(1000000/([16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87][(tempMsg.note-12)%12]*2**math.floor((tempMsg.note-12)/12))), int(msg.time*(500/midi.ticks_per_beat)*1000), int(tempMsg.time*(500/midi.ticks_per_beat)*1000))
                print(note, j, i)
                end[i].append(note)
                tempMsg = msg
                skip = True
            else: tempMsg = msg

        elif msg.type == "note_off" and msg.time != 0:
            note = (int(1000000/([16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87][(tempMsg.note-12)%12]*2**math.floor((tempMsg.note-12)/12))), int(msg.time*(500/midi.ticks_per_beat)*1000), 0 if skip else int(tempMsg.time*(500/midi.ticks_per_beat)*1000))
            skip = False
            end[i].append(note)'''

result = ""
for i, v in enumerate(end):
    a=[];b=[];c=[]
    for j in v:
        a.append(int(j[0]))
        b.append(int(j[1]*speed))
        c.append(int(j[2]*speed))
    result += "const int16_t " + ["a", "b", "c"][i] + "Len = " + str(len(a)-1) + ";\n"
    result += "const uint16_t " + ["a", "b", "c"][i] + "a[] = {" + ", ".join(str(x) for x in a) + "};\n"
    result += "const uint32_t " + ["a", "b", "c"][i] + "b[] = {" + ", ".join(str(x) for x in b) + "};\n"
    result += "const uint32_t " + ["a", "b", "c"][i] + "c[] = {" + ", ".join(str(x) for x in c) + "};\n"
with open("song.h", "w") as file: file.write(result)

print("Done")