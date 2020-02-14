#
# Brownie Learn
#
# Firmware: maixpy_v0.5.0_9_g8eba07d_m5stickv.bin
# http://dl.sipeed.com/MAIX/MaixPy/release/master/maixpy_v0.5.0_9_g8eba07d
#
import brownie as br
import KPU as kpu
import sensor
import lcd
import time
import uos
import gc
import ulab as np
from fpioa_manager import *
from Maix import utils
from machine import UART

utils.gc_heap_size(250000)

# for Grove Port
fm.register(35, fm.fpioa.UART2_TX, force=True)
fm.register(34, fm.fpioa.UART2_RX, force=True)
uart_Port = UART(UART.UART2, 115200,8,0,0, timeout=1000, read_buf_len= 4096)

def get_feature(task):
    count = 20
    for i in range(3):
        for j in range(count):
            img = sensor.snapshot()
            if j < (count>>1):
                br.led_r.value(0)
                img.draw_rectangle(1,46,222,132,color=br.get_color(255,0,0),thickness=3)
            else:
                br.led_r.value((1))
            lcd.display(img)
    time.sleep(1.0)
    feature = kpu.forward(task,img)
    return np.array(feature[:])

def get_nearest(feature_list,feature):
    min_dist = 10000
    name = ''
    vec = []
    for n,v in feature_list:
        dist = np.sum((v-feature)*(v-feature))
        if dist < min_dist:
            min_dist = dist
            name = n
            vec = v
    return name,min_dist,vec

def get_dist(a,b,p):
    u = np.sum((p-a)*(b-a))
    l = np.sum((b-a)*(b-a))
    t = u / l
    x = a + (b-a) * t
    dist = np.sum((p-x)*(p-x))
    if dist > 200.0:
        return 50.0
    else:
        return t

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
                vec = np.array([float(v) for v in li[1:]])
                if n == '0':
                    feature_0.append([n,vec])
                elif n == '100':
                    feature_100.append([n,vec])
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

def send_packet(message):
    data_packet = bytearray(message+'\x00')
    uart_Port.write(data_packet)
#
# main
#
br.show_logo()
br.exit_check()

feature_file = "/sd/features.csv"
feature_list,feature_0,feature_100 = load(feature_file)
task = kpu.load("/sd/model/mbnet751_feature.kmodel")

br.initialize_camera()

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
                feature_list.append([name,feature])
                if name=='0':
                    feature_0.append([name,feature])
                if name=='100':
                    feature_100.append([name,feature])
                save(feature_file, feature_list)
                br.play_sound("/sd/set.wav")
                gc.collect()
                # print(gc.mem_free())
                kpu.fmap_free(feature)
            print("[QR]: " + name)
            continue

        # inference
        fmap = kpu.forward(task, img)
        p = np.array(fmap[:])

        # get ratio between feature0 and feature100
        if len(feature_0)>0 and len(feature_100)>0:
            _,_,f0 = get_nearest(feature_0,p)
            _,_,f100 = get_nearest(feature_100,p)
            dist = 100.0 * get_dist(f0,f100,p)
            dist_str = "%.1f"%(dist)

            print("[DISTANCE]: " + dist_str)
            img.draw_string(2, 47, dist_str, scale=3)
            lcd.display(img)
            continue

        # get nearest target
        name,dist,_ = get_nearest(feature_list,p)
        if dist < 200 and name != "*exclude":
            img.draw_rectangle(1,46,222,132,color=br.get_color(0,255,0),thickness=3)
            img.draw_string(2, 47 +30, "%s"%(name), scale=3)
            if old_name != name:
                print("[DETECTED]: " + name)
                lcd.display(img)
                send_packet(name)
                br.play_sound("/sd/voice/"+name+".wav")
                old_name = name
        else:
            if old_name != '':
                send_packet('')
            old_name = ''

        # output
        img.draw_string(2,47,  "%.2f "%(dist),scale=3)
        lcd.display(img)
        kpu.fmap_free(fmap)
except KeyboardInterrupt:
    kpu.deinit(task)
    sys.exit()
