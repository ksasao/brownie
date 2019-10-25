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
        for i in range(0,768,3):
            dist = (dist
               + (feature[i]-vec[i])**2
               + (feature[i+1]-vec[i+1])**2
               + (feature[i+2]-vec[i+2])**2)
        if dist < nearest:
            nearest = dist
            name = n
    return name,nearest

def get_unit_vector(vec):
    sum_vec = 0
    unit_vec = []
    for i in range(768):
        sum_vec = sum_vec + vec[i] * vec[i]
    sum_vec = math.sqrt(sum_vec)
    for i in range(768):
        unit_vec.append(vec[i]/sum_vec)
    return unit_vec

def get_angle(unit_vec1,unit_vec2):
    sum_vec = 0
    for i in range(768):
        sum_vec = sum_vec + unit_vec1[i] * unit_vec2[i]
    return math.acos(sum_vec)*180.0/math.pi
def get_dist(a,b,p):
    u = 0
    l = 0
    for i in range(768):
        u = u + (p[i]-a[i])
        l = l + (b[i]-a[i])
    return u / l

def load(filename):
    feature_list=[]
    feature_0=[]
    feature_100=[]
    try:
        with open(filename, 'rt') as f:
            li = f.readline()
            while li:
                li = li.strip().split(',')
                n = str(li[0])
                vec = [float(v) for v in li[1:]]
                if n == '0':
                    feature_0.append([n,get_unit_vec(vec)])
                elif n == '100':
                    feature_100.append([n,get_unit_vec(vec)])
                else:
                    feature_list.append([n,vec])
                li = f.readline()
    except:
        print("no data.")
    return feature_list,feature_0,feature_100

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
feature_list,feature_0,feature_100 = load(feature_file)
task = kpu.load("/sd/model/mbnet751_feature.kmodel")

print('[info]: Started.')

info=kpu.netinfo(task)
#a=kpu.set_layers(task,29)

old_name=''
marker_0_100=0

clock = time.clock()
try:
    while(True):
        img = sensor.snapshot()

        # QR Code check
        res = img.find_qrcodes()
        if len(res) > 0:
            name = res[0].payload()
            if name=="*reset":
                feature_list = []
                feature_0 = []
                feature_100 = []
                save(feature_file, feature_list)
                br.play_sound("/sd/reset.wav")
            else:
                br.play_sound("/sd/camera.wav")
                feature = get_feature(task)
                feature_list.append([name,feature[:]])
                if name=='0':
                    feature_0.append([name,feature[:]])
                if name=='100':
                    feature_100.append([name,feature[:]])
                save(feature_file, feature_list)
                br.play_sound("/sd/set.wav")
                gc.collect()
                # print(gc.mem_free())
                kpu.fmap_free(feature)
            print("[QR]: " + name)
            continue

        # inference
        fmap = kpu.forward(task, img)
        plist=fmap[:]
        clock.tick()
        if len(feature_0)>0 and len(feature_100)>0:
            p = plist
            f0 = feature_0[0]
            f100 = feature_100[0]
            dist = 100.0 * get_dist(f0[1],f100[1],p)
            dist_str = "%.1f"%(dist)
            print("[DISTANCE]: " + dist_str)
            img.draw_string(2,47,  dist_str,scale=3)
            lcd.display(img)
            continue
        name,dist = get_nearest(feature_list,plist)
        #print(clock.fps())
        if dist < 200 and name != "exclude":
            img.draw_rectangle(1,46,222,132,color=br.get_color(0,255,0),thickness=3)
            img.draw_string(2,47 +30,  "%s"%(name),scale=3)
            if old_name != name:
                print("[DETECTED]: " + name)
                lcd.display(img)
                br.play_sound("/sd/voice/"+name+".wav")
                old_name = name
        else:
            old_name = ''

        # output
        img.draw_string(2,47,  "%.2f "%(dist),scale=3)
        lcd.display(img)
        kpu.fmap_free(fmap)
except KeyboardInterrupt:
    kpu.deinit(task)
    sys.exit()
