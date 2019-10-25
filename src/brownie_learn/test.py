import sensor, image, time
import math
sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)

clock = time.clock()

def get_array(val):
    list=[]
    for i in range(768):
        list.append(val)
    return list

def get_unit_vector(vec):
    sum_vec = 0
    unit_vec = vec.copy()
    for i in range(768):
        sum_vec = sum_vec + vec[i] * vec[i]
    sum_vec = math.sqrt(sum_vec)
    for i in range(768):
        unit_vec[i] = vec[i]/sum_vec
    return unit_vec

def get_angle(unit_vec1,unit_vec2):
    sum_vec = 0
    for i in range(768):
        sum_vec = sum_vec + unit_vec1[i] * unit_vec2[i]
    return math.acos(sum_vec)*180.0/math.pi

a = get_array(1)
b = get_array(-1)
#b[0] = 1

clock.tick()
c = get_unit_vector(a)
b = get_unit_vector(b)
print(clock.fps())

print(a)
print(c)

print(get_angle(c,b))
