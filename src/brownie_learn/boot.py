import brownie as br
import KPU as kpu
import sensor
import lcd
import time
import uos
import gc

def get_feature(task):
    count = 20
    for i in range(3):
        for j in range(count):
            img = sensor.snapshot()
            if j < (count>>1):
                img.draw_rectangle(1,46,222,132,color=br.get_color(255,0,0),thickness=3)
            lcd.display(img)
    time.sleep(1.0)
    feature = kpu.forward(task,img)
    return feature

def get_nearest(feature_list,feature):
    nearest = 10000
    name = ''
    for n,vec in feature_list:
        dist = 0
        for i in range(768):
            dist = dist + (feature[i]-vec[i])*(feature[i]-vec[i])
        if dist < nearest:
            nearest = dist
            name = n
    return name,nearest

def load(filename):
    feature_list=[]
    try:
        with open(filename, 'rt') as f:
            li = f.readline()
            while li:
                li = li.strip().split(',')
                n = str(li[0])
                vec = [float(v) for v in li[1:]]
                feature_list.append([n,vec])
                li = f.readline()
    except:
        print("no data.")
    return feature_list

def save(filename,feature_list):
    gc.collect()
    output = ""
    try:
        with open(filename, 'wt') as f:
            for n,vec in feature_list:
                f.write(n)
                for v in vec:
                    vec_str=",{0:.5f}".format(v)
                    f.write(vec_str)
                f.write('\n')
    except:
        print("write error.")
#
# main
#
br.show_logo()
br.exit_check()
br.initialize_camera()

feature_file = "/sd/features.csv"
feature_list = load(feature_file)
task = kpu.load("/sd/model/mbnet751.kmodel")

print('[info]: Started.')

info=kpu.netinfo(task)
a=kpu.set_layers(task,29)

old_name=''
try:
    while(True):
        img = sensor.snapshot()

        # QR Code check
        res = img.find_qrcodes()
        if len(res) > 0:
            name = res[0].payload()
            print(name)
            if name=="*reset":
                feature_list = []
                save(feature_file, feature_list)
                br.play_sound("/sd/reset.wav")
            else:
                br.play_sound("/sd/camera.wav")
                feature = get_feature(task)
                feature_list.append([name,feature[:]])
                save(feature_file, feature_list)
                br.play_sound("/sd/set.wav")
                kpu.fmap_free(feature)
            print("QR: " + name)
            continue

        # inference
        fmap = kpu.forward(task, img)
        plist=fmap[:]
        name,dist = get_nearest(feature_list,plist)
        if dist < 200 and name != "exclude":
            img.draw_rectangle(1,46,222,132,color=br.get_color(0,255,0),thickness=3)
            img.draw_string(2,47 +10,  "%s"%(name))
            if old_name != name:
                print(name)
                lcd.display(img)
                br.play_sound("/sd/voice/"+name+".wav")
                old_name = name
        else:
            old_name = ''

        # output
        img.draw_string(2,47,  "%.2f "%(dist))
        lcd.display(img)
        kpu.fmap_free(fmap)
except KeyboardInterrupt:
    kpu.deinit(task)
    sys.exit()
