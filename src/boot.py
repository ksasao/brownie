import audio
import gc
import image
import lcd
import sensor
import time
import uos
import KPU as kpu
from fpioa_manager import *
from machine import I2C
from Maix import I2S, GPIO

#
# initialize
#
lcd.init()
lcd.rotation(2)
i2c = I2C(I2C.I2C0, freq=400000, scl=28, sda=29)

fm.register(board_info.SPK_SD, fm.fpioa.GPIO0)
spk_sd=GPIO(GPIO.GPIO0, GPIO.OUT)
spk_sd.value(1) #Enable the SPK output

fm.register(board_info.SPK_DIN,fm.fpioa.I2S0_OUT_D1)
fm.register(board_info.SPK_BCLK,fm.fpioa.I2S0_SCLK)
fm.register(board_info.SPK_LRCLK,fm.fpioa.I2S0_WS)

wav_dev = I2S(I2S.DEVICE_0)

fm.register(board_info.BUTTON_A, fm.fpioa.GPIO1)
but_a=GPIO(GPIO.GPIO1, GPIO.IN, GPIO.PULL_UP) #PULL_UP is required here!

fm.register(board_info.BUTTON_B, fm.fpioa.GPIO2)
but_b = GPIO(GPIO.GPIO2, GPIO.IN, GPIO.PULL_UP) #PULL_UP is required here!

fm.register(board_info.LED_W, fm.fpioa.GPIO3)
led_w = GPIO(GPIO.GPIO3, GPIO.OUT)
led_w.value(1) #RGBW LEDs are Active Low

fm.register(board_info.LED_R, fm.fpioa.GPIO4)
led_r = GPIO(GPIO.GPIO4, GPIO.OUT)
led_r.value(1) #RGBW LEDs are Active Low

fm.register(board_info.LED_G, fm.fpioa.GPIO5)
led_g = GPIO(GPIO.GPIO5, GPIO.OUT)
led_g.value(1) #RGBW LEDs are Active Low

fm.register(board_info.LED_B, fm.fpioa.GPIO6)
led_b = GPIO(GPIO.GPIO6, GPIO.OUT)
led_b.value(1) #RGBW LEDs are Active Low


def play_sound(filename):
    try:
        player = audio.Audio(path = filename)
        player.volume(100)
        wav_info = player.play_process(wav_dev)
        wav_dev.channel_config(wav_dev.CHANNEL_1, I2S.TRANSMITTER,resolution = I2S.RESOLUTION_16_BIT, align_mode = I2S.STANDARD_MODE)
        wav_dev.set_sample_rate(wav_info[1])
        while True:
            ret = player.play()
            if ret == None:
                break
            elif ret==0:
                break
        player.finish()
    except:
        pass

def set_backlight(level):
    if level > 8:
        level = 8
    if level < 0:
        level = 0
    val = (level+7) << 4
    i2c.writeto_mem(0x34, 0x91,int(val))

def show_logo():
    try:
        img = image.Image("/sd/logo.jpg")
        set_backlight(0)
        lcd.display(img)
        for i in range(9):
            set_backlight(i)
            time.sleep(0.1)
        play_sound("/sd/logo.wav")

    except:
        lcd.draw_string(lcd.width()//2-100,lcd.height()//2-4, "Error: Cannot find logo.jpg", lcd.WHITE, lcd.RED)

def initialize_camera():
    err_counter = 0
    while 1:
        try:
            sensor.reset() #Reset sensor may failed, let's try some times
            break
        except:
            err_counter = err_counter + 1
            if err_counter == 20:
                lcd.draw_string(lcd.width()//2-100,lcd.height()//2-4, "Error: Sensor Init Failed", lcd.WHITE, lcd.RED)
            time.sleep(0.1)
            continue

    sensor.set_pixformat(sensor.RGB565)
    sensor.set_framesize(sensor.QVGA) #QVGA=320x240
    sensor.run(1)

def rgb888_to_rgb565(r,g,b):
    r = r >> 3
    g = g >> 2
    b = b >> 3
    return (r << 11)|(g <<5)|b

#
# main
#
show_logo()
if but_a.value() == 0: #If dont want to run the demo
    set_backlight(0)
    print('[info]: Exit by user operation')
    sys.exit()
initialize_camera()

classes = ['aeroplane', 'bicycle', 'bird', 'boat', 'bottle', 'bus', 'car', 'cat', 'chair', 'cow', 'diningtable', 'dog', 'horse', 'motorbike', 'person', 'pottedplant', 'sheep', 'sofa', 'train', 'tvmonitor']
task = kpu.load("/sd/model/20class.kmodel")
anchor = (1.889, 2.5245, 2.9465, 3.94056, 3.99987, 5.3658, 5.155437, 6.92275, 6.718375, 9.01025)
# Anchor data is for bbox, extracted from the training sets.
kpu.init_yolo2(task, 0.5, 0.3, 5, anchor)

print('[info]: Started.')
but_stu = 1

fore_color = rgb888_to_rgb565(119,48,48)
back_color = rgb888_to_rgb565(250,205,137)
try:
    while(True):
        #gc.collect()
        img = sensor.snapshot()
        code = kpu.run_yolo2(task, img)
        if code:
            max_id = 0
            max_rect = 0
            for i in code:
                img.draw_rectangle(i.rect())
                text = ' ' + classes[i.classid()] + ' (' + str(int(i.value()*100)) + '%) '
                for x in range(-1,2):
                    for y in range(-1,2):
                        img.draw_string(x+i.x(), y+i.y()+(i.h()>>1), text, color=(250,205,137), scale=2,mono_space=False)
                img.draw_string(i.x(), i.y()+(i.h()>>1), text, color=(119,48,48), scale=2,mono_space=False)
                id = i.classid()
                rect_size = i.w() * i.h()
                if rect_size > max_rect:
                    max_rect = rect_size
                    max_id = id
            if but_a.value() == 0:
                play_sound("/sd/voice/ja/"+str(max_id)+".wav")
        lcd.display(img)
except KeyboardInterrupt:
    kpu.deinit(task)
    sys.exit()
